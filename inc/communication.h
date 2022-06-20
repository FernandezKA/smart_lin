#ifndef _communication_h_
#define _communication_h_

#include "main.h"
#include "fifo.h"

#define CPU_ID_BEGIN ((uint32_t) 0x4865U) 
#define CPU_ID_END  ((uint32_t) 0x4870U)

extern uint8_t configArray[CONFIG_SIZE];


enum cmd get_command(uint8_t _data);

enum mode GetDevMode(void);

void get_dev_info(void);

bool get_receive_config(uint8_t* pData, uint16_t* index, uint8_t Data);

void get_send_config(uint8_t* pData);

void upd_config(void);

void send_ack(void);

void send_nack(void);

void send_byte(uint8_t data);

uint8_t get_crc(uint8_t* pData, uint16_t size);

bool check_crc(uint8_t rCRC, uint8_t* pData, uint16_t size);

void print(char* pData);

void print_cpu_id(void);

#endif
