type ui_main.cc ui_cfg.cc > tmp.txt
py genttf.py -f tmp.txt c:\windows\fonts\SarasaTermSC-Bold.ttf big.ttf
lv_font_conv --no-compress --bpp 4 --size 28 --format lvgl --font big.ttf -r 0x0-0x10ffff --lv-include "lvgl.h" -o font_big.c
