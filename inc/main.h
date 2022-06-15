#ifndef _main_h_
#define _main_h_

#define MAX_SIZE_DATA_FRAME 0x08U

#define F_CPU 16000000U
#define GET_BITS(x, pos) ((x & (1 << pos)) >> pos)

#define MODE_PORT GPIOD
#define MODE_PIN ((uint8_t) (1U<<3))

#include "stm8s_conf.h"
#include <stdbool.h>

#include "structure_definitions.h"
#include "lin_hw_layer.h"
#include "packet_parser.h"
#include "communication.h"
#include "fifo.h"
#include "eeprom.h"

INTERRUPT void EXTI_PORTD_IRQHandler(void);
INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */

extern uint32_t BAUDRATE;
extern struct lin_packet lin_rec;
extern enum Receive_FSM eLinReceive;

enum mode{
  config, 
  work
};

extern enum mode curr_mode; 
extern struct FIFO_STR uart_rx, uart_tx;

#endif