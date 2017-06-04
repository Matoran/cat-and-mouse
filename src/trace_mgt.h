/*
 * Description: trace management using UART
 * Created on : 30 sept. 2013
 * Author     : VP
 */
#ifndef TRACE_MGT_H_
#define TRACE_MGT_H_

#include <stdio.h>
#include <stdbool.h>
#include "traces_ref.h"

#define BUFFER_MAX 50


typedef struct {
	int read_pos;
	int write_pos;
	trace_t trace[BUFFER_MAX * sizeof(trace_t)];
} buffer_trace;

/* uses timer 2 */
//void init_traces();
void write_trace(uint8_t sig_idx, short val);
void task_traces();



#endif
