#ifndef _communication_h_
#define _communication_h_

#include "main.h"
#include "fifo.h"

#define HAT_SIZE    ((uint8_t) 8U)
#define PACKET_SIZE ((uint8_t) 24U)
#define COUNT_PACKET 10U
#define CONFIG_SIZE ((uint8_t) HAT_SIZE + PACKET_SIZE * COUNT_PACKET)

extern uint8_t configArray[CONFIG_SIZE];

enum cmd{
  dev_info = 0x40U, 
  read_config = 0x50U, 
  write_config = 0x60U, 
  undef
};
enum cmd get_command(uint8_t _data);

enum mode GetDevMode(void);

void get_dev_info(struct FIFO_STR* fifo);

bool get_receive_config(uint8_t* pData, uint16_t* index, uint8_t Data);

void send_ack(void);

void send_nack(void);

void send_byte(uint8_t data);

uint8_t get_crc(uint8_t* pData, uint16_t size);

bool check_crc(uint8_t rCRC, uint8_t* pData, uint16_t size);

void print(char* pData);

#endif
