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
	int oldPosX;
	int oldPoxY;
}cat_t;

typedef enum Direction Direction;
enum Direction
{
    NORTH, EAST, SOUTH,WEST
};

void init_dtmf();
dir_t read_dtmf(int buf_idx);

#endif
