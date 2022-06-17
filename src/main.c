#include "stm8s_conf.h"
#include "main.h"
#include "init.h"



// User global variables
struct Break xBreak;
extern uint32_t BAUDRATE;
enum Receive_FSM eLinReceive;
enum mode curr_mode = work;
FIFO uart_rx, uart_tx;
struct lin lin_rec;
struct lin lin_tr;

int SystemInit(void)
{
  CLK_Init();
  PORT_Init();
  UART_Init(9600U);
  TIM1_Init();
  // TIM4_Init();
  IRQ_Init();
  GetReset(&uart_rx);
  GetReset(&uart_tx);
  return 0;
}

static inline void reset_state_cmd(bool *_cmd, enum cmd *_ecmd)
{
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
  uint8_t tmp_data;
  (curr_mode == config) ? (config_uart()) : (config_lin());
  if(curr_mode == work){ //Upd list with PIDs
    read_packet(pid_slave_array, pid_filters_array);
  }
  while (1)
  {
    if (GetSize(&uart_rx) != 0x00U)
    { // receive new data
      switch (curr_mode)
      { // For received lin packets
      case work:
        /* main logic the next:
          1. receive lin packet
          2. check pid of lin packet
          3. if pid compare is equal - get send_slave or get filter_action
          */
           
        break;

      case config:
      /*
        In this case loaded config with parameters for work of dev
        Exist commands:
          1. 0x40 0x00 - get out info about device
          2. 0x50 0x00 - read configuration
          3. 0x60 + configuration packets - write configuration
      */
        if (!cmd_receive)
        {
          curr_cmd = get_command(Pull(&uart_rx));
          if (undef != curr_cmd)
          {
            cmd_receive = true;
          }
        }
        else
        {
          switch (curr_cmd)
          {
          case dev_info:
            get_dev_info();
            reset_state_cmd(&cmd_receive, &curr_cmd);
            GetReset(&uart_rx);
            break;

          case read_config:
            if (read_config_packet(configArray))
            {
              get_send_config(configArray);
            }
            else
            {
              print("Config. not exist\n\r");
            }
            reset_state_cmd(&cmd_receive, &curr_cmd);
            GetReset(&uart_rx);
            break;

          case write_config:
            tmp_data = Pull(&uart_rx);
            if (get_receive_config(configArray, &tmp_arr_index, tmp_data))
            { // Received packet, wait CRC
              GetReset(&uart_rx);
              send_nack();
              print("wait crc\n\r");
              while (GetSize(&uart_rx) == 0x00)
              {
                asm("nop");
              }
              tmp_data = Pull(&uart_rx); // Receive CRC
              if (check_crc(tmp_data, configArray, tmp_arr_index))
              { // CRC is valid
                write_config_packet(configArray, tmp_arr_index);
                // upd_config();
                print("Load complete\n\r");
                reset_state_cmd(&cmd_receive, &curr_cmd);
              }
              else
              {
                print("Invalid CRC\n\r");
              }
              reset_state_cmd(&cmd_receive, &curr_cmd);
              tmp_arr_index = 0x00U;
              GetReset(&uart_rx);
            }
            break;
          }
        }
        break;
      }
    }

    // Transmit data on IRQ
    if (GetSize(&uart_tx) != 0x00U)
    {
      UART1->CR2 |= UART1_CR2_TIEN;
      UART1->DR = Pull(&uart_tx);
    }

    // Switch state on receive fsm
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
    // Lin packet received
    if (eLinReceive == completed)
    {
      static uint8_t ex_pid_slave_ind, ex_pid_filter_ind;
      if(search_pid(pid_slave_array, lin_rec.pid, &ex_pid_slave_ind)){
        load_slave_packet(ex_pid_slave_ind, &lin_rec);
      }
      else if(search_pid(pid_filters_array, lin_rec.pid, &ex_pid_slave_ind)){

      }
      else{
        
      }
    }
    // Check receive lin FSM state for complete packet
  };
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif
