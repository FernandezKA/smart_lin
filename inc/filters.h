#ifndef _filters_h_
#define _filters_h_

#include "main.h"

struct filter{
  uint8_t low_edge; 
  uint8_t high_edge;
  enum{
    all, 
    more_then, 
    less_then, 
    in_edges, 
    more_or_equal, 
    less_or_equal, 
    in_edges_with_edge, 
    not_equal
  } rule;
};

struct filter_rule{
  uint8_t PID;
  struct filter array[8U];
  bool discrete_input; 
};

bool bCheckRule(uint8_t _data, struct filter* _filter);

bool bCheckFilterEquation(struct lin_packet* _packet, struct filter_rule* _rule);

#endif