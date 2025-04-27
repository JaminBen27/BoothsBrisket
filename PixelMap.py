from PIL import Image
from PIL import ImageColor


name = input('Enter File Name')
#name = 'graphic'
img = Image.open(name)
f = open(name + '.txt','w')

print(str(img.height))
print(str(img.width))

f.write(str(img.height))
f.write('\n')
f.write(str(img.width))
f.write('\n')

height = img.height
width = img.width
#pix = img.load()
rgb = img.convert('RGB')
img.close()

for r in range(0,height,1):
    for c in range(0,width,1):
        #print(rgb.getpixel((c,r)))
        R, G, B = rgb.getpixel((c, r))
        #print(str(R), str(G), str(B), ' ', end = '')
        line = str(R) + ' ' + str(G) + ' ' + str(B) + ' '
        #print(str(line), end = ' ')
        f.write(line)
    #print()
    f.write('\n')

f.close()
