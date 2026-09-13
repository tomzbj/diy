/* Author: tomzbj  Updated: 2024-05-29 Rev: 0.03 */

#define ZPIN_STM32_STP

#ifdef ZPIN_GD32
#define ZPIN_PORT_IN(port) (GPIO_ISTAT(port))
#define ZPIN_PORT_OUT(port) (GPIO_OCTL(port))
#define ZPIN_PORT_BSRR(port) (GPIO_BOP(port))
#define ZPIN_PORT_BRR(port) (GPIO_BC(port))
#define ZPIN_PORT_TYPE uint32_t

#endif

#ifdef ZPIN_STM32_STP
#define ZPIN_PORT_IN(port) (port->IDR)
#define ZPIN_PORT_OUT(port) (port->ODR)
#define ZPIN_PORT_BSRR(port) (port->BSRR)
#define ZPIN_PORT_BRR(port) (port->BRR)
#define ZPIN_PORT_TYPE GPIO_TypeDef*
#endif
