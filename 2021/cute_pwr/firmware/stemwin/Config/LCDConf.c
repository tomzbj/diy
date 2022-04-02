#include "platform.h"
#include "GUI.h"
#include "GUIDRV_FlexColor.h"
//
// Physical display size
//
#define XSIZE_PHYS  132 // To be adapted to x-screen size
#define YSIZE_PHYS  132 // To be adapted to y-screen size
#ifndef   VXSIZE_PHYS
#define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
#define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
#error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
#error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
#error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
#error No display driver defined!
#endif

void LCD_X_Config(void)
{
    GUI_DEVICE* pDevice;
    CONFIG_FLEXCOLOR Config = {0};
    GUI_PORT_API PortAPI = {0};

    // Set display driver and color conversion
    pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);

    // Display driver configuration, required for Lin-driver
    LCD_SetSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);

    // Orientation
    //    Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y;
    Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y | !GUI_MIRROR_X;
    //    Config.Orientation =  0;
    GUIDRV_FlexColor_Config(pDevice, &Config);

    // Set controller and operation mode
    PortAPI.pfWrite8_A0 = ST7735_WriteReg;
    PortAPI.pfWrite8_A1 = ST7735_WriteData;
    PortAPI.pfWriteM8_A1 = ST7735_WriteDataFast;
    PortAPI.pfReadM8_A1 = ST7735_ReadDataFast;
    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void* pData)
{
    int r;
    (void)LayerIndex;
    (void)pData;
    switch(Cmd) {
        case LCD_X_INITCONTROLLER: {
            return 0;
        }
        default:
            r = -1;
    }
    return r;
}
