#include "main.h"

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  // Check line state
  if ((GPIOD->IDR & (1U << 5)) == (1 << 5))
  { // Rising edge
    uint16_t _cnt_val = 0x00U;
    _cnt_val = TIM1->CNTRH << 8;
    _cnt_val |= TIM1->CNTRL;
    xBreak.u16Counter = _cnt_val;
    xBreak.break_fsm = detect_rise;
    vConfigLIN();
  }
  else
  { // Falling edge
    TIM1->CNTRL = 0x00U;
    TIM1->CNTRH = 0x00U;
    TIM1->CR1|=TIM1_CR1_CEN;
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
  //Clear status flags
  UART1->SR = 0x00U;
  //Get action on FSM receive
  bLinPacketReceive(UART1->DR, &eLinReceive, &lin_rec);
}