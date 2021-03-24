import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import array as arr

a = arr.array('d', [])
b = arr.array('d', [])
c = arr.array('d', [])

f=open("output/4_8_8_TREE.txt","r")

line=f.readlines()
for x in line:
    txt = x
    temp = txt.split(",")
    a.append(float(temp[0]))
    b.append(float(temp[1]))
    c.append(float(temp[2]))
    # print(x)

# x = np.linspace(0, 20, 100)

plt.plot(a,b)
plt.plot(a,c)
plt.show() 