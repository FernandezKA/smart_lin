#ifndef _queue_lin_h_
#define _queue_lin_h_

#include "main.h"

#define QUEUE_LIN_SIZE 0x05U

struct queue_lin{
  uint8_t timeout[QUEUE_LIN_SIZE];
  struct lin packets[QUEUE_LIN_SIZE];
  bool is_sended[QUEUE_LIN_SIZE];
  uint8_t count_added;
};

void queue_clear(struct queue_lin* xQue);

bool queue_handler(struct queue_lin* xQue);

bool queue_add_packet(struct lin* packet, struct queue_lin* xQue);



#endif