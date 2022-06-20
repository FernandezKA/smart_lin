#include "queue_lin.h"

void queue_clear(struct queue_lin* xQue){
  for(uint8_t i = 0; i < QUEUE_LIN_SIZE; ++i){
    xQue->is_sended[i] = false;
    xQue->timeout[i] = 0x00U;
  }
  xQue->count_added = 0x00U;
}

bool queue_handler(struct queue_lin* xQue){
  uint8_t index_timeout = 0x00;
  bool timeout_handle = false;
  for(uint8_t i = 0; i < xQue->count_added; ++i){
    if(xQue->timeout[i] != 0x00 && !(xQue->is_sended)){
      xQue->timeout[i]--;
    }
    else if(xQue->timeout[i] == 0x00U && !(xQue->is_sended)){
      get_send_lin_packet(&xQue->packets[index_timeout]); 
      xQue->is_sended[i] = true;
      timeout_handle = true;
    }
  } 
  return timeout_handle;
}

bool queue_add_packet(struct lin* packet, struct queue_lin* xQue){
  if(xQue->count_added < QUEUE_LIN_SIZE){
    xQue->packets[xQue->count_added++] =  *packet;
    return true;
  }
  else{
    return false;
  }
}
