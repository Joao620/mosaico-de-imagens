from PIL import Image
import numpy as np
from time import time

joao = Image.open('foto1.jpg')

comeco = time()

resultado = list(joao.getdata())

final = time()

print(f"o resultado foi {final-comeco}")
