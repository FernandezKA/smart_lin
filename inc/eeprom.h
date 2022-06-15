#ifndef _eeprom_h_
#define _eeprom_h_
#include "main.h"
#include "stm8s_flash.h"

#define EEPROM_INFO ((uint32_t) 0x004000)
#define EEPROM_START_PACKET ((uint32_t) 0x004040)
#define EEPROM_END_PACKET ((uint32_t) 0x004280)


struct edges{
  uint8_t edges_low[8U];
  uint8_t edges_high[8U];
};

struct filer{
  uint8_t PID;
  uint8_t DLC;
  uint8_t DATA[8U];
  uint8_t CRC;
  uint8_t timeout;
};

struct slave_packet{
  uint8_t PID;
  uint8_t DLC;
  struct edges _edge[8U];
  uint8_t diskr_out;
  uint8_t CRC;
};

void get_receive(void);

bool read_dev_info(uint8_t* pList);

void write_dev_info(uint8_t* pList);

bool read_pid_slave(uint8_t* pList);

bool read_pid_filter(uint8_t* pList);

void write_config_packet(uint8_t* pData, uint16_t size);

bool read_config_packet(uint8_t* pData);

#endif