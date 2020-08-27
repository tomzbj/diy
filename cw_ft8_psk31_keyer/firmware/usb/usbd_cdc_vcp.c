#include "usbd_cdc_vcp.h"
#include "misc.h"
#include "platform.h"

LINE_CODING linecoding = {115200, 0x00, 0x00, 0x08};
USART_InitTypeDef uis;

extern uint8_t APP_Rx_Buffer[];
extern uint32_t APP_Rx_ptr_in;

static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx(uint8_t* msg, uint32_t size);
static uint16_t VCP_COMConfig(uint8_t Conf);
CDC_IF_Prop_TypeDef VCP_fops = {    //
    VCP_Init, VCP_DeInit, VCP_Ctrl, VCP_DataTx, VCP_DataRx};

static uint16_t VCP_Init(void)
{
    return USBD_OK;
}
static uint16_t VCP_DeInit(void)
{
    return USBD_OK;
}
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
    switch(Cmd) {
        case SEND_ENCAPSULATED_COMMAND:
            break;
        case GET_ENCAPSULATED_RESPONSE:
            break;
        case SET_COMM_FEATURE:
            break;
        case GET_COMM_FEATURE:
            break;
        case CLEAR_COMM_FEATURE:
            break;
        case SET_LINE_CODING:
            linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8)
                | (Buf[2] << 16) | (Buf[3] << 24));
            linecoding.format = Buf[4];
            linecoding.paritytype = Buf[5];
            linecoding.datatype = Buf[6];
            VCP_COMConfig(OTHER_CONFIG);
            break;
        case GET_LINE_CODING:
            Buf[0] = (uint8_t)(linecoding.bitrate);
            Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
            Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
            Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
            Buf[4] = linecoding.format;
            Buf[5] = linecoding.paritytype;
            Buf[6] = linecoding.datatype;
            break;
        case SET_CONTROL_LINE_STATE:
            break;
        case SEND_BREAK:
            break;
        default:
            break;
    }
    return USBD_OK;
}

static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
    return USBD_OK;
}

void USB_VCP_WriteData(const void* msg, int size)
{
    DCD_EP_Tx(&USB_Device_dev, CDC_IN_EP, (uint8_t*)msg, size);
    _delay_us(10);
}

static uint16_t VCP_DataRx(uint8_t* msg, uint32_t size)
{
    usart_parser(msg, size, USART_SOURCE_USB_VCP);
    return USBD_OK;
}

static uint16_t VCP_COMConfig(uint8_t Conf)
{
    return USBD_OK;
}
