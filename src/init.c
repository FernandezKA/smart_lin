#include "init.h"

void CLK_Init(void)
{
  // USE INTERNAL RC OSCILLATOR 16 MHz
  CLK->CKDIVR = 0x00;
}

void UART_Init(uint16_t BAUD)
{
  uint16_t prescaler = F_CPU / BAUD;
  UART1->BRR1 = (prescaler & 0xFF0) >> 4;
  UART1->BRR2 = ((prescaler & 0xF000) >> 4) | (prescaler & 0x0F);
  UART1->CR2 |= UART1_CR2_RIEN | UART1_CR2_REN | UART1_CR2_TEN;
}

void LIN_Init(void)
{
  UART1->CR3 |= UART1_CR3_LINEN;
}

void PORT_Init(void)
{
  // GPIOD->CR2 |= (1U<<5);//ENABLE EXTERNAL IRQ FOR UART_TX (RECONFIG ON UART WORK MODE, USED FOR BREAK DETECTION //REPLACE ON LIN_CONFIG
  EXTI->CR1 |= (1U << 7 | 1U << 6); // RISE AND FALL DETECTION ON PORTB
  LED_PORT->DDR |= LED_PIN;
}

void IRQ_Init(void)
{
  asm("rim");
}

void TIM1_Init(void)
{
  TIM1->CR1 |= TIM1_CR1_OPM;
  TIM1->PSCRL = (uint8_t)(((F_CPU) / 9600U) & 0xFF);
  TIM1->PSCRH = (uint8_t)((((F_CPU) / 9600U) & 0xFF00) >> 8);
}

void TIM4_Init(void)
{
  TIM4->PSCR |= (1U << 2 | 1U << 1 | 1U << 0); // PSCR 32768
  TIM4->ARR = 0xFFU;
  TIM4->IER |= TIM4_IER_UIE;
  TIM4->CR1 |= TIM4_CR1_CEN;
}

void config_uart(void)
{
  GPIOD->CR2 &= ~(1U << 5); // Disable external IRQ
  UART1->CR2 |= UART1_CR2_RIEN | UART1_CR2_REN | UART1_CR2_TEN;
}

void config_lin(void)
{
  UART1->CR2 &= ~(UART1_CR2_RIEN | UART1_CR2_REN | UART1_CR2_TEN);
  GPIOD->CR2 |= (1U << 5);
}

void set_baud(uint16_t _baud)
{
  uint16_t prescaler = F_CPU / _baud;
  UART1->BRR1 = (prescaler & 0xFF0) >> 4;
  UART1->BRR2 = ((prescaler & 0xF000) >> 4) | (prescaler & 0x0F);
}
