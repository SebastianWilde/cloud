#Separar un texto en palabras
import os
import os.path as path
filepath = os.getcwd()+'/Word_Count_input.txt'
outpath = 'data.txt'
prohibido = ['"',',','.',';',"'",'-','?','!']

out = open(outpath,"a")
with open(filepath) as fp:
    for line in fp:
        for char in prohibido:
            line = line.replace(char," ")
        palabras = line.split()
        #print(palabras)
        for palabra in palabras:
            out.write(palabra + '\n')
        #out.writelines(palabras)
        #wait = input("PRESS ENTER TO CONTINUE.")

out.close()

