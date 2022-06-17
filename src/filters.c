#include "filters.h"

uint8_t pid_slave_array[COUNT_PACKET];
uint8_t pid_filters_array[COUNT_PACKET];

enum packet_type get_check_packet_type(uint8_t *pData)
{
  if (pData[1] == 0x01)
  {
    return pckt_pid_slave;
  }
  else if (pData[1] == 0x02)
  {
    return pckt_pid_filters;
  }
  else
  {
    return pckt_undef;
  }
}

bool get_check_pid(uint8_t _pid)
{
  if (_pid > 0x00 && _pid <= 0x3F)
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
    }
  }
  return state;
}

void load_slave_packet(uint8_t index, struct lin *packet)
{
  uint8_t tmpArr[PACKET_SIZE];
  get_packet(index, tmpArr);
  packet->pid = tmpArr[0];
  packet->dlc = tmpArr[1];
  for(uint8_t i = 0; i < packet->dlc; ++i){
    packet->data[i] = tmpArr[3U + i];
  }
  packet->crc = tmpArr[11U];
  packet->timeout = tmpArr[12U];
}

void read_all_packets(void){
  for(uint8_t i = 0; i < COUNT_PACKET; ++i){
    
  }
}