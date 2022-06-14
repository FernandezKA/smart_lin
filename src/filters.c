#include "filters.h"

//bool bCheckFilterEquation(struct lin_packet* _packet, struct filter_rule* _rule){
//  bool state = false;
//  if(_packet->PID != _rule->PID){
//    state = false;
//  }
//  else{
//    bool edge_state = true;
//    for(uint8_t i = 0; i < 0x08U; ++i){
//      edge_state&= bCheckRule(_packet->Data[i], &_rule->array[i]);
//    }
//    state = edge_state;
//  }
//  return state;
//}
//
// bool bCheckRule(uint8_t _data, struct filter* _filter){
//   bool state = false;
//    switch(_filter->rule){
//    case all:
//      state = true;
//      break;
//      
//    case more_then: 
//      state = (bool) (_data > _filter->low_edge);
//      break;
//      
//    case less_then:
//      state = (bool) (_data < _filter->low_edge);
//      break;
//      
//    case in_edges:
//      state = (bool) (_data > _filter->low_edge && _data < _filter->high_edge);
//      break;
//      
//    case more_or_equal:
//      state = (bool) (_data >= _filter->low_edge);
//      break;
//      
//    case less_or_equal:
//      state = (bool) (_data <= _filter->low_edge);
//      break;
//      
//    case in_edges_with_edge:
//      state = (bool) (_data >= _filter->low_edge && _data <= _filter->high_edge);
//      break;
//      
//    case not_equal:
//      state = (bool) (_data != _filter->low_edge);
//      break;
//    }
//    return state;
//  }