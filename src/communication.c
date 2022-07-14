#include "communication.h"
#include "eeprom.h"
#include "init.h"

//uint8_t configArray[CONFIG_SIZE];

enum mode GetDevMode(void)
{
  enum mode _mode;
  (((MODE_PORT->IDR) & MODE_PIN) != MODE_PIN) ? (_mode = config) : (_mode = work);
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
    get_erase_config();
    send_write_end();
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

bool get_receive_config(uint16_t *index, uint8_t Data)
{
  if (*index != CONFIG_SIZE - 1U)
  {
    //TODO: rewrite adding to eeprom without heap 
    get_write_byte_eeprom(Data, ((*index)++) + EEPROM_START_PACKET);
    //pData[(*index)++] = Data;
    return false;
  }
  else
  {
    //pData[*index] = Data;
    get_write_byte_eeprom(Data, (*index) + EEPROM_START_PACKET);
    return true;
  }
}

void get_send_config(void)
{
    for (uint16_t i = 0; i <= CONFIG_SIZE; ++i)
    {
      send_byte(FLASH_ReadByte(i + EEPROM_START_PACKET));
      //send_byte(pData[i]);
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

void send_write_end(void){
  send_byte(0x65U);
}

void send_byte(uint8_t data)
{
  while ((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE)
  {
    asm("nop");
  }
  UART1->DR = data;
}

uint8_t get_crc(uint16_t size)
{
  uint8_t crc = 0xFFU;
  for(uint16_t i = 0; i < size; ++i){
     crc^=FLASH_ReadByte(EEPROM_START_PACKET + (uint32_t) i);
     for(uint8_t j = 0; j < 8U; ++j){
      crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
     }
  }
  return crc;
}

bool check_crc(uint8_t rCRC, uint16_t size)
{
  return (rCRC == get_crc(size));
}

void print(char *pData)
{
  uint8_t index = 0x00U;
  while(index < 0xFFU & pData[index] != '\0'){
    send_byte(pData[index++]);
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
    from_hex_to_string(FLASH_ReadByte(i));
  }
  send_byte('\n');
  send_byte('\r');
}

void from_hex_to_string(uint8_t data){
  uint8_t first_digit = 0x00U, second_digit = 0x00U;
  (((data & 0xF0) >> 4) >= 0x0AU) ? (first_digit = (((data & 0xF0) >> 4)%0x0A) + 'A') : (first_digit = ((data & 0xF0) >> 4) + '0');
  ((data & 0x0F) >= 0x0AU) ? (second_digit = ((data & 0x0F)%0x0A) + 'A') : (second_digit = (data & 0x0F) + '0');
  send_byte(first_digit); 
  send_byte(second_digit);
}

void get_erase_config(void)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  for(uint16_t i = 0; i < CONFIG_SIZE; ++i){
    FLASH_EraseByte(EEPROM_START_PACKET + (uint32_t) i);
  }
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

