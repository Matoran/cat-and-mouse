/*
 * mouse.h
 *
 *  Created on: 24 mai 2017
 *      Author: selement
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "objects.h"

#define JOYSTICK_LEFT	 23
#define JOYSTICK_RIGHT	 21
#define JOYSTICK_TOP	 20
#define JOYSTICK_BOTTOM	 22
#define JOYSTICK_CENTER	 19
#define VITALITYMAX 	 75
#define NORTH			 0
#define EAST			 1
#define SOUTH			 2
#define WEST			 3

// structure for the mouse
typedef struct {
	object_t object;
	int lenght;
	int width;
	int m;
	int vitality;
	int score;
	bool moving;
} mouse_t;

void init_mouse(mouse_t *mouse);
void task_mouse(void *param);
void draw_mouse(mouse_t *old, mouse_t *new, sprites_t *sprites);

bool joystick_get_state(uint8_t pos);


#endif /* MOUSE_H_ */
