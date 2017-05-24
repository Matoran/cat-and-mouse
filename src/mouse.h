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
#define VITALITYMAX 	 75

// structure for the mouse
typedef struct {
	int new_posx;
	int new_posy;
	int old_posx;
	int old_posy;
	int lenght;
	int width;
	int new_dir;
	int old_dir;
	int not_moving;
	int m;
	int vitality;
} mouse_t;

mouse_t mouse;

void init_mouse(sprites_t sprites);
void task_mouse(sprites_t sprites);

bool joystick_get_state(uint8_t pos);


#endif /* MOUSE_H_ */
