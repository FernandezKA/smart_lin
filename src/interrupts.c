#include "main.h"

uint32_t led_div = 0x00U;

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
    //vConfigLIN();
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
  UART1->SR = 0x00U;
  if(GetSize(&uart_tx) == 0x00U){
    //GetReset(&uart_tx);
    UART1->CR2 &= ~UART1_CR2_TIEN;
  }
  else{
    UART1->DR = Pull(&uart_tx);
  }
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  //Clear status flags
  UART1->SR = 0x00U;
  Push(&uart_rx, UART1->DR);
  //Get action on FSM receive
  //bLinPacketReceive(UART1->DR, &eLinReceive, &lin_rec);
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  if(led_div == 100000){
    led_div = 0x00U;
    LED_PORT->ODR^= LED_PIN;
  }
  else{
    led_div++;
  }
}

