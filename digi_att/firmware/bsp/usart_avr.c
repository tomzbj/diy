#include "misc.h"
#include "platform.h"

unsigned char USART_ReadByte(int source)
{
    return UDR0;
}

void USART_WriteByte(unsigned char c, int source)
{
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = c;
}

static int uputc(char c, FILE* stream)
{
    if(c == '\n')
        uputc('\r', stream);
    USART_WriteByte(c, 0);
    return 0;
}

void USART_Config(void)
{
    const unsigned long baudrate = 500000UL;
    DDRD = _BV(PD1);
    UCSR0A = 0x00;
    UCSR0C = 0x06;
    UBRR0 = F_CPU / baudrate / 16 - 1;
    UCSR0B = 0x18;

    UCSR0B |= _BV(RXCIE0);

    static FILE mystdout = FDEV_SETUP_STREAM(uputc, NULL, _FDEV_SETUP_WRITE);
    stdout = &mystdout;

//    while(1) { uputc('K'); _delay_ms(100); }
}
