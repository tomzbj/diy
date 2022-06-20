from math import sin
from math import pi

num_of_dots = 256
amplitude = 100
offset = 128
num_per_row = 12

f = open("1.txt", "w")
for i in range(num_of_dots):
    s = "0x%02x, " % (int(sin(i / num_of_dots * 2 * pi) * amplitude + offset))
#    s = "%d, " % (int(sin(i / num_of_dots * 2 * pi) * amplitude + offset))
    if i % num_per_row == 0:
        f.write("\n")
    f.write(s)
f.close()
