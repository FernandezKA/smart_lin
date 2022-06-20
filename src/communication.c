#include "communication.h"
#include "eeprom.h"
#include "init.h"

uint8_t configArray[CONFIG_SIZE];

enum mode GetDevMode(void)
{
  enum mode _mode;
  (((MODE_PORT->IDR) & MODE_PIN) == MODE_PIN) ? (_mode = config) : (_mode = work);
  return _mode;
}

enum cmd get_command(uint8_t _data)
{
  enum cmd _cmd = undef;
  switch (_data)
  {
  case 0x40U:
    _cmd = dev_info;
    break;

  case 0x50U:
    _cmd = read_config;
    break;

  case 0x60U:
    _cmd = write_config;
    break;

  default:
    _cmd = undef;
    break;
  }
  return _cmd;
}

void get_dev_info(void)
{
  uint8_t _dev_info[8U];
  read_dev_info(_dev_info);
  for (uint8_t i = 0; i < 0x08U; ++i)
  {
    send_byte(_dev_info[i]);
  }
}

bool get_receive_config(uint8_t *pData, uint16_t *index, uint8_t Data)
{
  if (*index != CONFIG_SIZE - 1U)
  {
    pData[(*index)++] = Data;
    return false;
  }
  else
  {
    pData[*index] = Data;
    return true;
  }
}

void get_send_config(uint8_t *pData)
{
  if (read_config_packet(pData))
  {
    for (uint16_t i = 0; i < CONFIG_SIZE; ++i)
    {
      send_byte(pData[i]);
    }
  }
}

void send_ack(void)
{
  send_byte(0x55U);
}

void send_nack(void)
{
  send_byte(0xAAU);
}

void send_byte(uint8_t data)
{
  while ((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE)
  {
    asm("nop");
  }
  UART1->DR = data;
}

uint8_t get_crc(uint8_t *pData, uint16_t size)
{
  uint8_t crc = 0xFFU;
  for (uint8_t i = 0; i < size; ++i)
  {
    crc ^= pData[i];
  }
  return crc;
}

bool check_crc(uint8_t rCRC, uint8_t *pData, uint16_t size)
{
  return (rCRC == get_crc(pData, size));
}

void print(char *pData)
{
  char last = 0x00U, curr = 0x00U;
  uint8_t index = 0x00U;
  while (!(last == '\n' && curr == '\r') && !(last == '\r' && curr == '\n'))
  {
    last = curr;
    curr = pData[index++];
    send_byte(curr);
  }
}

void upd_config(void)
{
  uint16_t baud = 0x00U;
  baud = (FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t) * 3U) << 8);
  baud |= FLASH_ReadByte(EEPROM_INFO + sizeof(uint8_t) * 4U);
  set_baud(baud);
}

void print_cpu_id(void)
{
  for (uint32_t i = CPU_ID_BEGIN; i <= CPU_ID_END; ++i)
  {
    send_byte(FLASH_ReadByte(i));
  }
  send_byte('\n');
  send_byte('\r');
}
