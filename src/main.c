#include "stm8s_conf.h"
#include "main.h"
#include "init.h"

// User global variables
struct Break xBreak;
extern uint32_t BAUDRATE;
enum Receive_FSM eLinReceive;
struct lin_packet lin_rec;

int SystemInit(void)
{
  CLK_Init();
  PORT_Init();
  UART_Init(9600U);
  TIM1_Init();
  IRQ_Init();
  return 0;
}

void main(void)
{
  SystemInit();
  while (1)
  {
    //TODO: ADD RECEIVE FROM SOFT USART
    
    
    //Switch state on receive fsm 
    if (xBreak.break_fsm == detect_rise)
    {
      if (bLinCheckBreak(&xBreak))
      {
        // switch lin receive fsm state
        eLinReceive = wait_synch;
      }
      else
      {
        // Reset break fsm
        xBreak.break_fsm = wait_fall;
        eLinReceive = wait_break;
      }
    }
    //Lin packet received
    if(eLinReceive == completed){ 
      //TODO: Add parser for received structure
      
    }
    //Check receive lin FSM state for complete packet

  };
}
