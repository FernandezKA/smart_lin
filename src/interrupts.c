#include "main.h"

uint32_t led_div = 0x00U;
static bool div_tim2 = false;
uint8_t action_uart_timeout = 0x00U;

INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  if(((BTN_PORT->IDR) & BTN_0) != BTN_0){
    btn_0 = true;
  }
  else{
    btn_0 = false;
  }
  
  if(((BTN_PORT->IDR)&BTN_1) != BTN_1){
    btn_1 = true;
  }
  else{
    btn_1 = false;
  }
}


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
    action_uart_timeout = 0x04U;
    // vConfigLIN();
  }
  else
  { // Falling edge
    TIM1->CNTRL = 0x00U;
    TIM1->CNTRH = 0x00U;
    TIM1->CR1 |= TIM1_CR1_CEN;
    xBreak.break_fsm = increment;
  }
}

INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  UART1->SR = 0x00U;
  if (GetSize(&uart_tx) == 0x00U)
  {
    // GetReset(&uart_tx);
    UART1->CR2 &= ~UART1_CR2_TIEN;
  }
  else
  {
    UART1->DR = Pull(&uart_tx);
  }
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  // Clear status flags
  if((UART1->CR4 & UART1_CR4_LBDF) == UART1_CR4_LBDF){
    eLinReceive = wait_synch;
    UART1->CR4 &= ~UART1_CR4_LBDF;
    return;
  }
  else if((UART1->SR & UART1_SR_RXNE) == UART1_SR_RXNE){
    Push(&uart_rx, UART1->DR);
    UART1->SR = 0x00U;
  }
  action_uart_timeout = 0x0AU;
  // Get action on FSM receive
  // bLinPacketReceive(UART1->DR, &eLinReceive, &lin_rec);
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  if (led_div == 20000U)
  {
    led_div = 0x00U;
    //LED_PORT->ODR ^= LED_PIN;
  }
  else
  {
    led_div++;
  }
}

INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
  TIM2->SR1 = 0x00U;
  if(curr_mode == config){
    LED_PORT->ODR &= ~LED_PIN;
  }
  else if (curr_mode == work)
  {
    if(action_uart_timeout > 0x00U){
      action_uart_timeout--;
      LED_PORT->ODR^=LED_PIN;
    }
    else{
      if(div_tim2){
        LED_PORT->ODR^=LED_PIN;
      }
    }
  }
  
  if(div_tim2){
    ++sys_time;
  }
  
  (div_tim2) ? (div_tim2 = false) : (div_tim2 = true);
}