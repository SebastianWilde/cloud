#Script crear data
import os
import os.path as path
import sys

gigaByte = (1024*1024*1024.0)
megaByte = (1024*1024.0)

opcion = raw_input('Opcion:')
cantidad = input('Cantidad')

name = 'merged-file.txt'

if opcion == "mb":
    name = str(cantidad)+'mb'+name
    cantidad = cantidad * megaByte
elif opcion == "gb":
    name = str(cantidad)+'gb'+name
    cantidad = cantidad * gigaByte
else:
    sys.exit()

file = os.getcwd()+'/'+name
f = open(file,'w+')
#aux = path.getsize(os.getcwd()+'/merged-file.txt')
aux = path.getsize(file)
print(aux)

while aux < cantidad:
    os.system("cat  data.txt >> " + name)
    #aux = path.getsize(os.getcwd()+'/merged-file.txt')
    aux = path.getsize(file)
    #a = input('stop if is necessary')
f.close()