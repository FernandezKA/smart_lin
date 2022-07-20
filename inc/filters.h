#ifndef _filters_h_
#define _filters_h_

#include "main.h"

extern uint8_t pid_slave_array[COUNT_PACKET];
extern uint8_t pid_filters_array[COUNT_PACKET];
extern uint8_t pid_triggered_array[COUNT_PACKET];

// Check packet type from config. array
enum packet_type get_check_packet_type(uint8_t *pData);
// Check pid valid
bool get_check_pid(uint8_t _pid);
// Add to array with PID new value
void add_to_array(uint8_t *pData, uint8_t index, uint8_t pid);
// Increment addr (right now not used)
void get_next_cell(uint8_t *index); //(?) This function may be removed from proj.
// Check pid value on array
bool search_pid(uint8_t *pArr, uint8_t PID, uint8_t *index);
// Parse packet from array to struct
void load_slave_packet(uint8_t index, struct lin *packet);
// Parse packet from array to struct
void load_filter_packet(uint8_t index, struct filter *packet);
// This function compare received packet with rules
bool get_check_filter(struct lin *packet_lin, struct filter *packet_filter);
// This function add current PID to list with searched
void get_add_to_trig_list(uint8_t *pArray, uint8_t *index, uint8_t pid);
// This function clear pid from array
void get_remove_pid(uint8_t *pArray, uint8_t pid);
// This function return current button state
bool get_btn0_state(void);
bool get_btn1_state(void);
bool get_btn_state(uint8_t state);

void get_out_action(enum out_state state, uint8_t timeout_value);
#endif