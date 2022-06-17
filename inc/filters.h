#ifndef _filters_h_
#define _filters_h_

#include "main.h"

extern uint8_t pid_slave_array[COUNT_PACKET];
extern uint8_t pid_filters_array[COUNT_PACKET];

struct filter{
  uint8_t low_edge; 
  uint8_t high_edge;
  enum{
    all, 
    more_then, 
    less_then, 
    in_edges, 
    more_or_equal, 
    less_or_equal, 
    in_edges_with_edge, 
    not_equal
  } rule;
};

enum packet_type{
  pckt_pid_slave, 
  pckt_pid_filters, 
  pckt_undef
};
//Check packet type from config. array
enum packet_type get_check_packet_type(uint8_t* pData);
//Check pid valid
bool get_check_pid(uint8_t _pid);
//Add to array with PID new value
void add_to_array(uint8_t* pData, uint8_t index, uint8_t pid);
//Increment addr (right now not used)
void get_next_cell(uint8_t* index);

//Check pid value on array
bool search_pid(uint8_t* pArr, uint8_t PID, uint8_t* index);
//Parse packet from eeprom to struct
void load_slave_packet(uint8_t index, struct lin *packet);
//Read all of packet, then parse PID to arrays
void read_all_packets(void);
#endif