#include "stm8s_conf.h"
#include "main.h"

// User global variables
struct Break xBreak;
extern uint32_t BAUDRATE;

int SystemInit(void)
{
  return 0;
}

void main(void)
{
  SystemInit();
  while (1)
  {
    if (xBreak.break_fsm == detect_rise)
    {
      if (bLinCheckBreak(&xBreak))
      {
        // switch lin receive fsm state
      }
      else
      {
        // Reset break fsm
        xBreak.break_fsm = wait_fall;
      }
    }
    //Check receive lin FSM state
    if()

  };
}
