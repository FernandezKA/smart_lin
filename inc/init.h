#ifndef _init_h_
#define _init_h_
#include "main.h"

void CLK_Init(void);

void UART_Init(uint16_t BAUD);

void LIN_Init(void);

void PORT_Init(void);

void TIM1_Init(void);

void TIM4_Init(void);

void IRQ_Init(void);

void config_uart(void);

void config_lin(void);

void set_baud(uint16_t _baud);
#endif