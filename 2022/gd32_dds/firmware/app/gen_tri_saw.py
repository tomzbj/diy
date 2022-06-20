from math import sin
from math import pi

num_of_dots = 256
num_per_row = 12

for i in range(num_of_dots):
    if i < 128: j=i*2
    else: j=255-(i-128)*2
    s = "0x%02x, " % j
    if i % num_per_row == 0:
        print()
    print(s, end="")
print()
