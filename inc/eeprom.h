#ifndef _eeprom_h_
#define _eeprom_h_
#include "main.h"
#include "stm8s_flash.h"

#define EEPROM_INFO ((uint32_t) 0x004000)
#define EEPROM_START_PACKET ((uint32_t) 0x004000)
#define EEPROM_END_PACKET ((uint32_t) 0x004280)

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
bool read_config_packet(void);
//Write single byte to eeprom 
void get_write_byte_eeprom(uint8_t _data, uint32_t pAddr);


#endif