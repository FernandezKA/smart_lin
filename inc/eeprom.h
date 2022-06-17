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
//Receive config. file
void get_receive(void);
//Return block with inf. about device
bool read_dev_info(uint8_t* pList);
//Write block with dev. information
void write_dev_info(uint8_t* pList);
//Parse all of data ti the lists of PID (slave + filters)
void read_packet(uint8_t* pSlave, uint8_t* pFilter);
//Return packet from config. file
void get_packet(uint8_t index, uint8_t* packet);
//Write data of config packet to EEPROM
void write_config_packet(uint8_t* pData, uint16_t size);
//Read all of data in config. packet
bool read_config_packet(uint8_t* pData);

#endif