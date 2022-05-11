#include "packet_parser.h"

extern enum Receive_FSM eLinReceive;

bool bLinPacketReceive(uint8_t _data, enum Receive_FSM *_rec_fsm, struct lin_packet *_packet)
{
  switch (*_rec_fsm)
  {
  case wait_break:
    // This case handled on IRQ handler (pait of EXTI + TIM4)

    return false;
    break;

  case wait_synch:
    if (_data == 0x55U)
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
    // Right now not detect parity error
    _packet->PID = _data & 0x3FU;
    _packet->size = u8GetSizeDataFrame(*_packet);
    return false;
    break;

  case wait_data_frame:
    if (_packet->CounterDataFrame < _packet->size)
    {
      _packet->Data[_packet->CounterDataFrame++] = _data;
    }
    else
    {
      _packet->Data[_packet->CounterDataFrame++] = _data;
      _packet->CounterDataFrame = 0x00U;
      *_rec_fsm = wait_CRC;
    }
    return false;
    break;

  case wait_CRC:
    if (u8GetCRC(*_packet) == _data)
    {
      _packet->CRC = _data;
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