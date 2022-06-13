#ifndef _packet_parser_h_
#define _packet_parser_h_

#define SYNCH_FIELD 0x55U

#include "main.h"

/*This part of code used for packed received*/

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

/* This part of code used for parse input packed with depend on user defined rules */


#endif