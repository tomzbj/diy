#ifndef _ZUART_H
#define _ZUART_H

#define ZUART_TX_MODE_INTERRUPT 0

class ZUart {
public:
    ZUart(int rx_buf_size, int tx_buf_size);
    ~ZUart(void);
    void poll(void);
    void idle_irqhandler(void);
    void rxne_irqhandler(void);
    void tc_irqhandler(void);
    void writedata(void* msg, int size);
    void bind(
        //
        void (*parser_f)(const void* _msg, int size),
        unsigned char (*readbyte_f)(void),
        void (*writebyte_f)(unsigned char data)    //
            );

private:
    void (*parser_f)(const void* _msg, int size);
    unsigned char (*readbyte_f)(void);
    void (*writebyte_f)(unsigned char data);

    unsigned char* _msg;
    unsigned char* _rx_buf;
    unsigned char* _rx_ptr;
    int _rx_size, _updated;
    int _rx_buf_size;

#if ZUART_TX_MODE_INTERRUPT
    unsigned char* _tx_buf;
    unsigned char* _tx_ptr;
    int _tx_size;
#endif

};

#endif
