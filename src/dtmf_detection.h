#ifndef DTMF_DETECTION_H
#define DTMF_DETECTION_H

#include "objects.h"
#include "define.h"

#define BUF_SIZE 400
#define LIMIT_DETEC 2000000
#define FE 8000.
#define PI 3.14159265359
#define FREQ1 852
#define FREQ2 941
#define FREQ3 1336
#define FREQ4 1608

#define abs1(x) ((x<0)?(-x):x)


typedef struct complex{
	float re;
	float im;
}complex;

int direction (uint16_t* buf);
unsigned short* init_dtmf();
dir_t read_dtmf(int buf_idx);

#endif
