#include <string.h>
#include "cw.h"

static struct {
    char msg[128];
    int pos, sending;
    unsigned short curr;
    cw_cbs_t cbs;
} g;

static const unsigned short cw_table[64] = {
// from ascii 32 to ascii 95,  10 for dot, 11 for dash
    0x0000,     // 32 sp
    0xeef0,     // 33 !  -.-.--
    0xbae0,     // 34 "  .-..-.
    0xeaf0,     // 35 #  -...--
    0xabac,     // 36 $  ...-..-
    0xebb0,     // 37 %  -..-.-
    0xbaa8,     // 38 &  .-...
    0xbfe0,     // 39 '  .----.
    0xef80,     // 40 (  -.--.
    0xefb0,     // 41 )  -.--.-
    0xeee0,     // 42 *  -.-.-.
    0xbb80,     // 43 +  .-.-.
    0xfaf0,     // 44 ,  --..--
    0xeab0,     // 45 -  -....-
    0xbbb0,     // 46 .  .-.-.-
    0xeb80,     // 47 /  -..-.
    0xffc0,     // 48 0  -----
    0xbfc0,     // 49 1  .----
    0xafc0,     // 50 2  ..---
    0xabc0,     // 51 3  ...--
    0xaac0,     // 52 4  ....-
    0xaa80,     // 53 5  .....
    0xea80,     // 54 6  -....
    0xfa80,     // 55 7  --...
    0xfe80,     // 56 8  ---..
    0xff80,     // 57 9  ----.
    0xfea0,     // 58 :  ---...
    0xeee0,     // 59 ;  -.-.-.
    0xffa0,     // 60 <  ----..
    0xeac0,     // 61 =  -...-
    0xffe0,     // 62 >  -----.
    0xafa0,     // 63 ?  ..--..
    0xbee0,     // 64 @  .--.-.
    0xb000,     // 65 A  .-
    0xea00,     // 66 B  -...
    0xee00,     // 67 C  -.-.
    0xe800,     // 68 D  -..
    0x8000,     // 69 E  .
    0xae00,     // 70 F  ..-.
    0xf800,     // 71 G  --.
    0xaa00,     // 72 H  ....
    0xa000,     // 73 I  ..
    0xbf00,     // 74 J  .---
    0xec00,     // 75 K  -.-
    0xba00,     // 76 L  .-..
    0xf000,     // 77 M  --
    0xe000,     // 78 N  -.
    0xfc00,     // 79 O  ---
    0xbe00,     // 80 P  .--.
    0xfb00,     // 81 Q  --.-
    0xb800,     // 82 R  .-.
    0xa800,     // 83 S  ...
    0xc000,     // 84 T  -
    0xac00,     // 85 U  ..-
    0xab00,     // 86 V  ...-
    0xbc00,     // 87 W  .--
    0xeb00,     // 88 X  -..-
    0xef00,     // 89 Y  -.--
    0xfa00,     // 90 Z  --..
    0xefbc,     // 91 [  -.--.--
    0xefe8,     // 92 \  -.---..
    0xefec,     // 93 ]  -.---.-
    0xeff8,     // 94 ^  -.----.
    0xafb0      // 95 _  ..--.-
    };

void cw_init(cw_cbs_t* cbs)
{
    g.cbs = *cbs;
}

void cw_tx(const char* msg, int len)
{
    memset(g.msg, '\0', sizeof(g.msg));
    strncpy(g.msg, msg, len);
    for(int i = 0; i < len; i++) {
        if(g.msg[i] >= 'a' && g.msg[i] <= 'z')
            g.msg[i] = g.msg[i] - 'a' + 'A';
    }
    g.pos = 0;
    g.sending = 1;
    g.curr = cw_table[msg[0] - 32];
    g.cbs.tx_start_f();
}

void cw_tx_repeat(void)    // for beacon mode
{
    g.pos = 0;
    g.sending = 1;
    g.curr = cw_table[g.msg[0] - 32];
}

void cw_poll(void)  // will be called every 1ms
{
    static int count0 = 0, count1 = 0;
    if(g.sending) {
        if(count1) {
            g.cbs.tx_symbol_f(1);
            count1--;
            return;
        }
        if(count0) {
            g.cbs.tx_symbol_f(0);
            count0--;
            return;
        }
        if(g.curr == 0) {
            g.cbs.tx_symbol_f(0);
            g.pos++;
            if(g.msg[g.pos]) {
                g.cbs.tx_char_f(g.msg[g.pos]);
                g.curr = cw_table[g.msg[g.pos] - 32];
                count0 = 1;    // inteval between characters
                if(g.msg[g.pos] == ' ')
                    count0 = 3;    // inteval between words
            }
            else {      // transmission ends
                g.sending = 0;
                count0 = 0;
                count1 = 0;
                g.cbs.tx_end_f();
            }
        }
        else {
            if((g.curr & 0xc000) == 0xc000) {    // dash
                g.cbs.tx_symbol_f(1);
                count1 = 2;
            }
            else if((g.curr & 0xc000) == 0x8000) {    // dot
                g.cbs.tx_symbol_f(1);
                count1 = 0;
            }
            count0 = 1;
            g.curr <<= 2;
        }
    }
    else {
    }
}
