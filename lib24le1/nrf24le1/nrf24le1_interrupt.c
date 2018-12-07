#include "nrf24le1.h"

void INTERRUPT_EnableIRQ(IRQn_TypeDef irq)
{
    unsigned char reg = irq & 0xff;
    IEN0 |= reg;
    reg = (irq >> 8) & 0xff;
    IEN1 |= reg;
    reg = (irq >> 16) & 0xff;
    INTEXP |= reg;
}

void INTERRUPT_DisableIRQ(IRQn_TypeDef irq)
{
    unsigned char reg = irq & 0xff;
    IEN0 &= ~reg;
    reg = (irq >> 8) & 0xff;
    IEN1 &= ~reg;
    reg = (irq >> 16) & 0xff;
    INTEXP &= ~reg;
}
