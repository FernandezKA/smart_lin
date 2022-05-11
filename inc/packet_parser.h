#ifndef _packet_parser_h_
#define _packet_parser_h_

#include "main.h"

enum Receive_FSM
{
  wait_break,
  wait_synch,
  wait_PID,
  wait_data_frame,
  wait_CRC,
  completed
};

bool bLinPacketReceive(uint8_t _data, enum Receive_FSM *_rec_fsm, struct lin_packet *_packet);

#endif