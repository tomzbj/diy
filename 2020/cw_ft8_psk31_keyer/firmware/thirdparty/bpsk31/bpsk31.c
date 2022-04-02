// bpsk31.c BPSK31 transmitter by BG2BHC
#include "misc.h"

#include "bpsk31.h"
#include <stdint.h>

const int16_t sinetable[] = {
    //
    0X0, 0XB8, 0X170, 0X228, 0X2E0, 0X398, 0X450, 0X508, 0X5C0, 0X677, 0X72F,
    0X7E7, 0X89E, 0X956, 0XA0D, 0XAC5, 0XB7C, 0XC33, 0XCEA, 0XDA1, 0XE58, 0XF0E,
    0XFC5, 0X107B, 0X1131, 0X11E7, 0X129D, 0X1353, 0X1408, 0X14BE, 0X1573,
    0X1628, 0X16DC, 0X1791, 0X1845, 0X18F9, 0X19AD, 0X1A60, 0X1B13, 0X1BC6,
    0X1C79, 0X1D2B, 0X1DDD, 0X1E8F, 0X1F41, 0X1FF2, 0X20A3, 0X2154, 0X2204,
    0X22B4, 0X2364, 0X2413, 0X24C2, 0X2571, 0X261F, 0X26CD, 0X277A, 0X2827,
    0X28D4, 0X2980, 0X2A2C, 0X2AD8, 0X2B83, 0X2C2E, 0X2CD8, 0X2D82, 0X2E2B,
    0X2ED4, 0X2F7D, 0X3025, 0X30CC, 0X3174, 0X321A, 0X32C0, 0X3366, 0X340B,
    0X34B0, 0X3554, 0X35F8, 0X369B, 0X373D, 0X37DF, 0X3881, 0X3922, 0X39C2,
    0X3A62, 0X3B02, 0X3BA0, 0X3C3F, 0X3CDC, 0X3D79, 0X3E16, 0X3EB1, 0X3F4D,
    0X3FE7, 0X4081, 0X411B, 0X41B3, 0X424B, 0X42E3, 0X437A, 0X4410, 0X44A5,
    0X453A, 0X45CE, 0X4662, 0X46F5, 0X4787, 0X4818, 0X48A9, 0X4939, 0X49C9,
    0X4A57, 0X4AE5, 0X4B72, 0X4BFF, 0X4C8B, 0X4D16, 0X4DA0, 0X4E29, 0X4EB2,
    0X4F3A, 0X4FC2, 0X5048, 0X50CE, 0X5153, 0X51D7, 0X525A, 0X52DD, 0X535E,
    0X53DF, 0X5460, 0X54DF, 0X555D, 0X55DB, 0X5658, 0X56D4, 0X574F, 0X57CA,
    0X5843, 0X58BC, 0X5934, 0X59AB, 0X5A21, 0X5A96, 0X5B0A, 0X5B7E, 0X5BF0,
    0X5C62, 0X5CD3, 0X5D43, 0X5DB2, 0X5E20, 0X5E8D, 0X5EF9, 0X5F65, 0X5FCF,
    0X6039, 0X60A1, 0X6109, 0X6170, 0X61D5, 0X623A, 0X629E, 0X6301, 0X6363,
    0X63C4, 0X6424, 0X6483, 0X64E2, 0X653F, 0X659B, 0X65F6, 0X6650, 0X66AA,
    0X6702, 0X6759, 0X67AF, 0X6805, 0X6859, 0X68AC, 0X68FE, 0X6950, 0X69A0,
    0X69EF, 0X6A3D, 0X6A8B, 0X6AD7, 0X6B22, 0X6B6C, 0X6BB5, 0X6BFD, 0X6C44,
    0X6C8A, 0X6CCF, 0X6D13, 0X6D55, 0X6D97, 0X6DD8, 0X6E17, 0X6E56, 0X6E93,
    0X6ED0, 0X6F0B, 0X6F45, 0X6F7F, 0X6FB7, 0X6FEE, 0X7024, 0X7059, 0X708C,
    0X70BF, 0X70F1, 0X7121, 0X7151, 0X717F, 0X71AC, 0X71D9, 0X7204, 0X722E,
    0X7257, 0X727E, 0X72A5, 0X72CB, 0X72EF, 0X7312, 0X7335, 0X7356, 0X7376,
    0X7395, 0X73B3, 0X73CF, 0X73EB, 0X7405, 0X741F, 0X7437, 0X744E, 0X7464,
    0X7479, 0X748C, 0X749F, 0X74B1, 0X74C1, 0X74D0, 0X74DE, 0X74EB, 0X74F7,
    0X7502, 0X750B, 0X7514, 0X751B, 0X7521, 0X7526, 0X752A, 0X752D, 0X752F,
    0X7530, 0X752F, 0X752D, 0X752A, 0X7526, 0X7521, 0X751B, 0X7514, 0X750B,
    0X7502, 0X74F7, 0X74EB, 0X74DE, 0X74D0, 0X74C1, 0X74B1, 0X749F, 0X748C,
    0X7479, 0X7464, 0X744E, 0X7437, 0X741F, 0X7405, 0X73EB, 0X73CF, 0X73B3,
    0X7395, 0X7376, 0X7356, 0X7335, 0X7312, 0X72EF, 0X72CB, 0X72A5, 0X727E,
    0X7257, 0X722E, 0X7204, 0X71D9, 0X71AC, 0X717F, 0X7151, 0X7121, 0X70F1,
    0X70BF, 0X708C, 0X7059, 0X7024, 0X6FEE, 0X6FB7, 0X6F7F, 0X6F45, 0X6F0B,
    0X6ED0, 0X6E93, 0X6E56, 0X6E17, 0X6DD8, 0X6D97, 0X6D55, 0X6D13, 0X6CCF,
    0X6C8A, 0X6C44, 0X6BFD, 0X6BB5, 0X6B6C, 0X6B22, 0X6AD7, 0X6A8B, 0X6A3D,
    0X69EF, 0X69A0, 0X6950, 0X68FE, 0X68AC, 0X6859, 0X6805, 0X67AF, 0X6759,
    0X6702, 0X66AA, 0X6650, 0X65F6, 0X659B, 0X653F, 0X64E2, 0X6483, 0X6424,
    0X63C4, 0X6363, 0X6301, 0X629E, 0X623A, 0X61D5, 0X6170, 0X6109, 0X60A1,
    0X6039, 0X5FCF, 0X5F65, 0X5EF9, 0X5E8D, 0X5E20, 0X5DB2, 0X5D43, 0X5CD3,
    0X5C62, 0X5BF0, 0X5B7E, 0X5B0A, 0X5A96, 0X5A21, 0X59AB, 0X5934, 0X58BC,
    0X5843, 0X57CA, 0X574F, 0X56D4, 0X5658, 0X55DB, 0X555D, 0X54DF, 0X5460,
    0X53DF, 0X535E, 0X52DD, 0X525A, 0X51D7, 0X5153, 0X50CE, 0X5048, 0X4FC2,
    0X4F3A, 0X4EB2, 0X4E29, 0X4DA0, 0X4D16, 0X4C8B, 0X4BFF, 0X4B72, 0X4AE5,
    0X4A57, 0X49C9, 0X4939, 0X48A9, 0X4818, 0X4787, 0X46F5, 0X4662, 0X45CE,
    0X453A, 0X44A5, 0X4410, 0X437A, 0X42E3, 0X424B, 0X41B3, 0X411B, 0X4081,
    0X3FE7, 0X3F4D, 0X3EB1, 0X3E16, 0X3D79, 0X3CDC, 0X3C3F, 0X3BA0, 0X3B02,
    0X3A62, 0X39C2, 0X3922, 0X3881, 0X37DF, 0X373D, 0X369B, 0X35F8, 0X3554,
    0X34B0, 0X340B, 0X3366, 0X32C0, 0X321A, 0X3174, 0X30CC, 0X3025, 0X2F7D,
    0X2ED4, 0X2E2B, 0X2D82, 0X2CD8, 0X2C2E, 0X2B83, 0X2AD8, 0X2A2C, 0X2980,
    0X28D4, 0X2827, 0X277A, 0X26CD, 0X261F, 0X2571, 0X24C2, 0X2413, 0X2364,
    0X22B4, 0X2204, 0X2154, 0X20A3, 0X1FF2, 0X1F41, 0X1E8F, 0X1DDD, 0X1D2B,
    0X1C79, 0X1BC6, 0X1B13, 0X1A60, 0X19AD, 0X18F9, 0X1845, 0X1791, 0X16DC,
    0X1628, 0X1573, 0X14BE, 0X1408, 0X1353, 0X129D, 0X11E7, 0X1131, 0X107B,
    0XFC5, 0XF0E, 0XE58, 0XDA1, 0XCEA, 0XC33, 0XB7C, 0XAC5, 0XA0D, 0X956, 0X89E,
    0X7E7, 0X72F, 0X677, 0X5C0, 0X508, 0X450, 0X398, 0X2E0, 0X228, 0X170, 0XB8,
    0X0, 0XFF48, 0XFE90, 0XFDD8, 0XFD20, 0XFC68, 0XFBB0, 0XFAF8, 0XFA40, 0XF989,
    0XF8D1, 0XF819, 0XF762, 0XF6AA, 0XF5F3, 0XF53B, 0XF484, 0XF3CD, 0XF316,
    0XF25F, 0XF1A8, 0XF0F2, 0XF03B, 0XEF85, 0XEECF, 0XEE19, 0XED63, 0XECAD,
    0XEBF8, 0XEB42, 0XEA8D, 0XE9D8, 0XE924, 0XE86F, 0XE7BB, 0XE707, 0XE653,
    0XE5A0, 0XE4ED, 0XE43A, 0XE387, 0XE2D5, 0XE223, 0XE171, 0XE0BF, 0XE00E,
    0XDF5D, 0XDEAC, 0XDDFC, 0XDD4C, 0XDC9C, 0XDBED, 0XDB3E, 0XDA8F, 0XD9E1,
    0XD933, 0XD886, 0XD7D9, 0XD72C, 0XD680, 0XD5D4, 0XD528, 0XD47D, 0XD3D2,
    0XD328, 0XD27E, 0XD1D5, 0XD12C, 0XD083, 0XCFDB, 0XCF34, 0XCE8C, 0XCDE6,
    0XCD40, 0XCC9A, 0XCBF5, 0XCB50, 0XCAAC, 0XCA08, 0XC965, 0XC8C3, 0XC821,
    0XC77F, 0XC6DE, 0XC63E, 0XC59E, 0XC4FE, 0XC460, 0XC3C1, 0XC324, 0XC287,
    0XC1EA, 0XC14F, 0XC0B3, 0XC019, 0XBF7F, 0XBEE5, 0XBE4D, 0XBDB5, 0XBD1D,
    0XBC86, 0XBBF0, 0XBB5B, 0XBAC6, 0XBA32, 0XB99E, 0XB90B, 0XB879, 0XB7E8,
    0XB757, 0XB6C7, 0XB637, 0XB5A9, 0XB51B, 0XB48E, 0XB401, 0XB375, 0XB2EA,
    0XB260, 0XB1D7, 0XB14E, 0XB0C6, 0XB03E, 0XAFB8, 0XAF32, 0XAEAD, 0XAE29,
    0XADA6, 0XAD23, 0XACA2, 0XAC21, 0XABA0, 0XAB21, 0XAAA3, 0XAA25, 0XA9A8,
    0XA92C, 0XA8B1, 0XA836, 0XA7BD, 0XA744, 0XA6CC, 0XA655, 0XA5DF, 0XA56A,
    0XA4F6, 0XA482, 0XA410, 0XA39E, 0XA32D, 0XA2BD, 0XA24E, 0XA1E0, 0XA173,
    0XA107, 0XA09B, 0XA031, 0X9FC7, 0X9F5F, 0X9EF7, 0X9E90, 0X9E2B, 0X9DC6,
    0X9D62, 0X9CFF, 0X9C9D, 0X9C3C, 0X9BDC, 0X9B7D, 0X9B1E, 0X9AC1, 0X9A65,
    0X9A0A, 0X99B0, 0X9956, 0X98FE, 0X98A7, 0X9851, 0X97FB, 0X97A7, 0X9754,
    0X9702, 0X96B0, 0X9660, 0X9611, 0X95C3, 0X9575, 0X9529, 0X94DE, 0X9494,
    0X944B, 0X9403, 0X93BC, 0X9376, 0X9331, 0X92ED, 0X92AB, 0X9269, 0X9228,
    0X91E9, 0X91AA, 0X916D, 0X9130, 0X90F5, 0X90BB, 0X9081, 0X9049, 0X9012,
    0X8FDC, 0X8FA7, 0X8F74, 0X8F41, 0X8F0F, 0X8EDF, 0X8EAF, 0X8E81, 0X8E54,
    0X8E27, 0X8DFC, 0X8DD2, 0X8DA9, 0X8D82, 0X8D5B, 0X8D35, 0X8D11, 0X8CEE,
    0X8CCB, 0X8CAA, 0X8C8A, 0X8C6B, 0X8C4D, 0X8C31, 0X8C15, 0X8BFB, 0X8BE1,
    0X8BC9, 0X8BB2, 0X8B9C, 0X8B87, 0X8B74, 0X8B61, 0X8B4F, 0X8B3F, 0X8B30,
    0X8B22, 0X8B15, 0X8B09, 0X8AFE, 0X8AF5, 0X8AEC, 0X8AE5, 0X8ADF, 0X8ADA,
    0X8AD6, 0X8AD3, 0X8AD1, 0X8AD0, 0X8AD1, 0X8AD3, 0X8AD6, 0X8ADA, 0X8ADF,
    0X8AE5, 0X8AEC, 0X8AF5, 0X8AFE, 0X8B09, 0X8B15, 0X8B22, 0X8B30, 0X8B3F,
    0X8B4F, 0X8B61, 0X8B74, 0X8B87, 0X8B9C, 0X8BB2, 0X8BC9, 0X8BE1, 0X8BFB,
    0X8C15, 0X8C31, 0X8C4D, 0X8C6B, 0X8C8A, 0X8CAA, 0X8CCB, 0X8CEE, 0X8D11,
    0X8D35, 0X8D5B, 0X8D82, 0X8DA9, 0X8DD2, 0X8DFC, 0X8E27, 0X8E54, 0X8E81,
    0X8EAF, 0X8EDF, 0X8F0F, 0X8F41, 0X8F74, 0X8FA7, 0X8FDC, 0X9012, 0X9049,
    0X9081, 0X90BB, 0X90F5, 0X9130, 0X916D, 0X91AA, 0X91E9, 0X9228, 0X9269,
    0X92AB, 0X92ED, 0X9331, 0X9376, 0X93BC, 0X9403, 0X944B, 0X9494, 0X94DE,
    0X9529, 0X9575, 0X95C3, 0X9611, 0X9660, 0X96B0, 0X9702, 0X9754, 0X97A7,
    0X97FB, 0X9851, 0X98A7, 0X98FE, 0X9956, 0X99B0, 0X9A0A, 0X9A65, 0X9AC1,
    0X9B1E, 0X9B7D, 0X9BDC, 0X9C3C, 0X9C9D, 0X9CFF, 0X9D62, 0X9DC6, 0X9E2B,
    0X9E90, 0X9EF7, 0X9F5F, 0X9FC7, 0XA031, 0XA09B, 0XA107, 0XA173, 0XA1E0,
    0XA24E, 0XA2BD, 0XA32D, 0XA39E, 0XA410, 0XA482, 0XA4F6, 0XA56A, 0XA5DF,
    0XA655, 0XA6CC, 0XA744, 0XA7BD, 0XA836, 0XA8B1, 0XA92C, 0XA9A8, 0XAA25,
    0XAAA3, 0XAB21, 0XABA0, 0XAC21, 0XACA2, 0XAD23, 0XADA6, 0XAE29, 0XAEAD,
    0XAF32, 0XAFB8, 0XB03E, 0XB0C6, 0XB14E, 0XB1D7, 0XB260, 0XB2EA, 0XB375,
    0XB401, 0XB48E, 0XB51B, 0XB5A9, 0XB637, 0XB6C7, 0XB757, 0XB7E8, 0XB879,
    0XB90B, 0XB99E, 0XBA32, 0XBAC6, 0XBB5B, 0XBBF0, 0XBC86, 0XBD1D, 0XBDB5,
    0XBE4D, 0XBEE5, 0XBF7F, 0XC019, 0XC0B3, 0XC14F, 0XC1EA, 0XC287, 0XC324,
    0XC3C1, 0XC460, 0XC4FE, 0XC59E, 0XC63E, 0XC6DE, 0XC77F, 0XC821, 0XC8C3,
    0XC965, 0XCA08, 0XCAAC, 0XCB50, 0XCBF5, 0XCC9A, 0XCD40, 0XCDE6, 0XCE8C,
    0XCF34, 0XCFDB, 0XD083, 0XD12C, 0XD1D5, 0XD27E, 0XD328, 0XD3D2, 0XD47D,
    0XD528, 0XD5D4, 0XD680, 0XD72C, 0XD7D9, 0XD886, 0XD933, 0XD9E1, 0XDA8F,
    0XDB3E, 0XDBED, 0XDC9C, 0XDD4C, 0XDDFC, 0XDEAC, 0XDF5D, 0XE00E, 0XE0BF,
    0XE171, 0XE223, 0XE2D5, 0XE387, 0XE43A, 0XE4ED, 0XE5A0, 0XE653, 0XE707,
    0XE7BB, 0XE86F, 0XE924, 0XE9D8, 0XEA8D, 0XEB42, 0XEBF8, 0XECAD, 0XED63,
    0XEE19, 0XEECF, 0XEF85, 0XF03B, 0XF0F2, 0XF1A8, 0XF25F, 0XF316, 0XF3CD,
    0XF484, 0XF53B, 0XF5F3, 0XF6AA, 0XF762, 0XF819, 0XF8D1, 0XF989, 0XFA40,
    0XFAF8, 0XFBB0, 0XFC68, 0XFD20, 0XFDD8, 0XFE90, 0XFF48    //
    };

