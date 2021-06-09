from math import sin
from math import pi

num_of_dots = 240
amplitude = 1300
offset = 2048
num_per_row = 12

f = open("1.txt", "w")
for i in range(num_of_dots):
    s = "%d, " % (int(sin(i / num_of_dots * 2 * pi) * amplitude + offset))
    if i % num_per_row == 0:
        f.write("\n")
    f.write(s)
f.close()
