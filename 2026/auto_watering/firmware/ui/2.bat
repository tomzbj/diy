rem py genttf.py -f ui_main.cc c:\windows\fonts\SourceHanSerifSC-Bold.otf big.ttf
rem py genttf.py -f ui_main.cc "c:\windows\fonts\anton-regular.ttf" big.ttf
rem py bin2oct.py -c big.ttf font_big

rem py genttf.py -f ui_main.cc c:\windows\fonts\SourceHanSerifSC-Bold.otf big.ttf
type ui_main.cc ui_cfg.cc > tmp.txt
py genttf.py -f tmp.txt c:\windows\fonts\SarasaTermSC-Bold.ttf big.ttf
lv_font_conv --no-compress --bpp 4 --size 28 --format lvgl --font big.ttf -r 0x0-0x10ffff --lv-include "lvgl.h" -o font_big.c