const uint16_t VARICODE_TABLE[256] = {
//
    0xAAC0, /* ASCII =   0	1010101011                    */
    0xB6C0, /* ASCII =   1	1011011011                    */
    0xBB40, /* ASCII =   2	1011101101                    */
    0xDDC0, /* ASCII =   3	1101110111                    */
    0xBAC0, /* ASCII =   4	1011101011                    */
    0xD7C0, /* ASCII =   5	1101011111                    */
    0xBBC0, /* ASCII =   6	1011101111                    */
    0xBF40, /* ASCII =   7	1011111101                    */
    0xBFC0, /* ASCII =   8	1011111111                    */
    0xEF00, /* ASCII =   9	11101111                      */
    0xE800, /* ASCII =  10	11101                         */
    0xDBC0, /* ASCII =  11	1101101111                    */
    0xB740, /* ASCII =  12	1011011101                    */
    0xF800, /* ASCII =  13	11111                         */
    0xDD40, /* ASCII =  14	1101110101                    */
    0xEAC0, /* ASCII =  15	1110101011                    */
    0xBDC0, /* ASCII =  16	1011110111                    */
    0xBD40, /* ASCII =  17	1011110101                    */
    0xEB40, /* ASCII =  18	1110101101                    */
    0xEBC0, /* ASCII =  19	1110101111                    */
    0xD6C0, /* ASCII =  20	1101011011                    */
    0xDAC0, /* ASCII =  21	1101101011                    */
    0xDB40, /* ASCII =  22	1101101101                    */
    0xD5C0, /* ASCII =  23	1101010111                    */
    0xDEC0, /* ASCII =  24	1101111011                    */
    0xDF40, /* ASCII =  25	1101111101                    */
    0xEDC0, /* ASCII =  26	1110110111                    */
    0xD540, /* ASCII =  27	1101010101                    */
    0xD740, /* ASCII =  28	1101011101                    */
    0xEEC0, /* ASCII =  29	1110111011                    */
    0xBEC0, /* ASCII =  30	1011111011                    */
    0xDFC0, /* ASCII =  31	1101111111                    */
    0x8000, /* ASCII = ' '	1                             */
    0xFF80, /* ASCII = '!'	111111111                     */
    0xAF80, /* ASCII = '"'	101011111                     */
    0xFA80, /* ASCII = '#'	111110101                     */
    0xED80, /* ASCII = '$'	111011011                     */
    0xB540, /* ASCII = '%'	1011010101                    */
    0xAEC0, /* ASCII = '&'	1010111011                    */
    0xBF80, /* ASCII = '''	101111111                     */
    0xFB00, /* ASCII = '('	11111011                      */
    0xF700, /* ASCII = ')'	11110111                      */
    0xB780, /* ASCII = '*'	101101111                     */
    0xEF80, /* ASCII = '+'	111011111                     */
    0xEA00, /* ASCII = ','	1110101                       */
    0xD400, /* ASCII = '-'	110101                        */
    0xAE00, /* ASCII = '.'	1010111                       */
    0xD780, /* ASCII = '/'	110101111                     */
    0xB700, /* ASCII = '0'	10110111                      */
    0xBD00, /* ASCII = '1'	10111101                      */
    0xED00, /* ASCII = '2'	11101101                      */
    0xFF00, /* ASCII = '3'	11111111                      */
    0xBB80, /* ASCII = '4'	101110111                     */
    0xAD80, /* ASCII = '5'	101011011                     */
    0xB580, /* ASCII = '6'	101101011                     */
    0xD680, /* ASCII = '7'	110101101                     */
    0xD580, /* ASCII = '8'	110101011                     */
    0xDB80, /* ASCII = '9'	110110111                     */
    0xF500, /* ASCII = ':'	11110101                      */
    0xDE80, /* ASCII = ';'	110111101                     */
    0xF680, /* ASCII = '<'	111101101                     */
    0xAA00, /* ASCII = '='	1010101                       */
    0xEB80, /* ASCII = '>'	111010111                     */
    0xABC0, /* ASCII = '?'	1010101111                    */
    0xAF40, /* ASCII = '@'	1010111101                    */
    0xFA00, /* ASCII = 'A'	1111101                       */
    0xEB00, /* ASCII = 'B'	11101011                      */
    0xAD00, /* ASCII = 'C'	10101101                      */
    0xB500, /* ASCII = 'D'	10110101                      */
    0xEE00, /* ASCII = 'E'	1110111                       */
    0xDB00, /* ASCII = 'F'	11011011                      */
    0xFD00, /* ASCII = 'G'	11111101                      */
    0xAA80, /* ASCII = 'H'	101010101                     */
    0xFE00, /* ASCII = 'I'	1111111                       */
    0xFE80, /* ASCII = 'J'	111111101                     */
    0xBE80, /* ASCII = 'K'	101111101                     */
    0xD700, /* ASCII = 'L'	11010111                      */
    0xBB00, /* ASCII = 'M'	10111011                      */
    0xDD00, /* ASCII = 'N'	11011101                      */
    0xAB00, /* ASCII = 'O'	10101011                      */
    0xD500, /* ASCII = 'P'	11010101                      */
    0xEE80, /* ASCII = 'Q'	111011101                     */
    0xAF00, /* ASCII = 'R'	10101111                      */
    0xDE00, /* ASCII = 'S'	1101111                       */
    0xDA00, /* ASCII = 'T'	1101101                       */
    0xAB80, /* ASCII = 'U'	101010111                     */
    0xDA80, /* ASCII = 'V'	110110101                     */
    0xAE80, /* ASCII = 'W'	101011101                     */
    0xBA80, /* ASCII = 'X'	101110101                     */
    0xBD80, /* ASCII = 'Y'	101111011                     */
    0xAB40, /* ASCII = 'Z'	1010101101                    */
    0xFB80, /* ASCII = '['	111110111                     */
    0xF780, /* ASCII = '\'	111101111                     */
    0xFD80, /* ASCII = ']'	111111011                     */
    0xAFC0, /* ASCII = '^'	1010111111                    */
    0xB680, /* ASCII = '_'	101101101                     */
    0xB7C0, /* ASCII = '`'	1011011111          AD7JT     */
    0xB000, /* ASCII = 'a'	1011                          */
    0xBE00, /* ASCII = 'b'	1011111                       */
    0xBC00, /* ASCII = 'c'	101111                        */
    0xB400, /* ASCII = 'd'	101101                        */
    0xC000, /* ASCII = 'e'	11                            */
    0xF400, /* ASCII = 'f'	111101                        */
    0xB600, /* ASCII = 'g'	1011011                       */
    0xAC00, /* ASCII = 'h'	101011                        */
    0xD000, /* ASCII = 'i'	1101                          */
    0xF580, /* ASCII = 'j'	111101011                     */
    0xBF00, /* ASCII = 'k'	10111111                      */
    0xD800, /* ASCII = 'l'	11011                         */
    0xEC00, /* ASCII = 'm'	111011                        */
    0xF000, /* ASCII = 'n'	1111                          */
    0xE000, /* ASCII = 'o'	111                           */
    0xFC00, /* ASCII = 'p'	111111                        */
    0xDF80, /* ASCII = 'q'	110111111                     */
    0xA800, /* ASCII = 'r'	10101                         */
    0xB800, /* ASCII = 's'	10111                         */
    0xA000, /* ASCII = 't'	101                           */
    0xDC00, /* ASCII = 'u'	110111                        */
    0xF600, /* ASCII = 'v'	1111011                       */
    0xD600, /* ASCII = 'w'	1101011                       */
    0xDF00, /* ASCII = 'x'	11011111                      */
    0xBA00, /* ASCII = 'y'	1011101                       */
    0xEA80, /* ASCII = 'z'	111010101                     */
    0xADC0, /* ASCII = '{'	1010110111                    */
    0xDD80, /* ASCII = '|'	110111011                     */
    0xAD40, /* ASCII = '}'	1010110101                    */
    0xB5C0, /* ASCII = '~'	1011010111                    */
    0xED40, /* ASCII = 127	1110110101                    */
    0xEF40, /* ASCII = 128	1110111101                    */
    0xEFC0, /* ASCII = 129	1110111111                    */
    0xF540, /* ASCII = 130	1111010101                    */
    0xF5C0, /* ASCII = 131	1111010111                    */
    0xF6C0, /* ASCII = 132	1111011011                    */
    0xF740, /* ASCII = 133	1111011101                    */
    0xF7C0, /* ASCII = 134	1111011111                    */
    0xFAC0, /* ASCII = 135	1111101011                    */
    0xFB40, /* ASCII = 136	1111101101                    */
    0xFBC0, /* ASCII = 137	1111101111                    */
    0xFD40, /* ASCII = 138	1111110101                    */
    0xFDC0, /* ASCII = 139	1111110111                    */
    0xFEC0, /* ASCII = 140	1111111011                    */
    0xFF40, /* ASCII = 141	1111111101                    */
    0xFFC0, /* ASCII = 142	1111111111                    */
    0xAAA0, /* ASCII = 143	10101010101                   */
    0xAAE0, /* ASCII = 144	10101010111                   */
    0xAB60, /* ASCII = 145	10101011011                   */
    0xABA0, /* ASCII = 146	10101011101                   */
    0xABE0, /* ASCII = 147	10101011111                   */
    0xAD60, /* ASCII = 148	10101101011                   */
    0xADA0, /* ASCII = 149	10101101101                   */
    0xADE0, /* ASCII = 150	10101101111                   */
    0xAEA0, /* ASCII = 151	10101110101                   */
    0xAEE0, /* ASCII = 152	10101110111                   */
    0xAF60, /* ASCII = 153	10101111011                   */
    0xAFA0, /* ASCII = 154	10101111101                   */
    0xAFE0, /* ASCII = 155	10101111111                   */
    0xB560, /* ASCII = 156	10110101011                   */
    0xB5A0, /* ASCII = 157	10110101101                   */
    0xB5E0, /* ASCII = 158	10110101111                   */
    0xB6A0, /* ASCII = 159	10110110101                   */
    0xB6E0, /* ASCII = 160	10110110111                   */
    0xB760, /* ASCII = 161	10110111011                   */
    0xB7A0, /* ASCII = 162	10110111101                   */
    0xB7E0, /* ASCII = 163	10110111111                   */
    0xBAA0, /* ASCII = 164	10111010101                   */
    0xBAE0, /* ASCII = 165	10111010111                   */
    0xBB60, /* ASCII = 166	10111011011                   */
    0xBBA0, /* ASCII = 167	10111011101                   */
    0xBBE0, /* ASCII = 168	10111011111                   */
    0xBD60, /* ASCII = 169	10111101011                   */
    0xBDA0, /* ASCII = 170	10111101101                   */
    0xBDE0, /* ASCII = 171	10111101111                   */
    0xBEA0, /* ASCII = 172	10111110101                   */
    0xBEE0, /* ASCII = 173	10111110111                   */
    0xBF60, /* ASCII = 174	10111111011                   */
    0xBFA0, /* ASCII = 175	10111111101                   */
    0xBFE0, /* ASCII = 176	10111111111                   */
    0xD560, /* ASCII = 177	11010101011                   */
    0xD5A0, /* ASCII = 178	11010101101                   */
    0xD5E0, /* ASCII = 179	11010101111                   */
    0xD6A0, /* ASCII = 180	11010110101                   */
    0xD6E0, /* ASCII = 181	11010110111                   */
    0xD760, /* ASCII = 182	11010111011                   */
    0xD7A0, /* ASCII = 183	11010111101                   */
    0xD7E0, /* ASCII = 184	11010111111                   */
    0xDAA0, /* ASCII = 185	11011010101                   */
    0xDAE0, /* ASCII = 186	11011010111                   */
    0xDB60, /* ASCII = 187	11011011011                   */
    0xDBA0, /* ASCII = 188	11011011101                   */
    0xDBE0, /* ASCII = 189	11011011111                   */
    0xDD60, /* ASCII = 190	11011101011                   */
    0xDDA0, /* ASCII = 191	11011101101                   */
    0xDDE0, /* ASCII = 192	11011101111                   */
    0xDEA0, /* ASCII = 193	11011110101                   */
    0xDEE0, /* ASCII = 194	11011110111                   */
    0xDF60, /* ASCII = 195	11011111011                   */
    0xDFA0, /* ASCII = 196	11011111101                   */
    0xDFE0, /* ASCII = 197	11011111111                   */
    0xEAA0, /* ASCII = 198	11101010101                   */
    0xEAE0, /* ASCII = 199	11101010111                   */
    0xEB60, /* ASCII = 200	11101011011                   */
    0xEBA0, /* ASCII = 201	11101011101                   */
    0xEBE0, /* ASCII = 202	11101011111                   */
    0xED60, /* ASCII = 203	11101101011                   */
    0xEDA0, /* ASCII = 204	11101101101                   */
    0xEDE0, /* ASCII = 205	11101101111                   */
    0xEEA0, /* ASCII = 206	11101110101                   */
    0xEEE0, /* ASCII = 207	11101110111                   */
    0xEF60, /* ASCII = 208	11101111011                   */
    0xEFA0, /* ASCII = 209	11101111101                   */
    0xEFE0, /* ASCII = 210	11101111111                   */
    0xF560, /* ASCII = 211	11110101011                   */
    0xF5A0, /* ASCII = 212	11110101101                   */
    0xF5E0, /* ASCII = 213	11110101111                   */
    0xF6A0, /* ASCII = 214	11110110101                   */
    0xF6E0, /* ASCII = 215	11110110111                   */
    0xF760, /* ASCII = 216	11110111011                   */
    0xF7A0, /* ASCII = 217	11110111101                   */
    0xF7E0, /* ASCII = 218	11110111111                   */
    0xFAA0, /* ASCII = 219	11111010101                   */
    0xFAE0, /* ASCII = 220	11111010111                   */
    0xFB60, /* ASCII = 221	11111011011                   */
    0xFBA0, /* ASCII = 222	11111011101                   */
    0xFBE0, /* ASCII = 223	11111011111                   */
    0xFD60, /* ASCII = 224	11111101011                   */
    0xFDA0, /* ASCII = 225	11111101101                   */
    0xFDE0, /* ASCII = 226	11111101111                   */
    0xFEA0, /* ASCII = 227	11111110101                   */
    0xFEE0, /* ASCII = 228	11111110111                   */
    0xFF60, /* ASCII = 229	11111111011                   */
    0xFFA0, /* ASCII = 230	11111111101                   */
    0xFFE0, /* ASCII = 231	11111111111                   */
    0xAAB0, /* ASCII = 232	101010101011                  */
    0xAAD0, /* ASCII = 233	101010101101                  */
    0xAAF0, /* ASCII = 234	101010101111                  */
    0xAB50, /* ASCII = 235	101010110101                  */
    0xAB70, /* ASCII = 236	101010110111                  */
    0xABB0, /* ASCII = 237	101010111011                  */
    0xABD0, /* ASCII = 238	101010111101                  */
    0xABF0, /* ASCII = 239	101010111111                  */
    0xAD50, /* ASCII = 240	101011010101                  */
    0xAD70, /* ASCII = 241	101011010111                  */
    0xADB0, /* ASCII = 242	101011011011                  */
    0xADD0, /* ASCII = 243	101011011101                  */
    0xADF0, /* ASCII = 244	101011011111                  */
    0xAEB0, /* ASCII = 245	101011101011                  */
    0xAED0, /* ASCII = 246	101011101101                  */
    0xAEF0, /* ASCII = 247	101011101111                  */
    0xAF50, /* ASCII = 248	101011110101                  */
    0xAF70, /* ASCII = 249	101011110111                  */
    0xAFB0, /* ASCII = 250	101011111011                  */
    0xAFD0, /* ASCII = 251	101011111101                  */
    0xAFF0, /* ASCII = 252	101011111111                  */
    0xB550, /* ASCII = 253	101101010101                  */
    0xB570, /* ASCII = 254	101101010111                  */
    0xB5B0 /* ASCII = 255	101101011011                  */
};

