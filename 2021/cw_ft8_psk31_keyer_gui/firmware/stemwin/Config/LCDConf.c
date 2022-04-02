#include "platform.h"
#include "GUI.h"
#include "GUIDRV_FlexColor.h"
//
// Physical display size
//
#define XSIZE_PHYS  240// To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size
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
    pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);

    // Display driver configuration, required for Lin-driver
    LCD_SetSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);

    // Orientation
    Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y | GUI_MIRROR_X;
    Config.NumDummyReads = 1;

    GUIDRV_FlexColor_Config(pDevice, &Config);

    // Set controller and operation mode
    PortAPI.pfWrite8_A0 = ILI9341_WriteReg;
    PortAPI.pfWrite8_A1 = ILI9341_WriteData;
    PortAPI.pfWriteM8_A1 = ILI9341_WriteDataFast;
    PortAPI.pfReadM8_A1 = ILI9341_ReadDataFast;
    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);

    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 319, 499, 3845);
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 239, 3766, 260);
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

