#include "lin_hw_layer.h"
#include "main.h"

void vSetBAUD(uint16_t _BAUD)
{
#ifndef F_CPU
#error F_CPU unknowledge
#endif

#ifndef LIN_UART
#error hw uart for lin not selected
#endif

  uint16_t USART_DIV = F_CPU / _BAUD;
  // Set baudrate
  LIN_UART->BRR1 = (uint8_t)((USART_DIV >> 8) & 0xFF);
  LIN_UART->BRR2 = (uint8_t)(((uint8_t)(USART_DIV & 0xF000) >> 8U) | (USART_DIV & 0x0F));
}

void vSetBreakLength(uint16_t _BAUD)
{
}

uint16_t u16GetBAUD(void)
{

  return u16BAUD;
}

uint8_t u8GetCRC(struct lin_packet _packet)
{
  static uint32_t _sum = 0x00U;
  static uint8_t CRC = 0x00U;
  for (uint8_t i = 0; i < _packet.size; ++i)
  {
    _sum += _packet.Data[i];
  }
  CRC = (uint8_t)(_sum & 0xFFU) ^ 0xFFU;
  return CRC;
}

void vLinPacketClear(struct lin_packet *_packet)
{
  _packet->PID = 0x00U;
  _packet->CRC = 0x00U;
  for (uint8_t i = 0; i < 0x08U; ++i)
  {
    _packet->Data[i] = 0x00U;
  }
  _packet->CounterDataFrame = 0x00U;
}

uint8_t u8GetSizeDataFrame(struct lin_packet _packet)
{
  if (_packet.PID < 0x20U)
  {
    return 0x02U;
  }
  else if (_packet.PID < 0x30U)
  {
    return 0x04U;
  }
  else
  {
    return 0x08U;
  }
}

bool bLinCheckBreak(struct Break *_break)
{
  if (_break->break_fsm == detect_rise)
  {
    if (_break->u16Counter > 0x08U && _break->u16Counter < 0x0CU)
    {
      return true;
    }
    else
    {
      _break->u16Counter = 0x00U;
      _break->break_fsm = wait_fall;
      return false;
    }
  }
  else
  {
    return false;
  }
}

void vConfigLIN(void){
   GPIOD->CR2 &= ~(1U<<5);//DISABLE EXTERNAL IRQ FOR UART_TX (RECONFIG ON UART WORK MODE, USED FOR BREAK DETECTION
}

void vConfigBreak(void){
    GPIOD->CR2 |= (1U<<5);//ENABLE EXTERNAL IRQ FOR UART_TX (RECONFIG ON UART WORK MODE, USED FOR BREAK DETECTION
}