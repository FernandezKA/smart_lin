#include "filters.h"

uint8_t pid_slave_array[COUNT_PACKET];
uint8_t pid_filters_array[COUNT_PACKET];
uint8_t pid_triggered_array[COUNT_PACKET];

enum packet_type get_check_packet_type(uint8_t *pData)
{
  if (pData[1] == 0x01U)
  {
    return pckt_pid_filters;
  }
  else if (pData[1] == 0x02U)
  {
    return pckt_pid_slave;
  }
  else
  {
    return pckt_undef;
  }
}

bool get_check_pid(uint8_t _pid)
{
  if (_pid > 0x00U && _pid <= 0x3FU)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void add_to_array(uint8_t *pData, uint8_t index, uint8_t pid)
{
  pData[index] = pid;
}

void get_next_cell(uint8_t *index)
{
  if (*index < COUNT_PACKET)
  {
    ++(*index);
  }
}

bool search_pid(uint8_t *pArr, uint8_t PID, uint8_t *index)
{
  bool state = false;
  for (uint8_t i = 0; i < COUNT_PACKET; ++i)
  {
    if (pArr[i] == PID)
    {
      state = true;
      *index = i;
      break;
    }
  }
  return state;
}

void load_slave_packet(uint8_t index, struct lin *packet)
{
  uint8_t tmpArr[PACKET_SIZE];
  get_packet(index, tmpArr);
  packet->pid = tmpArr[0];
  packet->dlc = tmpArr[2];
  for (uint8_t i = 0; i < packet->dlc; ++i)
  {
    packet->data[i] = tmpArr[3U + i];
  }
  packet->crc = tmpArr[11U];
}

void load_filter_packet(uint8_t index, struct filter *packet)
{
  uint8_t tmpArr[PACKET_SIZE];
  get_packet(index, tmpArr);
  packet->pid = tmpArr[0];
  // tmpArr[1] reserved for type
  packet->dlc = tmpArr[2];
  for(uint8_t i = 0; i < 8; ++i)
  {
    packet->edges_low[i] = tmpArr[3U + 2*i];
    packet->edges_high[i] = tmpArr[4U + 2*i];
  }
  packet->timeout = tmpArr[19U];
  packet->btn_state = tmpArr[20U];
  packet->remove_after_use = tmpArr[21U];
}

bool get_check_filter(struct lin *packet_lin, struct filter *packet_filter)
{
  bool rules_trig = true;
  rules_trig &= (packet_lin->data[0] >= packet_filter->edges_low[0]);
  rules_trig &= (packet_lin->data[1] >= packet_filter->edges_low[1]);
//  rules_trig &= (packet_lin->data[2] >= packet_filter->edges_low[2]);
//  rules_trig &= (packet_lin->data[3] >= packet_filter->edges_low[3]);
//  rules_trig &= (packet_lin->data[4] >= packet_filter->edges_low[4]);
//  rules_trig &= (packet_lin->data[5] >= packet_filter->edges_low[5]);
//  rules_trig &= (packet_lin->data[6] >= packet_filter->edges_low[6]);
//  rules_trig &= (packet_lin->data[7] >= packet_filter->edges_low[7]);
  rules_trig &= get_btn_state(packet_filter->btn_state);
  return rules_trig;
}

void get_add_to_trig_list(uint8_t* pArray, uint8_t* index, uint8_t pid){
  bool isEmpty = true;
  for(uint8_t i = 0; i < COUNT_PACKET; ++i){
    if(pArray[i] != 0x00U){
     isEmpty = false; 
     break;
    }
  }
  
  if(isEmpty){
    *index = 0x00U;
  }
  
  if(*index < COUNT_PACKET - 1U){
    pArray[(*index)++] = pid;
  }
  else{
    pArray[*index] = pid;//Simple rewrite last cell into array
  }
}

void get_remove_pid(uint8_t* pArray, uint8_t pid){
  for(uint8_t i = 0; i < COUNT_PACKET; ++i){
    if(pArray[i] == pid){       
      pArray[i] = 0x00U;
      break;
    }
  }
}

bool get_btn0_state(void)
{
  return btn_0;
}

bool get_btn1_state(void){
  return btn_1;
}

bool get_btn_state(uint8_t state){
  bool _state = false;
  switch(state){
  case 0x00U:
    _state = true;
    break;
    
  case 0x01U:
    _state = get_btn0_state();
    break;
    
  case 0x02U:
    _state = get_btn1_state();
    break;
    
  case 0x03U:
    _state = get_btn0_state() | get_btn1_state();
    break;
    
  default:
   _state = false;
    break;
  }  
  return _state;
}
