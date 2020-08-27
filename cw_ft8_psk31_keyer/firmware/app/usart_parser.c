#include "platform.h"

enum {
    FRAME_TYPE_NONE,
    FRAME_TYPE_BUSY,
    FRAME_TYPE_CLI,
    FRAME_TYPE_USB,
    FRAME_TYPE_PAYLOAD
};

static struct {
    int stdout_target;
} g = {STDOUT_TARGET_USART1};

void stdout_write(void* ptr, int len)
{
    if(g.stdout_target == STDOUT_TARGET_USART1)
        USART_WriteData(USART1, ptr, len);
    else if(g.stdout_target == STDOUT_TARGET_USB_VCP)
        USB_VCP_WriteData(ptr, len);
}

void usart_parser(const void* msg, int size, int source)
{
    char* pmsg = (char*)msg;
    char type;
    if(pmsg[0] == '?')
        type = FRAME_TYPE_CLI;
    if(pmsg[0] == '#' && pmsg[1] == '#')
        type = FRAME_TYPE_CLI;
    if(pmsg[0] == '$')
        type = FRAME_TYPE_PAYLOAD;

    if(source == USART_SOURCE_USART1)
        g.stdout_target = STDOUT_TARGET_USART1;
    else if(source == USART_SOURCE_USB_VCP)
        g.stdout_target = STDOUT_TARGET_USB_VCP;

    switch(type) {
        case FRAME_TYPE_NONE:
            return;
        case FRAME_TYPE_CLI:
            CLI_Parse(msg, size);
            break;
        case FRAME_TYPE_PAYLOAD: {
            DataConfig_t* pdc = DC_Get();
            pmsg++;
            size--;
            if(pdc->cfg.tx_mode == DC_TX_MODE_CW)
                cw_tx(pmsg, size);
            else if(pdc->cfg.tx_mode == DC_TX_MODE_BPSK)
                bpsk31_tx((const unsigned char*)pmsg, size);
            else if(pdc->cfg.tx_mode == DC_TX_MODE_FT8)
                ft8_tx(pmsg, size);
            break;
        }
        default:
            break;
    }
    type = FRAME_TYPE_NONE;
}
