#include "usbd_desc.h"

#define USBD_VID                   0x0483
#define USBD_PID                   0x5720
#define USBD_LANGID_STRING         0x409
#define USBD_MANUFACTURER_STRING      (uint8_t*)"ST"
#define USBD_PRODUCT_HS_STRING        (uint8_t*)"MSC_HS"
#define USBD_PRODUCT_FS_STRING        (uint8_t*)"MSC_FS"
#define USBD_CONFIGURATION_HS_STRING  (uint8_t*)"MSC Config"
#define USBD_INTERFACE_HS_STRING      (uint8_t*)"MSC Interface"
#define USBD_CONFIGURATION_FS_STRING  (uint8_t*)"MSC Config"
#define USBD_INTERFACE_FS_STRING      (uint8_t*)"MSC Interface"

USBD_DEVICE USR_desc = {
    USBD_USR_DeviceDescriptor,
    USBD_USR_LangIDStrDescriptor,
    USBD_USR_ManufacturerStrDescriptor,
    USBD_USR_ProductStrDescriptor,
    USBD_USR_SerialStrDescriptor,
    USBD_USR_ConfigStrDescriptor,
    USBD_USR_InterfaceStrDescriptor,
};
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ )      
#pragma data_alignment=4
#endif
#endif                          
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __ALIGN_END = {
    0x12,                         
    USB_DEVICE_DESCRIPTOR_TYPE,   
    0x00,                         
    0x02,
    0x00,                         
    0x00,                         
    0x00,                         
    USB_OTG_MAX_EP0_SIZE,         
    LOBYTE(USBD_VID),             
    HIBYTE(USBD_VID),             
    LOBYTE(USBD_PID),             
    HIBYTE(USBD_PID),             
    0x00,                         
    0x02,
    USBD_IDX_MFC_STR,             
    USBD_IDX_PRODUCT_STR,         
    USBD_IDX_SERIAL_STR,          
    USBD_CFG_MAX_NUM              
};                              
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ )      
#pragma data_alignment=4
#endif
#endif                          
__ALIGN_BEGIN uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]
__ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ )      
#pragma data_alignment=4
#endif
#endif                          
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END = {
    USB_SIZ_STRING_LANGID,
    USB_DESC_TYPE_STRING,
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING),
};
uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] = {
    USB_SIZ_STRING_SERIAL,
    USB_DESC_TYPE_STRING,
};
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ )      
#pragma data_alignment=4
#endif
#endif                          
__ALIGN_BEGIN uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ] __ALIGN_END;
static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len);
static void Get_SerialNum(void);
uint8_t *USBD_USR_DeviceDescriptor(uint8_t speed, uint16_t * length)
{
    *length = sizeof(USBD_DeviceDesc);
    return (uint8_t *) USBD_DeviceDesc;
}
uint8_t *USBD_USR_LangIDStrDescriptor(uint8_t speed, uint16_t * length)
{
    *length = sizeof(USBD_LangIDDesc);
    return (uint8_t *) USBD_LangIDDesc;
}
uint8_t *USBD_USR_ProductStrDescriptor(uint8_t speed, uint16_t * length)
{
    if (speed == 0) {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_PRODUCT_HS_STRING, USBD_StrDesc,
                length);
    }
    else {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_PRODUCT_FS_STRING, USBD_StrDesc,
                length);
    }
    return USBD_StrDesc;
}
uint8_t *USBD_USR_ManufacturerStrDescriptor(uint8_t speed, uint16_t * length)
{
    USBD_GetString((uint8_t *) (uint8_t *) USBD_MANUFACTURER_STRING, USBD_StrDesc,
            length);
    return USBD_StrDesc;
}
uint8_t *USBD_USR_SerialStrDescriptor(uint8_t speed, uint16_t * length)
{
    *length = USB_SIZ_STRING_SERIAL;
    Get_SerialNum();
    return (uint8_t *) USBD_StringSerial;
}
uint8_t *USBD_USR_ConfigStrDescriptor(uint8_t speed, uint16_t * length)
{
    if (speed == USB_OTG_SPEED_HIGH) {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_CONFIGURATION_HS_STRING,
                USBD_StrDesc, length);
    }
    else {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_CONFIGURATION_FS_STRING,
                USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}
uint8_t *USBD_USR_InterfaceStrDescriptor(uint8_t speed, uint16_t * length)
{
    if (speed == 0) {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_INTERFACE_HS_STRING,
                USBD_StrDesc, length);
    }
    else {
        USBD_GetString((uint8_t *) (uint8_t *) USBD_INTERFACE_FS_STRING,
                USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}
static void Get_SerialNum(void)
{
    uint32_t deviceserial0, deviceserial1, deviceserial2;
    deviceserial0 = *(uint32_t *) DEVICE_ID1;
    deviceserial1 = *(uint32_t *) DEVICE_ID2;
    deviceserial2 = *(uint32_t *) DEVICE_ID3;
    deviceserial0 += deviceserial2;
    if (deviceserial0 != 0)
    {
        IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
        IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
    }
}
static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len)
{
    uint8_t idx = 0;
    for (idx = 0; idx < len; idx++)
    {
        if (((value >> 28)) < 0xA)
        {
            pbuf[2 * idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2 * idx] = (value >> 28) + 'A' - 10;
        }
        value = value << 4;
        pbuf[2 * idx + 1] = 0;
    }
}
