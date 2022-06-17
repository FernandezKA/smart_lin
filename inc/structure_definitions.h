#ifndef _structure_definitions_h_
#define _structure_definitions_h_

#include "main.h"

struct lin{
  uint8_t pid;
  uint8_t dlc;
  uint8_t data[8U]; 
  uint8_t crc;
  uint8_t timeout;
  uint8_t cnt_receive;
};

struct send_delay{
  uint8_t counter_timeout; 
  struct lin packet;
  bool is_load;
};

extern struct send_delay delay_packet;

#endif