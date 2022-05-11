#ifndef _main_h_
#define _main_h_

#define F_CPU 8000000U
#define GET_BITS(x, pos) ((x & (1 << pos)) >> pos)

#include "stm8s_conf.h"
#include <stdbool.h>

#include "structure_definitions.h"
#include "lin_hw_layer.h"

INTERRUPT void EXTI_PORTD_IRQHandler(void);
INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */

#endif