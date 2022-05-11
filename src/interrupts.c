#include "main.h"

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  // Check line state
  if ((GPIOD->IDR & (1U << 6)) == (1 << 6))
  { // Rising edge
    uint16_t _cnt_val = 0x00U;
    _cnt_val = TIM1->CNTRH << 8;
    _cnt_val |= TIM1->CNTRL;
    xBreak.break_fsm = detect_rise;
  }
  else
  { // Falling edge
    TIM1->CNTRH = 0x00U;
    TIM1->CNTRL = 0x00U;
    xBreak.break_fsm = increment;
  }
}

INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  while (1)
  {
  };
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
}