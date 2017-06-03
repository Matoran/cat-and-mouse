#ifndef DTMF_DETECTION_H
#define DTMF_DETECTION_H

#include "objects.h"
#include "define.h"
#define BUF_SIZE 400

typedef struct complex{
	float re;
	float im;
}complex;

unsigned short* init_dtmf();
dir_t read_dtmf(int buf_idx);
int direction (uint16_t* buf);

#endif
