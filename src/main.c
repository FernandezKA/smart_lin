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
// struct queue_lin lin_queue;
struct filter loaded_filter;
bool btn_0 = false;
bool btn_1 = false;
uint32_t sys_time = 0x00U;
uint8_t trig_index = 0x00U;
bool isCRC = false;
uint8_t ex_pid_slave_ind, ex_pid_filter_ind, ex_pid_triggered_ind;
uint8_t out_time = 0x00U;


void SystemInit(void)
{
  CLK_Init();
  PORT_Init();
  UART_Init(9600U);
  // TIM1_Init();
  TIM2_Init();
  IRQ_Init();
  GetReset(&uart_rx);
  GetReset(&uart_tx);
  // return 0;
}

static inline void reset_state_cmd(bool *_cmd, enum cmd *_ecmd)
{
  *_cmd = false;
  *_ecmd = undef;
}

void main(void)
{
  SystemInit();
  // Variables init.
  static bool cmd_receive = false;
  //NB: The next string only for tests
  //curr_mode = config;
  curr_mode = GetDevMode();
  enum cmd curr_cmd = undef;
  uint16_t tmp_arr_index = 0x00U;
  uint8_t tmp_data;
  // Get select update
  (curr_mode == config) ? (config_uart()) : (config_lin());
  // Update list of exisiting PID from EEPROM config. file
  if (curr_mode == work)
  {
    read_packet(pid_slave_array, pid_filters_array);
  }

  while (true)
  {    
    // Check new bytes in FIFO ring buffer
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
        bLinPacketReceive(Pull(&uart_rx), &eLinReceive, &lin_rec);
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
            print("Lin smart device v. 0.3 \n\r");
            print("Release date: 2022-07-14\n\r");
            print("CPU ID: ");
            print_cpu_id();
            reset_state_cmd(&cmd_receive, &curr_cmd);
            GetReset(&uart_rx);
            break;

          case read_config:
            if (FLASH_ReadByte(EEPROM_START_PACKET) == 0x01)
            {
              print("ERROR: Reading forbidden\n\r");
            }
            else
            {
              get_send_config();
            }
            reset_state_cmd(&cmd_receive, &curr_cmd);
            GetReset(&uart_rx);
            break;

          case write_config:
            isCRC = false;
            if (GetSize(&uart_rx) == PACKET_SIZE)
            { // Get part of config array (1 packet)
              while (GetSize(&uart_rx) != 0x00U)
              {
                tmp_data = Pull(&uart_rx);
                if (get_receive_config(&tmp_arr_index, tmp_data))
                { // wait CRC, then check it
                  
                  uint8_t cCRC = 0x00U;
                  cCRC = get_crc(CONFIG_SIZE);
                  GetReset(&uart_rx);
                  send_write_end();
                  
                  // print("Wait CRC\n\r");
                  while (GetSize(&uart_rx) == 0x00U)
                  {
                    asm("nop"); // Wait CRC in polling mode
                  }
                  // print("Calculated CRC: ");
                  /*Warning! At current moment we skmip check CRC*/
                  if (Pull(&uart_rx) == cCRC)
                  //if (Pull(&uart_rx) == Pull(&uart_rx))
                  {                      
                    // CRC is valid
                    get_write_byte_eeprom(cCRC, EEPROM_START_PACKET + CONFIG_SIZE);
                    print("Download complete\n\r");
                    isCRC = true;
                  }
                  else
                  {
                    // CRC is invalid
                    print("Download error\n\r");
                    print("Calculated CRC: ");
                    from_hex_to_string(cCRC);
                    get_erase_config();
                    get_write_byte_eeprom(0x00U, EEPROM_START_PACKET + CONFIG_SIZE);
                    isCRC = true;
                  }
                  GetReset(&uart_rx);
                  reset_state_cmd(&cmd_receive, &curr_cmd);
                  tmp_arr_index = 0x00U;
                  // from_hex_to_string(cCRC);
                }
                else
                { // New byte is writed,
                  asm("nop");
                }
              }
              if (!isCRC)
              {
                send_write_end();
              }
              break;
            }
          }
        }
        break;

      default:

        break;
      }
    }

    /*Transmit data on IRQ with ring buffer */
    if (GetSize(&uart_tx) != 0x00U)
    {
      UART1->CR2 |= UART1_CR2_TIEN;
      UART1->DR = Pull(&uart_tx);
    }
    /* This FSM state on fully receive lin packet, check PID on list with SLAVE&&FILTERS
        then get action. If PID exist on 2 lists, used first finded value. */
    if (eLinReceive == completed)
    {
      // Get check PID in existing list
      //  Slave action, check rule
      if (search_pid(pid_filters_array, lin_rec.pid, &ex_pid_filter_ind)) // Trigger pid doesnt't exist, check this pid in rules
      {
        load_filter_packet(ex_pid_filter_ind, &loaded_filter);
        // if (get_check_filter(&lin_rec, &loaded_filter, get_btn0_state()))
        if (get_check_filter(&lin_rec, &loaded_filter))
        {
          // print("Rules trig\n\r");
          get_add_to_trig_list(pid_triggered_array, &trig_index, lin_rec.pid);
          vLinPacketClear(&lin_rec);
          eLinReceive = wait_break;
        }
        else
        {
          vLinPacketClear(&lin_rec);
          eLinReceive = wait_break;
          // print("Received packet isn't valid for current rules\n\r");
        }
      }
      else
      {
        vLinPacketClear(&lin_rec);
        eLinReceive = wait_break;
        // print("Current PID not exist\n\r");
        // asm("nop");
      }
    }
    // Check receive lin FSM state for complete packet

    // Check responce for PID request from master device
    if (wait_data_frame == eLinReceive)
    {
      if (search_pid(pid_triggered_array, lin_rec.pid, &ex_pid_triggered_ind))
      {
        struct lin sended_packet;
        /*Value for timeout consists in filter structure, then loaded filter structure value, then send time to parse*/
        if (search_pid(pid_slave_array, lin_rec.pid, &ex_pid_slave_ind))
        {
          load_slave_packet(ex_pid_slave_ind, &sended_packet);
          if(search_pid(pid_filters_array, lin_rec.pid, &ex_pid_filter_ind)){
            load_filter_packet(ex_pid_filter_ind, &loaded_filter);
          }
          else{
            vLinPacketClear(&lin_rec);
            eLinReceive = wait_break;
            error_handler();
          }
          // CHECK time
          if ((uint32_t)loaded_filter.timeout < sys_time)
          {
            search_pid(pid_filters_array, lin_rec.pid, &ex_pid_slave_ind); // Check time and remove flag in filter structure
            load_filter_packet(ex_pid_slave_ind, &loaded_filter);
            //TODO: Add output behaviour!!!
            if (loaded_filter.remove_after_use)
            {
              get_send_data_frame(&sended_packet);
              get_remove_pid(pid_triggered_array, lin_rec.pid);
            }
            else
            {
              get_send_data_frame(&sended_packet);
            }
          }

          //get_remove_pid(pid_triggered_array, lin_rec.pid);
          vLinPacketClear(&lin_rec);
          eLinReceive = wait_break;
        }
        // If timeout less then sys_time - send packet, else remove it
      }
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif

void error_handler(void){
  asm("nop");
}
