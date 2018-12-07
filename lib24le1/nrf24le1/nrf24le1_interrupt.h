#ifndef _NRF24LE1_INTERRUPT_H
#define _NRF24LE1_INTERRUPT_H

typedef enum {
    GLOBAL_IRQn = 0x80, TIMER2_IRQn = 0x20, UART_IRQn = 0x10,
    TIMER1_IRQn = 0x8, PWR_FAIL_IRQn = 0x4, TIMER0_IRQn = 0x2, IFP_IRQn = 0x1,
    TIMER2_EXT_IRQn = 0x8000, TICK_IRQn = 0x2000, MISC_IRQn = 0x1000,
    WUOP_IRQn = 0x800, SPI_I2C_IRQn = 0x400, RF_IRQn = 0x200,
    RF_RDY_IRQn = 0x100, GPINT2_IRQn = 0x200000, GPINT1_IRQn = 0x100000,
    GPINT0_IRQn = 0x80000, I2C_IRQn = 0x40000, MSPI_DONE_IRQn = 0x20000,
    SSPI_DONE_IRQn = 0x10000
} IRQn_TypeDef;

#define IFP_IRQHandler()        void ISR_IFP()      __interrupt(0)
#define TIMER0_IRQHandler()     void ISR_TIMER00()  __interrupt(1)
#define PWR_FAIL_IRQHandler()   void ISR_PWR_FAIL() __interrupt(2)
#define TIMER1_IRQHandler()     void ISR_TIMER1()   __interrupt(3)
#define UART_IRQHandler()       void ISR_UART(void)     __interrupt(4)
#define TIMER2_IRQHandler()     void ISR_TIMER2()   __interrupt(5)
#define RFRDY_IRQHandler()      void ISR_RFRDY()    __interrupt(8)
#define RF_IRQHandler()         void ISR_RF()       __interrupt(9)
#define SPI_I2C_IRQHandler()    void ISR_SPI_I2C()  __interrupt(10)
#define WUOP_IRQHandler()       void ISR_WUOP()     __interrupt(11)
#define MISC_IRQHandler()       void ISR_MISC()     __interrupt(12)
#define TICK_IRQHandler()       void ISR_TICK()     __interrupt(13)

void INTERRUPT_EnableIRQ(IRQn_TypeDef irq);
void INTERRUPT_DisableIRQ(IRQn_TypeDef irq);
void INTERRUPT_ClearIRQ(IRQn_TypeDef irq);

#endif
