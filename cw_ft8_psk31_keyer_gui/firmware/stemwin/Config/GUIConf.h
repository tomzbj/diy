#ifndef GUICONF_H
#define GUICONF_H
#define GUI_NUM_LAYERS            2    // Maximum number of available layers

#ifdef OS_SUPPORT
#define GUI_OS                    (1)  // Compile with multitasking support
#else
#define GUI_OS                    (0)
#endif

#ifndef   GUI_SUPPORT_TOUCH
#define GUI_SUPPORT_TOUCH       (1)  // Support touchscreen
#endif

#define GUI_DEFAULT_FONT          &GUI_Font6x8
#define GUI_SUPPORT_MOUSE             (1)    
#define GUI_WINSUPPORT                (1)    
#define GUI_SUPPORT_MEMDEV            (1)    
#define GUI_SUPPORT_DEVICES           (1)    

#define WM_SUPPORT_NOTIFY_VIS_CHANGED (1)

#endif  
