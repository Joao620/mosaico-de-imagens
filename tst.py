def teste(lista, x, y, z, sx, sz):
    demonio = (y*(sz*sx)) + x*sz+z
    return lista[demonio]


lista = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18]

print(teste(lista, 0, 0, 0, 2, 3))

