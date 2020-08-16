from getopt import getopt, GetoptError
from ctypes import *
from PIL import Image
import sys, os
import json
import math

#abre o catalogo que esta na pasta de pixagens
def carregar_catalogo():
    try:
        with open(CAMINHO_REL_PASTA_PIXEL + '_catalogo.json', 'r') as obj:
            return json.load(obj)
    except:
            raise Exception("O catalogo ou esta vazio ou nao existe")

#corta a imagem para conter uma quantidade extada de pixagens
def ajustar_tam(imagem_PIL, largura_pixagens, altura_pixagens):

    #quantidade maxima de pixagens por eixo da imagem
    pixagensX = math.floor(imagem_PIL.width / largura_pixagens)
    pixagensY = math.floor(imagem_PIL.height / altura_pixagens)

    nova_largura = pixagensX * largura_pixagens
    nova_altura = pixagensY * altura_pixagens
    
    #checa se o tamanho ja esta certo e nao precisa cortar
    if nova_altura == imagem_PIL.width and nova_largura == imagem_PIL.height:
        return imagem_PIL, pixagensX, pixagensY

    #corta as bordas da imagem para caber uma quantidade exata de pixagens
    nova_imagem = imagem_PIL.crop((0, 0, nova_largura, nova_altura))

    return  nova_imagem, pixagensX, pixagensY

#salva o nome 
def salvar_img(imagem_PIL):
    contador = 0
    nome_img = 'saida' + str(contador) + '.png'

    while nome_img in os.listdir(CAMINHO_REL_PASTA_SAIDA):
        contador += 1
        nome_img = 'saida' + str(contador) + '.png'

    caminho_img = CAMINHO_REL_PASTA_SAIDA + nome_img

    imagem_PIL.save(caminho_img)

    return 0



def main():
    #abre a imagem que vai ser convertida
    imagem_base = Image.open(CAMINHO_REL_IMAGEM_BASE)
    #talvez faca um teste antes de converter sla
    imagem_base = imagem_base.convert('RGB')

    #vai para a pasta das pixagens

    #abre o catalo das pixagens
    catalogo = carregar_catalogo()

    try:
        padrao_pixagens = catalogo.pop('padrao_imagens')
        #padrao_pixagens é uma lista que contém nessa ordem
        #largura, altura e o padrao de cores das imagens
    except KeyError:
        raise Exception("o catalogo nao possui a chave 'padrao_imagens'")

    #pixelsX = padrao_pixagens[0]
    #pixelsY = padrao_pixagens[1]

    #larguara e altura das pixagens
    largura_pixagens, altura_pixagens, __ = padrao_pixagens

    #pixagens são as imagens menores que vão compor a imagem base
    #pixagensX e Y são a quantidade de pixagens na largura e altura da imagem base
    imagem_base, pixagensX, pixagensY = ajustar_tam(imagem_base, largura_pixagens, altura_pixagens)
    pixagens_total = pixagensX * pixagensY
    largura_imagem, altura_imagem = imagem_base.size

    #faz as coisas do ctype
    dll = CDLL(CAMINHO_REL_DLL)

    dll.media.restype = c_void_p
    dll.media.argtypes = [c_int, c_int, c_int, c_int, c_char_p]

    dll.imagemMaisProxima.restype = c_void_p
    dll.imagemMaisProxima.argtypes = [c_char_p, c_int, c_char_p, c_int]
    
    #a lista de pixels da imagem, mas cada pixel individual é uma tupla
    pixels_img_multd = list(imagem_base.getdata())
    #lista de pixels só que uma lista continua de numeros 
    pixels_img = [c for RGB in pixels_img_multd for c in RGB]
    #ponteiro para a lista continua com ctypes
    p_pixels_imagem = (c_char * len(pixels_img))(*pixels_img)   

    #carrega todas as imagens usadas para pixagens na memoria
    pixagens = []
    for nome_imagem in catalogo:
        try:
            caminho_imagem = CAMINHO_REL_PASTA_PIXEL + nome_imagem
            pixagens.append(Image.open(caminho_imagem).convert('RGB'))
        except FileNotFoundError:
            print(f'a imagem "{nome_imagem}" nao pode ser aberta ou nao existe')

    #lista continua da media rgb das pixagens
    lista_media_pixagens = [rgb for medias in catalogo.values() for rgb in medias]
    #ponteiro para as medias das pixagens
    p_media_pixagens = (c_char * len(lista_media_pixagens))(*lista_media_pixagens)

    #rodam as funcoes da dll e transformam o return em um ponteiro
    endereco_medias = dll.media(altura_pixagens, largura_pixagens, 
        altura_imagem, largura_imagem, p_pixels_imagem)

    medias = cast(endereco_medias, POINTER(c_char))

    endereco_indices = dll.imagemMaisProxima(medias, pixagens_total * 3,
        p_media_pixagens, len(p_media_pixagens))

    indices = cast(endereco_indices, POINTER(c_int))

    #colagem é a imagem que vai ser montada por pixagens
    colagem = Image.new(imagem_base.mode, imagem_base.size)

    #vai colar as imagens escolidas na colagem
    for y in range(pixagensY):
        for x in range(pixagensX):
            pontos_upLeft = (x * largura_pixagens, y * altura_pixagens)
            indice_escolido = indices[y*pixagensX + x]
            colagem.paste(pixagens[indice_escolido], pontos_upLeft)

    salvar_img(colagem) 

    return 0
    

#camihos padroes se nao foram especificados
CAMINHO_REL_IMAGEM_BASE = '../assets/exemplos/rua.jpg'
CAMINHO_REL_PASTA_PIXEL = '../assets/coresBasicas/'
CAMINHO_REL_PASTA_SAIDA = '../assets/saida/'
CAMINHO_REL_DLL = '../dll/agregador.dll'

if __name__ == '__main__':
    #pega parametros passados pela linha de comando
    if(len(sys.argv) > 1):
        try:
            opts, args = getopt(sys.argv[1:],"b:p:s:d:")
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