static struct {
    const unsigned char* msg;
    int blocksize;
    int32_t dp_fc;
    unsigned short pacc_env;    // 0x100 for 8kSPS
    uint8_t n_preamble;
    uint8_t n_postamble;
    int16_t reg_shift;
    uint16_t current_symbol;
    uint8_t n_zero;
    int16_t n_sample;
    int32_t pacc_fc;
    uint8_t sending;
    bpsk31_cbs_t cbs;
} g;

void bpsk31_init(bpsk31_cbs_t* cbs)
{
    g.cbs = *cbs;
}

void bpsk31_tx(const unsigned char* msg, int blocksize)
{
    g.msg = msg;
    g.blocksize = blocksize;
    g.dp_fc = 64;    // 64: 500 Hz @8 kSPS
    g.pacc_env = 0x100;    // 0x100: pi/4 @8 kSPS
    g.n_preamble = 32;
    g.n_postamble = 32;
    g.reg_shift = VARICODE_TABLE[*msg];
    g.n_zero = 0;
    g.n_sample = 1;
    g.sending = 1;
}

static inline int __SSAT(int32_t VAL, int32_t BITPOS)
{
    int32_t min = -(1 << (BITPOS - 1));
    int32_t max = (1 << (BITPOS - 1)) - 1;
    if(VAL < min)
        return min;
    else if(VAL > max)
        return max;
    else
        return VAL;
}

