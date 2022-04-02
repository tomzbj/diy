#ifndef _USART_PARSER_H
#define _USART_PARSER_H

enum {
    STDOUT_TARGET_USART1, STDOUT_TARGET_USB_VCP
};

enum {
    USART_SOURCE_USART1, USART_SOURCE_USB_VCP
};
void usart_parser(const void* msg, int size, int source);
void stdout_write(void* ptr, int len);

#endif
