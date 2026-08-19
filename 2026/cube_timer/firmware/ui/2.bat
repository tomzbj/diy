rem py genttf.py -f ui_main.cc c:\windows\fonts\SourceHanSerifSC-Bold.otf big.ttf
rem py genttf.py -f ui_main.cc "c:\windows\fonts\anton-regular.ttf" big.ttf
rem py bin2oct.py -c big.ttf font_big

rem py genttf.py -f ui_main.cc c:\windows\fonts\SourceHanSerifSC-Bold.otf big.ttf
rem type ui_main.cc ui_cfg.cc > tmp.txt
rem py genttf.py -f tmp.txt c:\windows\fonts\SarasaTermSC-Bold.ttf big.ttf
lv_font_conv --no-compress --bpp 1 --size 100 --format lvgl --font c:\windows\fonts\Anton-Regular.ttf -r 0x0-0xff --lv-include "lvgl.h" -o anton.c
