#include "eeprom.h"
#include "filters.h"

bool read_dev_info(uint8_t *pList)
{
  bool status = false;
  if (FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t)) != 0xFF)
  {
    status = true;
    for (uint8_t i = 0; i < 0x08U; ++i)
    {
      pList[i] = FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t) * i);
    }
  }
  return status;
}

void write_dev_info(uint8_t *pList)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  for (uint8_t i = 0; i < 0x08; ++i)
  {
    FLASH_ProgramByte(EEPROM_INFO + sizeof(uint8_t), pList[i]);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void read_packet(uint8_t *pSlave, uint8_t *pFilter)
{
  uint8_t tmp_packet[PACKET_SIZE];
  //Read all of existing packets
  for (uint8_t i = 0; i < COUNT_PACKET; ++i)
  {
    //TODO: read packet to array, then parse it
    get_packet(i, tmp_packet);
    switch (get_check_packet_type(tmp_packet))
    {
    case pckt_pid_slave:
      add_to_array(pSlave, i, tmp_packet[1]);
      break;

    case pckt_pid_filters:
      add_to_array(pFilter, i, tmp_packet[1]);
      break;

    case pckt_undef:
      asm("nop"); // Skip this index
      break;
    }
  }
}

void get_packet(uint8_t index, uint8_t *packet)
{
  uint32_t base_addr = EEPROM_START_PACKET + index * PACKET_SIZE;
  for (uint8_t i = 0; i < PACKET_SIZE; ++i)
  {
    packet[i] = FLASH_ReadByte(base_addr + i);
  }
}

void write_config_packet(uint8_t *pData, uint16_t size)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  for (uint16_t i = 0; i <= size; ++i)
  {
    FLASH_ProgramByte(EEPROM_START_PACKET + i, pData[i]);
  }
  FLASH_ProgramByte(EEPROM_INFO + 1U, 0xFF); // Get set configure flag
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

bool read_config_packet(uint8_t *pData)
{
  if (0x00 == FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t)))
  {
    return false;
  }
  else
  {
    for (uint16_t i = 0; i < CONFIG_SIZE; ++i)
    {
      pData[i] = FLASH_ReadByte(EEPROM_START_PACKET + sizeof(uint8_t) * i);
    }
    return true;
  }
}
