#ifndef _lin_hw_layer_h_
#define _lin_hw_layer_h_

#define LIN_UART UART1

#include "main.h"

// Extern user variables

extern uint16_t u16BAUD;

void vSetBAUD(uint16_t _BAUD);

void vSetBreakLength(uint16_t _BAUD);

uint16_t u16GetBAUD(void);

uint8_t u8GetCRC(struct lin_packet _packet);

void vLinPacketClear(struct lin_packet *_packet);

uint8_t u8GetSizeDataFrame(struct lin_packet _packet);

bool bLinCheckBreak(struct Break* _break);

struct Break
{
  enum
  {
    wait_fall,
    increment,
    detect_rise
  } break_fsm;
  uint8_t u8Counter;
};

extern struct Break xBreak;

#endif