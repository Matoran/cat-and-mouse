#ifndef DTMF_DETECTION_H
#define DTMF_DETECTION_H

#include "objects.h"
#include "define.h"

typedef struct complex{
	float re;
	float im;
}complex;

typedef struct cat_t{
	int posX;
	int posY;
	int direction;
	int oldDirection;
}cat_t;

unsigned short* init_dtmf();
dir_t read_dtmf(int buf_idx);

#endif
