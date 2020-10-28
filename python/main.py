import json
from math import floor
import os
import sys
from ctypes import *
from getopt import GetoptError, getopt

from PIL import Image


class Imagem(Structure):
    _fields_ = [
        ('imagem', POINTER(c_char)),
        ('tamanho', c_int),
        ('altura', c_int),
        ('largura', c_int)
    ]


class Pixagens:
    lista = []
    largura = 0
    altura = 0


def abrir_imagem_base(caminho_imagem, largura_pixagens, altura_pixagens):
    # abre a imagem que vai ser convertida
    imagem_base = Image.open(CAMINHO_REL_IMAGEM_BASE)
    # talvez faca um teste antes de converter sla
    imagem_base = imagem_base.convert('RGB')
    # pixagens são as imagens menores que vão compor a imagem base
    # pixagensX e Y são a quantidade de pixagens na largura e altura da imagem base
    imagem_base = ajustar_tam(imagem_base, largura_pixagens, altura_pixagens)
    return imagem_base


def abrir_pixagens(catalogo, caminho_textura):
    # carrega todas as imagens usadas para pixagens na memoria
    pixagens = []
    for nome_imagem in catalogo:
        try:
            caminho_imagem = caminho_textura + nome_imagem
            pixagens.append(Image.open(caminho_imagem).convert('RGB'))
        except FileNotFoundError:
            print(
                f'a imagem "{nome_imagem}" nao pode ser aberta ou nao existe')

    return pixagens

# abre o catalogo que esta na pasta de pixagens


def carregar_catalogo():
    try:
        with open(CAMINHO_REL_PASTA_PIXEL + '_catalogo.json', 'r') as obj:
            catalogo = json.load(obj)
            # padrao_pixagens é uma lista que contém nessa ordem
            # largura, altura e o padrao de cores das imagens
            padrao_pixagens = catalogo.pop('padrao_imagens')
            return (catalogo, *padrao_pixagens)
    except:
        raise Exception("O catalogo não existe ou esta estraviado")

# corta a imagem para conter uma quantidade extada de pixagens


def ajustar_tam(imagem_PIL, largura_pixagens, altura_pixagens):

    # quantidade maxima de pixagens por eixo da imagem
    pixagensX = floor(imagem_PIL.width / largura_pixagens)
    pixagensY = floor(imagem_PIL.height / altura_pixagens)

    nova_largura = pixagensX * largura_pixagens
    nova_altura = pixagensY * altura_pixagens

    # checa se o tamanho ja esta certo e nao precisa cortar
    if nova_altura == imagem_PIL.width and nova_largura == imagem_PIL.height:
        return imagem_PIL, pixagensX, pixagensY

    # corta as bordas da imagem para caber uma quantidade exata de pixagens
    nova_imagem = imagem_PIL.crop((0, 0, nova_largura, nova_altura))

    return nova_imagem

# salva o nome


def salvar_img(imagem_PIL):
    contador = 0
    nome_img = 'saida' + str(contador) + '.png'

    while nome_img in os.listdir(CAMINHO_REL_PASTA_SAIDA):
        contador += 1
        nome_img = 'saida' + str(contador) + '.png'

    caminho_img = CAMINHO_REL_PASTA_SAIDA + nome_img

    imagem_PIL.save(caminho_img)

    return 0

# configura os argtypes e restypes das dll's


def configurarDll(caminhoDll):
    DLL = CDLL(caminhoDll)

    DLL.media.restype = c_void_p
    DLL.media.argtypes = [POINTER(Imagem), c_int, c_int]

    DLL.imagemMaisProxima.restype = c_void_p
    DLL.imagemMaisProxima.argtypes = [c_char_p, c_int, c_char_p, c_int, c_int]

    return DLL


