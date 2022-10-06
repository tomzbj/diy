from math import sin
from math import pi

num_of_dots = 330       ## 样本数
amplitude = 100         ## 振幅
offset = 2048           ## 偏置
num_per_row = 12        ## 每行样本数
fmt = int               ## 十进制/十六进制
output_file = "1.txt"   ## 输出文件名

f = open(output_file, "w") 
for i in range(num_of_dots):
    if fmt == hex:
        s = "0x%02x, " % (int(sin(i / num_of_dots * 2 * pi) * amplitude + offset))
    elif fmt == int:
        s = "%d, " % (int(sin(i / num_of_dots * 2 * pi) * amplitude + offset))
    else:
        exit
    if i % num_per_row == 0:
        f.write("\n")
    f.write(s)
f.close()
