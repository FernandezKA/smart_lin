#ifndef _lin_hw_layer_h_
#define _lin_hw_layer_h_

#define LIN_UART UART1
#define LIN_SYNCH ((uint8_t) 0x55U)
#include "main.h"

// Extern user variables


struct Break
{
  enum
  {
    wait_fall,
    increment,
    detect_rise
  } break_fsm;
  uint16_t u16Counter;
};

extern struct Break xBreak;

extern uint16_t u16BAUD;

void vSetBAUD(uint16_t _BAUD);

void vSetBreakLength(uint16_t _BAUD);

uint16_t u16GetBAUD(void);

uint8_t u8GetCRC(struct lin _packet);

void vLinPacketClear(struct lin *_packet);

uint8_t u8GetSizeDataFrame(struct lin _packet);

bool bLinCheckBreak(struct Break* _break);

void vConfigBreak(void);

void vConfigLIN(void);

void get_send_lin_packet(struct lin* packet);

void get_send_data_frame(struct lin* packet);

//Queue for send



#endif