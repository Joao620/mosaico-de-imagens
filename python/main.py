from getopt import getopt, GetoptError
from ctypes import *
from PIL import Image
import sys, os
import numpy as np
import json
import math

def carregar_catalogo():
    try:
        with open('_catalogo.json', 'r') as obj:
            return json.load(obj)
    except:
            raise Exception("O catalogo ou esta vazio ou nao existe")

def ajustar_tam(imagem_PIL, pixelsx, pixelsy):

    imagem_largura = math.floor(imagem_PIL.width / pixelsx)
    imagem_altura = math.floor(imagem_PIL.height / pixelsy)

    if imagem_largura < imagem_altura:
        valor_menor_lado = imagem_largura
    else:
        valor_menor_lado = imagem_altura

    corte_x = imagem_PIL.width - valor_menor_lado * pixelsx
    corte_y = imagem_PIL.height - valor_menor_lado * pixelsy

    nova_largura =  imagem_PIL.width - corte_x
    nova_altura = imagem_PIL.height - corte_y

    ###todo checa se  tamanho vai ficar igual pra economizar uns segundos

    #imagem_PIL.crop((nova_altura, nova_largura), Image.EXTENT, (0, 0, nova_altura, nova_largura))
    return imagem_PIL.crop((0, 0, nova_largura, nova_altura))

def main():
    #cara hulmildade total mas colocas uns objetos aqui dps


    #abre a imagem que vai ser convertida
    imagem_base = Image.open(CAMINHO_REL_IMAGEM_BASE)
    #talvez faca um teste antes de converter sla
    imagem_base = imagem_base.convert('RGB')

    #vai para a pasta das pixagens
    os.chdir(CAMINHO_REL_PASTA_PIXEL)

    #abre o catalo das pixagens
    catalogo = carregar_catalogo()
    ##todo d+++ abre todas as imagem na memoria ja
    try:
        padrao_pixagens = catalogo.pop('padrao_imagens')    
    except KeyError:
        print('o catologo nao existe')
        print('depois eu tenho que criar ele automaticamente')

    ##todo adicionar um checar padrao_pixagens

    pixelsX = padrao_pixagens[0]
    pixelsY = padrao_pixagens[1]

    imagem_base = ajustar_tam(imagem_base, pixelsX, pixelsY)
    largura_imagem, altura_imagem = imagem_base.size

    colagem = Image.new(imagem_base.mode, imagem_base.size)

    dll = CDLL("/home/joao/Documents/Programas/mosaico-de-imagens/codigo c/a")

    dll.media.restype = c_char_p
    dll.media.argtypes = [c_int, c_int, c_int, c_int, c_char_p]

    dll.imagemMaisProxima.restype = c_void_p
    dll.imagemMaisProxima.argtypes = [c_char_p, c_int, c_char_p, c_int]

    pixels_img_multd = list(imagem_base.getdata())
    pixels_img = [RGB[c] for RGB in pixels_img_multd for c in range(3)]
    
    # temp = c_char * len(pixels_img)
    # p_lista_pixels = temp(*pixels_img)

    p_pixels_imagem = (c_char * len(pixels_img))(*pixels_img)

    pixagens = []

    for x in catalogo:
        try:
            pixagens.append(Image.open(x))
        except FileNotFoundError:
            print(f'a imagem {x} nao pode ser aberta ou nao existe')

    medias_pixagens = [RGB[c] for RGB in catalogo for c in range(3)]

    p_medias_pixagens = (c_char * len(medias_pixagens))(*medias_pixagens)

    endereco_resultado = dll.media(pixelsX, pixelsY, 
        largura_imagem, altura_imagem, p_pixels_imagem)

    resultado = cast(endereco_resultado, POINTER(c_char))





    quantidadeBlocosX = largura_imagem / pixelsX
    quantidadeBlocosY = altura_imagem / pixelsY

    blocos_total = int(quantidadeBlocosX * quantidadeBlocosY)

    for x in range(0, blocos_total * 3, 3):
        v1 = ord(resultado[x])
        v2 = ord(resultado[x+1])
        v3 = ord(resultado[x+2])
        print(f"R:{v1} G:{v2} B:{v3}")
    


CAMINHO_REL_IMAGEM_BASE = '../assets/teste.png'
CAMINHO_REL_PASTA_PIXEL = 'assets/chunks'

if __name__ == '__main__':
    if(len(sys.argv) > 1):
        try:
            opts, args = getopt(sys.argv[1:],"b:p:")
        except GetoptError:
            print("algum argumento invalido")
            sys.exit(-1)

        for opt, arg in opts:
            if opt == '-b':
                CAMINHO_REL_IMAGEM_BASE = arg
            elif opt == '-p':
                CAMINHO_REL_PASTA_PIXEL = arg

    main()
