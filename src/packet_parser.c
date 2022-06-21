#include "packet_parser.h"

extern enum Receive_FSM eLinReceive;

bool bLinPacketReceive(uint8_t _data, enum Receive_FSM *_rec_fsm, struct lin *_packet)
{
  switch (*_rec_fsm)
  {
  case wait_break:
    // This case handled on IRQ handler (part of EXTI + TIM4)

    return false;
    break;

  case wait_synch:
    if (_data == SYNCH_FIELD)
    {
      *_rec_fsm = wait_PID;
    }
    else
    {
      *_rec_fsm = wait_break;
    }
    return false;
    break;

  case wait_PID:
    // Right now I not detect parity error
    _packet->pid = _data & 0x3FU;
    _packet->dlc = u8GetSizeDataFrame(*_packet);
    *_rec_fsm = wait_data_frame;
    return false;
    break;

  case wait_data_frame:
    if (_packet->cnt_receive < _packet->dlc)
    {
      _packet->data[_packet->cnt_receive++] = _data;
    }
    else
    {
      _packet->data[_packet->cnt_receive++] = _data;
      _packet->cnt_receive = 0x00U;
      *_rec_fsm = wait_CRC;
    }
    return false;
    break;

  case wait_CRC:
    if (u8GetCRC(*_packet) == _data)
    {
      _packet->crc = _data;
      *_rec_fsm = completed;
    }
    else
    {
      *_rec_fsm = wait_break;
      vLinPacketClear(_packet);
    }
    return false;
    break;

  case completed:

    return true;
    break;

  default:

    return false;
    break;
  }
}