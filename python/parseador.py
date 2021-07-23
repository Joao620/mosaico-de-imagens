import argparse 
import pathlib

def comand_line():
    path_imagem = ''
    path_textura = '../assets/texturaMinecraft/'
    path_saida = '.'
    path_dll = '../dll/agregador.dll'

    parser = argparse.ArgumentParser(epilog='all the path are relative to the python file')
    parser.add_argument('img', help='the image that will be converted', default=path_imagem, metavar='image path')
    parser.add_argument('-t', help='the texture that will compose the big picture', default=path_textura, metavar='texture path')
    parser.add_argument('-o', help='the output location of the image', default=path_saida, metavar='output path')
    parser.add_argument('-dll', help='the path to the dynamic library', default=path_dll, metavar='dll path')

    saida = parser.parse_args()

    path_imagem = saida.img
    path_textura = saida.t or path_textura
    path_saida = saida.o or path_saida
    path_dll = saida.dll or path_dll
    
    if not file_exist(path_imagem):
        raise RuntimeError(f'the image in path "{saida.img}" does not exist')

    if not dir_exist(path_textura):
        raise RuntimeError(f'the texture path "{saida.t}" does no exist')

    if not dir_exist(path_saida):
        raise RuntimeError(f'the output location "{saida.o}" does not exist ')

    if not file_exist(path_dll):
        raise RuntimeError(f'does not exist a dll in the location "{saida.dll}"')

    return {
            'path_imagem': path_imagem,
            'path_textura': path_textura,
            'path_saida': path_saida,
            'path_dll': path_dll
    }
    
def file_exist(path: str):
    try:
        pathobj = pathlib.Path(path)
    except OSError:
        return False

    pathobj = pathobj.resolve()
    return pathobj.is_file()

def dir_exist(path: str):
    try:
        pathobj = pathlib.Path(path)
    except OSError:
        return False
    pathobj = pathobj.resolve()
    return pathobj.is_dir()
