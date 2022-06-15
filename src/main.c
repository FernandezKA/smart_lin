#include "stm8s_conf.h"
#include "main.h"
#include "init.h"

// User global variables
struct Break xBreak;
extern uint32_t BAUDRATE;
enum Receive_FSM eLinReceive;
struct lin_packet lin_rec;
enum mode curr_mode = work; 
FIFO uart_rx, uart_tx;

int SystemInit(void)
{
  CLK_Init();
  PORT_Init();
  UART_Init(9600U);
  TIM1_Init();
  IRQ_Init();
  GetReset(&uart_rx);
  GetReset(&uart_tx);
  return 0;
}

static inline void reset_state_cmd(bool* _cmd, enum cmd* _ecmd){
  *_cmd = false; 
  *_ecmd = undef;
}

void main(void)
{
  bool cmd_receive = false;
  SystemInit();
  curr_mode = GetDevMode();
  enum cmd curr_cmd = undef;
  uint16_t tmp_arr_index = 0x00U;
  (curr_mode == config) ? (config_uart()) : (config_lin());
  while (1)
  {
    if(GetSize(&uart_rx) != 0x00U){ //receive new data
        switch(curr_mode){
          case work:
    
            break;
    
          case config:
            curr_cmd = get_command(Pull(&uart_rx));
            if(!cmd_receive){
              if(undef != curr_cmd){
                cmd_receive = true;
                switch(curr_cmd){
                case dev_info:
                    get_dev_info(&uart_tx);
                  break;
                  
                case read_config:
                  if(read_config_packet(configArray)){
                    get_send_config(configArray);
                  }
                  else{
                    print("Config. not exist\n\r");
                  }
                  break;
                  
                case write_config:
                  if(get_receive_config(configArray, &tmp_arr_index, Pull(&uart_tx))){ //Received packet, wait CRC
                    if(check_crc(Pull(&uart_tx), configArray, tmp_arr_index)){ //CRC is valid
                      write_config_packet(configArray, tmp_arr_index);
                      upd_config(); 
                    }
                    else{
                      print("Invalid CRC\n\r");
                    }
                    reset_state_cmd(&cmd_receive, &curr_cmd);
                  }
                  break;
                }
              }
              else{
                reset_state_cmd(&cmd_receive, &curr_cmd);
              }
            }
            break;
      }
    }
    
    //Transmit data on IRQ
    if(GetSize(&uart_tx) != 0x00U){
      UART1->CR2 |= UART1_CR2_TIEN;
      UART1->DR = Pull(&uart_tx);
    }
    else{
      UART1->CR2 &= ~UART1_CR2_TIEN;
    }
    
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

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif
