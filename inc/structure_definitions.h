#ifndef _structure_definitions_h_
#define _structure_definitions_h_

#include "main.h"

#define QUEUE_LIN_SIZE 0x05U

enum cmd{
  dev_info = 0x40U, 
  read_config = 0x50U, 
  write_config = 0x60U, 
  undef
};

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

struct edges{
  uint8_t edges_low[8U];
  uint8_t edges_high[8U];
};

struct slave_packet{
  uint8_t PID;
  uint8_t DLC;
  struct edges _edge[8U];
  uint8_t diskr_out;
  uint8_t CRC;
};

struct queue_lin{
  uint8_t timeout[QUEUE_LIN_SIZE];
  struct lin packets[QUEUE_LIN_SIZE];
  bool is_sended[QUEUE_LIN_SIZE];
  uint8_t count_added;
};

struct filter{
  uint8_t pid;
  uint8_t dlc;
  uint8_t edges_low[MAX_SIZE_DATA_FRAME];
  uint8_t edges_high[MAX_SIZE_DATA_FRAME];
  bool btn_state;
  uint8_t timeout;
};

enum packet_type{
  pckt_pid_slave, 
  pckt_pid_filters, 
  pckt_undef
};

extern struct send_delay delay_packet;

#endif