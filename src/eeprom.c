#include "eeprom.h"

bool read_dev_info(uint8_t* pList){
  bool status = false;
  if(FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t)) != 0xFF){
    status = true;
    for(uint8_t i = 0; i < 0x08U; ++i){
      pList[i] = FLASH_ReadByte(EEPROM_INFO +sizeof(uint8_t) * i);
    }
  }
  return status;
}

void write_dev_info(uint8_t* pList)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  for(uint8_t i = 0; i < 0x08; ++i){
    FLASH_ProgramByte(EEPROM_INFO + sizeof(uint8_t), pList[i]);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

bool read_pid_slave(uint8_t* pList)
{
  for(uint8_t i = 0; i < COUNT_PACKET; ++i){
    if(FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i + 1U) != 0x01){ //Read byte RL_TYPE (Next from PID)
      pList[i] = 0x00U;
    }
    else{ //If it's 0x01 type of packet, add to list with PID
      pList[i] = FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i);
    }
  }
  return true;
}

bool read_pid_filter(uint8_t* pList)
{
  for(uint8_t i = 0; i < COUNT_PACKET; ++i){
    if(FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i + 1U) != 0x02){ //Read byte RL_TYPE (Next from PID)
      pList[i] = 0x00U;
    }
    else{ //If it's 0x01 type of packet, add to list with PID
      pList[i] = FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i);
    }
  }
  return true;
}

void write_config_packet(uint8_t* pData, uint16_t size){
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  for(uint16_t i = 0; i < size; ++i){
    FLASH_ProgramByte(EEPROM_START_PACKET + sizeof(uint8_t), pData[i]);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

bool read_config_packet(uint8_t* pData){
  if(0xFF == FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t))){
    return false;
  }
  else{
    for(uint16_t i = 0; i < CONFIG_SIZE; ++i){
      pData[i] = FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i);
    }
    return true;
  }
}