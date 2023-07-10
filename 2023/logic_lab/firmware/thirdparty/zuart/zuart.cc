#include "ZUart.h"

#include <string.h>

ZUart::ZUart(int rx_buf_size, int tx_buf_size)
{
    _rx_buf_size = rx_buf_size;
    _msg = new unsigned char[rx_buf_size];
    _rx_buf = new unsigned char[rx_buf_size];
    _rx_ptr = _rx_buf;
    _rx_size = 0;
    _updated = 0;
#if ZUART_TX_MODE_INTERRUPT
    _tx_buf = new unsigned char[tx_buf_size];
    _tx_ptr = tx_buf;
    _tx_size = 0;
#endif
    parser_f = NULL;
    readbyte_f = NULL;
    writebyte_f = NULL;
}

ZUart::~ZUart(void)
{
}

void ZUart::bind(
    //
    void (*parser_f)(const void* _msg, int size),
    unsigned char (*readbyte_f)(void), void (*writebyte_f)(unsigned char data)    //
        )
{
    this->parser_f = parser_f;
    this->readbyte_f = readbyte_f;
    this->writebyte_f = writebyte_f;
}

void ZUart::writedata(void* msg, int size)
{
    unsigned char* pmsg = (unsigned char*)msg;
#if ZUART_TX_MODE_INTERRUPT
    while(_tx_size > 0);
    if(size > ZUART_TX_MAX_LEN)
        size = ZUART_TX_MAX_LEN;
    memcpy(tx_buf, pmsg, size);
    if(writebyte_f != NULL)
        writebyte_f(tx_buf[0]);
    _tx_ptr = _tx_buf + 1;
    _tx_size = _size - 1;
#else
    if(writebyte_f != NULL)
        for(int i = 0; i < size; i++)
            writebyte_f(*pmsg++);
#endif
}

void ZUart::poll(void)
{
    if(_updated && parser_f != NULL) {
        parser_f(_msg, _rx_size);
        _updated = 0;
    }
}

void ZUart::tc_irqhandler(void)
{
#if ZUART_TX_MODE_INTERRUPT
    if(_tx_size > 0) {
        _writebyte_f(*_tx_ptr);
        _tx_ptr++;
        _tx_size--;
    }
#endif
}

void ZUart::rxne_irqhandler(void)
{
    if(_rx_ptr - _rx_buf >= _rx_buf_size || readbyte_f == NULL)
        return;
    *_rx_ptr = readbyte_f();    //usartx->DR;
    _rx_ptr++;
}

void ZUart::idle_irqhandler(void)
{
    _rx_size = _rx_ptr - _rx_buf;
    memcpy(_msg, _rx_buf, _rx_size);
    memset(_rx_buf, 0, _rx_buf_size);
    _rx_ptr = _rx_buf;
    _updated = 1;
}
