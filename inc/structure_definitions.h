#ifndef _structure_definitions_h_
#define _structure_definitions_h_

#include "main.h"

struct lin_packet
{
  uint8_t PID;
  uint8_t size;     // User field for size detect
  uint8_t Data[MAX_SIZE_DATA_FRAME]; // Max data frame size is 8 bytes
  uint8_t CRC;
  uint8_t CounterDataFrame;
};

//enum filter_trig
//{
//  more,
//  less,
//  equal,
//  not_more,
//  not_less,
//  into,
//  not_equal,
//  none
//};

//struct filter_structure
//{
//  enum filter_trig filter_array[MAX_SIZE_DATA_FRAME];
//  uint8_t low_edges[MAX_SIZE_DATA_FRAME];
//  uint8_t high_edges[MAX_SIZE_DATA_FRAME];
//  uint8_t used_filters; // Used filters detected at bit fields
//};

//struct trig_struct
//{
//  struct lin_packet packet_0, packet_1;
//  struct filter_structure _filters;
//  uint8_t _parsed_PID;
//};

#endif