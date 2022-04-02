
#include "usbd_desc.h"
#define USBD_VID                   0x0483
#define USBD_PID                   0x5720
#define USBD_LANGID_STRING         0x409
#define USBD_MANUFACTURER_STRING   "STMicroelectronics"
#define USBD_PRODUCT_FS_STRING     "Mass Storage in FS Mode"
#define USBD_CONFIGURATION_FS_STRING  "MSC Config"
#define USBD_INTERFACE_FS_STRING      "MSC Interface"
char USBD_SERIALNUMBER_FS_STRING[26];
USBD_DEVICE USR_desc =
{
    USBD_USR_DeviceDescriptor,
    USBD_USR_LangIDStrDescriptor, 
    USBD_USR_ManufacturerStrDescriptor,
    USBD_USR_ProductStrDescriptor,
    USBD_USR_SerialStrDescriptor,
    USBD_USR_ConfigStrDescriptor,
    USBD_USR_InterfaceStrDescriptor,
};
const uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] =
{
    0x12,                       
    USB_DEVICE_DESCRIPTOR_TYPE, 
    0x00,                       
    0x02,
    0x00,                       
    0x00,                       
    0x00,                       
    USB_MAX_EP0_SIZE,           
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
} ; 
const uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
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
const uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] =
{
    USB_SIZ_STRING_LANGID,         
    USB_DESC_TYPE_STRING,       
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING), 
};
uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] =
{
    USB_SIZ_STRING_SERIAL,       
    USB_STRING_DESCRIPTOR_TYPE,  
};
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
uint8_t *  USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
    *length = sizeof(USBD_DeviceDesc);
    return (uint8_t*)USBD_DeviceDesc;
}
uint8_t *  USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
    *length =  sizeof(USBD_LangIDDesc);  
    return (uint8_t*)USBD_LangIDDesc;
}
uint8_t *  USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString ( (uint8_t*)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);    
    return USBD_StrDesc;
}
uint8_t *  USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString ( (uint8_t*)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}
uint8_t *  USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length)
{
    *length = USB_SIZ_STRING_SERIAL; 
    return USBD_StringSerial;
}
uint8_t *  USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString ( (uint8_t*)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length); 
    return USBD_StrDesc;  
}
uint8_t *  USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{
    USBD_GetString ( (uint8_t*)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;  
}
void Get_SerialNum(void)
{
    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
    Device_Serial0 = *(uint32_t*)Device1_Identifier;
    Device_Serial1 = *(uint32_t*)Device2_Identifier;
    Device_Serial2 = *(uint32_t*)Device3_Identifier;
    Device_Serial0 += Device_Serial2;
    if (Device_Serial0 != 0)
    {
        IntToUnicode (Device_Serial0, &USBD_StringSerial[2] ,8);
        IntToUnicode (Device_Serial1, &USBD_StringSerial[18] ,4);
    }
}
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
    uint8_t idx = 0;
    for( idx = 0 ; idx < len ; idx ++)
    {
        if( ((value >> 28)) < 0xA )
        {
            pbuf[ 2* idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2* idx] = (value >> 28) + 'A' - 10; 
        }
        value = value << 4;
        pbuf[ 2* idx + 1] = 0;
    }
}

