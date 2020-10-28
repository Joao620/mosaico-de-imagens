from ctypes import *

class nomenadahaver(Structure):
    _fields_ = [
        ('teste', POINTER(c_int))
    ]

nome = '/home/joao/Documents/Programas/mosaico-de-imagens/testes/teste.dll'
dll = CDLL(nome)

listaTeste = ((c_int * 3) * 5)()

for i in range(5):
    for j in range(3):
        listaTeste[i][j] = i * 10 + j

for x in listaTeste:
    for y in x:
        print(y)

#strutura = nomenadahaver(byref(listaTeste))


resultado = dll.funcaoTeste2(listaTeste)

