#ifndef _main_h_
#define _main_h_

#define MAX_SIZE_DATA_FRAME 0x08U

#define HAT_SIZE    ((uint8_t) 8U)
#define PACKET_SIZE ((uint8_t) 24U)
#define COUNT_PACKET 2U
#define CONFIG_SIZE ((uint8_t) HAT_SIZE + PACKET_SIZE * COUNT_PACKET)

#define F_CPU 16000000U
#define GET_BITS(x, pos) ((x & (1 << pos)) >> pos)

#define MODE_PORT GPIOD
#define MODE_PIN ((uint8_t) (1U<<3))
#define LED_PORT GPIOB
#define LED_PIN  (1U<<5)
#include "stm8s_conf.h"
#include <stdbool.h>

#include "structure_definitions.h"
#include "lin_hw_layer.h"
#include "packet_parser.h"
#include "communication.h"
#include "filters.h"
#include "fifo.h"
#include "eeprom.h"

INTERRUPT void EXTI_PORTD_IRQHandler(void);
INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */

extern uint32_t BAUDRATE;
extern struct lin lin_rec;
extern struct lin lin_tr;
extern enum Receive_FSM eLinReceive;

extern uint32_t led_div;

enum mode{
  config, 
  work
};

extern struct lin rec_lin, tr_lin;
extern enum mode curr_mode; 
extern struct FIFO_STR uart_rx, uart_tx;

#endif