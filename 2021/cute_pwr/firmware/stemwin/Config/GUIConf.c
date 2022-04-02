#include "GUI.h"

// Define the available number of bytes available for the GUI
#define GUI_NUMBYTES  (16 * 1024)

void GUI_X_Config(void)
{
    static U32 aMemory[GUI_NUMBYTES / 4];    // 32 bit aligned memory area
    GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);    // Assign memory to emWin
    GUI_SetDefaultFont(GUI_FONT_6X8);    // Set default font
}