void bpsk31_poll(void)    //uint16_t blocksize)
{
    int blocksize = 1;    //g.blocksize;
    while(blocksize) {
        if(g.sending) {
            unsigned short out_phase = __SSAT(
                (((int16_t)sinetable[g.pacc_env & 0x03FF]
                    * sinetable[g.pacc_fc & 0x03FF]) >> 15), 16);

            g.cbs.tx_symbol_f(out_phase);
        }
        else {
            g.cbs.tx_symbol_f(0x8000);
        }
        blocksize--;
        g.pacc_fc += g.dp_fc;    // 载波相位+=64, 共用16个相位，8k/16=500Hz

        if(--g.n_sample) {
            if(!g.current_symbol) {    // 如果发0
                g.pacc_env += 2;    // 则包络相位+=2, 共用512个相位 (发完一个字符正好半个周期)
            }
        }
        else {
            g.n_sample = 256;

            if(g.n_preamble) {      // 同步前，发0
                g.current_symbol = 0;
                g.n_preamble--;
            }
            else if(g.blocksize) {    // 待发送
                if(g.reg_shift & 0x8000) {      // 待发字符最高位为1
                    g.current_symbol = 1;
                    g.n_zero = 0;
                }
                else {
                    g.current_symbol = 0;
                    g.n_zero++;
                }

                if((g.n_zero) > 2) {    // 连发两个0则跳过
                    g.blocksize--;
                    g.reg_shift = VARICODE_TABLE[*++g.msg];
                    g.cbs.tx_char_f(*g.msg);
//                    uputc_noblock(*g.message);
                }
                else {
                    g.reg_shift <<= 1;
                }

            }
            else if(g.n_postamble) {    // 同步后， 发1
                g.current_symbol = 1;
                g.n_postamble--;
            }
            else {
                g.sending = 0;
            }
        }
    }
}