def gerar_medias(funcao_DLL, imagem_base, pixagens):
    # a lista de pixels da imagem, mas cada pixel individual é uma tupla
    pixels_img_multd = list(imagem_base.getdata())

    # lista de pixels só que uma lista continua de numeros
    pixels_img = [c for RGB in pixels_img_multd for c in RGB]

    # ponteiro para a lista continua com ctypes
    p_pixels_imagem = (c_char * len(pixels_img))(*pixels_img)

    tamanho_imagem = imagem_base.width * imagem_base.height
    imagem = Imagem(p_pixels_imagem, tamanho_imagem,
                    imagem_base.width, imagem_base.height)

    # rodam as funcoes da dll e transformam o return em um ponteiro
    endereco_medias = funcao_DLL(
        byref(imagem), pixagens.altura, pixagens.largura)

    medias = cast(endereco_medias, POINTER(c_char))

    return medias


def buscar_pixagens(funcao_DLL, medias_blocos_imagem, tamanho_medias_blocos, catalogo, aleatoridade):
    # lista continua da media rgb das pixagens
    lista_media_pixagens = [rgb for medias_pixagens in catalogo.values()
                            for rgb in medias_pixagens]
    # ponteiro para as medias das pixagens
    p_media_pixagens = (c_char * len(lista_media_pixagens)
                        )(*lista_media_pixagens)

    endereco_indices = funcao_DLL(medias_blocos_imagem, tamanho_medias_blocos,
                                  p_media_pixagens, len(p_media_pixagens), aleatoridade)

    indices = cast(endereco_indices, POINTER(c_int))

    return indices


def main():
    # abre o catalo das pixagens

    pixagens = Pixagens()

    catalogo, pixagens.largura, pixagens.altura, __ = carregar_catalogo()

    pixagens.lista = abrir_pixagens(catalogo, CAMINHO_REL_PASTA_PIXEL)

    imagem_base = abrir_imagem_base(
        CAMINHO_REL_IMAGEM_BASE, pixagens.largura, pixagens.altura)

    pixagensX = floor(imagem_base.width / pixagens.largura)
    pixagensY = floor(imagem_base.height / pixagens.altura)
    quant_pixagens = pixagensX * pixagensY

    DLL = configurarDll(CAMINHO_REL_DLL)

    medias_blocos_imagem = gerar_medias(DLL.media, imagem_base, pixagens)

    tamanho_medias_blocos = quant_pixagens * 3

    indices = buscar_pixagens(
        DLL.imagemMaisProxima, medias_blocos_imagem, tamanho_medias_blocos, catalogo, 2)

    # colagem é a imagem que vai ser montada por pixagens
    colagem = Image.new(imagem_base.mode, imagem_base.size)

    # vai colar as imagens escolidas na colagem
    for y in range(pixagensY):
        for x in range(pixagensX):
            pontos_upLeft = (x * pixagens.largura, y * pixagens.altura)
            indice_escolido = indices[y*pixagensX + x]
            colagem.paste(pixagens.lista[indice_escolido], pontos_upLeft)

    salvar_img(colagem)

    return 0


# camihos padroes se nao foram especificados
CAMINHO_REL_IMAGEM_BASE = '../assets/exemplos/paisagem2.jpg'
CAMINHO_REL_PASTA_PIXEL = '../assets/texturaMinecraft/'
CAMINHO_REL_PASTA_SAIDA = '../assets/saida/'
CAMINHO_REL_DLL = '../dll/agregador.dll'

if __name__ == '__main__':
    # pega parametros passados pela linha de comando
    if(len(sys.argv) > 1):
        try:
            opts, args = getopt(sys.argv[1:], "b:p:s:d:")
        except GetoptError:
            print("algum argumento invalido")
            sys.exit(-1)

        for opt, arg in opts:
            if opt == '-b':
                CAMINHO_REL_IMAGEM_BASE = arg
            elif opt == '-d':
                CAMINHO_REL_DLL = arg
            elif opt == '-p':
                CAMINHO_REL_PASTA_PIXEL = arg
                if CAMINHO_REL_PASTA_PIXEL[-1] != '/':
                    CAMINHO_REL_PASTA_PIXEL += '/'
            elif opt == '-s':
                CAMINHO_REL_PASTA_SAIDA = arg
                if CAMINHO_REL_PASTA_SAIDA[-1] != '/':
                    CAMINHO_REL_PASTA_PIXEL += '/'
    main()
