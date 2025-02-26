/* Author: tomzbj  Updated: 2024-04-28 Rev: 0.02 */

#define ZPIN_STM32_STP

#ifdef ZPIN_GD32
#define ZPIN_IN_PORT(port) GPIO_ISTAT(port)
#endif

#ifdef ZPIN_STM32_STP
#define ZPIN_PORT_IN(port) (port->IDR)
#define ZPIN_PORT_OUT(port) (port->ODR)
#define ZPIN_PORT_BSRR(port) (port->BSRR)
#define ZPIN_PORT_BRR(port) (port->BRR)
#define ZPIN_PORT_TYPE GPIO_TypeDef*
#endif
