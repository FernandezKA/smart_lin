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

enum out_state{
  no_action = 0x00U, 
  out_0_time= 0x01U, 
  out_0_en = 0x02U, 
  out_0_dis = 0x03U, 
  out_1_time = 0x04U, 
  out_1_en = 0x05U, 
  out_1_dis = 0x06U
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

struct filter{
  uint8_t pid;
  uint8_t dlc;
  uint8_t edges_low[MAX_SIZE_DATA_FRAME];
  uint8_t edges_high[MAX_SIZE_DATA_FRAME];
  uint8_t btn_state;
  uint8_t timeout;
  bool remove_after_use;
  enum out_state out_st;
  uint8_t out_field;
};

enum packet_type{
  pckt_pid_slave, 
  pckt_pid_filters, 
  pckt_undef
};

extern struct send_delay delay_packet;

#endif