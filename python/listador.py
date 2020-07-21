#ele vai pegar uma pasta, e criar um json nela, listando a media de cores de cada imagem na pasta
#e juntar todas essas informacoes em um catalogo. funcoes que ele contem
#uma que vai pegar uma pasta inteira para listar todas as imagem, checar se ja tem um catalogo
#outra para remover item da lista, junto com seu arquivo
#e adicionar item na mao mesmo se quiser
'''
estrutura catalogo

dict gigante??
{
    "padrao_imagens" : [120, 120, "RGB"]
    "arquivo1.png" : (123, 221, 24),
    "arquivo2.png" : (151, 172, 38)
}

'''

import sys, os
from PIL import Image
from getopt import getopt, GetoptError 
import json
import numpy as np
import re

caminho = "assets/texturaMinecraft"
#'all' para pasta toda, 'rem' para remover arquivos e 'add' para adicionar arquivos
modo = 'all'

def carregar_catalogo():
    try:
        with open("_catalogo.json", "r") as obj:
            catalogo = json.load(obj)
    except:
        return definir_catalogo()

def salvar_catalogo(catalogo):
    with open("_catalogo.json", "w") as obj:
        json.dump(catalogo, obj)

def cores_medias_imagemRGB(PIL_imagem):
    pixels = np.array(PIL_imagem)
    soma_pixels_RGB = pixels.sum(1)

    soma_pixels_canal_R = soma_pixels_RGB[:, 0].sum()
    soma_pixels_canal_G = soma_pixels_RGB[:, 1].sum()
    soma_pixels_canal_B = soma_pixels_RGB[:, 2].sum()

    imagem_quant_pixels = PIL_imagem.height * PIL_imagem.width

    media_canal_R = soma_pixels_canal_R // imagem_quant_pixels
    media_canal_G = soma_pixels_canal_G // imagem_quant_pixels
    media_canal_B = soma_pixels_canal_B // imagem_quant_pixels

    return [int(media_canal_R), int(media_canal_G), int(media_canal_B)]

def definir_catalogo():
    print("ora bolar, percebo que nao tienes uno cataloguito")
    print("pero joy possy ajhudalo, so me ajuder a te ajudar")

    while True:
        tamanho_raw = input("qual o tamanho das imagens[ex: 100l 100a]: ").strip()
        if not input("tem certeza?: Y/N") in ["N", "n", "pero que no"]:
            try:
                largura, altura = tamanho_raw.split(" ")
                largura, altura = int(largura), int(altura)
            except:
                print("escreveru merda ai >:(")
                continue
            break

    while True:
        cores = input('e qual o padrao de cores [ex: RGB ou PB]: ').strip()
        if input("tem certeza?: Y/N") not in ["N", "n", "pero que no"]:
            if cores not in ["RGB", 'PB', 'rgb', 'pb', 'r', 'p']:
                print("entrou uma cor errada ai otario")
                continue
            break

    catalogo = {'padrao_imagens' : [largura, altura, cores]}
    return catalogo

def add_img_catalogo(catalogo, nome_imagem, cores_imagem):
    catalogo.update({nome_imagem : cores_imagem})

#pega os parametros pela linha de comando
if(len(sys.argv) > 1):
    try:
        opts, args = getopt(sys.argv[1:],"p:o:")
    except GetoptError:
        print("algum argumento invalido")
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-p':
            caminho = arg
        elif opt == '-o':
            modo = arg

if modo == 'all':
    #adiciona todas as imagens na pasta para um catalogo
    #o catalogo pode ou não já existir
    try:
        os.chdir(caminho)
    except FileNotFoundError:
        raise Exception(f'O caminho "{caminho}" nao e valido')

    imagens = [f for f in os.listdir() if f[-4:] == ".png"]

    catalogo = carregar_catalogo()

    for nome_imagem in imagens:
        if nome_imagem not in catalogo:
            imagem = Image.open(nome_imagem)
            medias_imagem = cores_medias_imagemRGB(imagem)
            add_img_catalogo(catalogo, nome_imagem, medias_imagem)

    salvar_catalogo(catalogo)


elif modo == 'rem':
    #remove uma imagem do catalogo dependendo procurando pelo nome dela
    pass
elif modo == 'add':
    #adiciona uma ou mais imagens no catalogo
    pass
else:
    print("modo invalido")
        

