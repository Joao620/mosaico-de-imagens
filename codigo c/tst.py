from random import randint, sample

textoMeste = ""

xx = 8
yy = 8
zz = 3


lista_ale = sample(range(0, 255), xx*yy*zz)


iterlista = iter(lista_ale)

for y in range(yy):
    textoMeste += '{'
    for x in range(xx):
        textoMeste += '{'
        for z in range(zz):
            textoMeste += f"{next(iterlista)}, "
        textoMeste = textoMeste[:-2] + "}, "
    textoMeste = textoMeste[:-2] + '},\n'

textoMeste = textoMeste[:-2]

print(textoMeste)
print("#" * 10)
print("\n\n")

textoMeste = ""
contador = 0

textoMeste += "{"
for i in range(xx*yy*zz):
    textoMeste += f"{lista_ale[i]}, "
    if(len(textoMeste) - contador >= 80):
        textoMeste += "\n"
        contador += 80

textoMeste = textoMeste[:-2] + "}" 

print(textoMeste)