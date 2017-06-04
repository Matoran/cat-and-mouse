/*
 * mouse->c
 *
 *  Created on: 24 mai 2017
 *      Author: selement
 */
#include "mouse.h"
#include "lcd.h"
#include "random.h"
#include "define.h"
#include "task.h"
#include <stdio.h>

void init_mouse(mouse_t *mouse) {
	int random_pos = randBetween(0, 2);
	LPC_GPIO1->FIODIR &= ~(0b11111 << 19);
	mouse->object.pos.x = random_pos * 80 + 34;
	mouse->object.pos.y = 280;
	mouse->object.dir = NORTH;
	mouse->moving = false;
	mouse->vitality = VITALITYMAX;
	mouse->m = 0;
}

void reset_mouse(mouse_t *mouse) {
	int random_pos = randBetween(0, 2);
	mouse->object.pos.x = random_pos * 80 + 34;
	mouse->object.pos.y = 280;
	mouse->object.dir = NORTH;
	mouse->vitality = VITALITYMAX;
	mouse->m = 0;
}

/***********************************
 * function     : JoystickGetState
 * arguments    : pos (from enum)
 * return value : int
 *   est 	= p1.21	 -> 2
 *   ouest 	= p1.23	 -> 4
 ***********************************/
bool joystick_get_state(uint8_t pos) {
	if (((LPC_GPIO1->FIOPIN >> pos) & 1) == 1) {
		return false;
	} else {
		return true;
	}
}

void draw_mouse(mouse_t *old, mouse_t *new, sprites_t *sprites) {
	if (new->object.dir == old->object.dir) {
		switch (new->object.dir) {
		case NORTH:
			lcd_filled_rectangle(new->object.pos.x,
					new->object.pos.y
							+ sprites->mouse_im[new->object.dir].height,
					old->object.pos.x
							+ sprites->mouse_im[old->object.dir].width,
					old->object.pos.y
							+ sprites->mouse_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case EAST:
			lcd_filled_rectangle(new->object.pos.x, new->object.pos.y,
					old->object.pos.x,
					old->object.pos.y
							+ sprites->mouse_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case SOUTH:
			lcd_filled_rectangle(new->object.pos.x, new->object.pos.y,
					old->object.pos.x
							+ sprites->mouse_im[old->object.dir].width,
					old->object.pos.y,
					LCD_WHITE);
			break;
		case WEST:
			lcd_filled_rectangle(
					new->object.pos.x
							+ sprites->mouse_im[old->object.dir].width,
					new->object.pos.y,
					old->object.pos.x
							+ sprites->mouse_im[old->object.dir].width,
					old->object.pos.y
							+ sprites->mouse_im[old->object.dir].height,
					LCD_WHITE);
			break;
		default:
			break;
		}
	} else {
		lcd_filled_rectangle(old->object.pos.x, old->object.pos.y,
				old->object.pos.x + sprites->mouse_im[old->object.dir].width,
				old->object.pos.y + sprites->mouse_im[old->object.dir].height,
				LCD_WHITE);
	}

	display_bitmap16(sprites->mouse_im[new->object.dir].bitmap,
			new->object.pos.x, new->object.pos.y,
			sprites->mouse_im[new->object.dir].width,
			sprites->mouse_im[new->object.dir].height);
}

void task_mouse(void *param) {

	sprites_t *sprites = (sprites_t*) param;
	mouse_t mouse;
	init_mouse(&mouse);
	bool reset;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&xLastWakeTime, 20 / portTICK_RATE_MS);
		if (xQueueReceive(mouseResetQueue, &reset, 0)) {
			reset_mouse(&mouse);
		}
		mouse.m = (3 * mouse.vitality + VITALITYMAX / 2) / VITALITYMAX + 1;
		mouse.moving = true;
		if (joystick_get_state(JOYSTICK_LEFT)) {
			mouse.object.dir = WEST;
			if (mouse.object.pos.x - mouse.m > 0) {
				mouse.object.pos.x -= mouse.m;
			}
		} else if (joystick_get_state(JOYSTICK_RIGHT)) {
			mouse.object.dir = EAST;
			if (mouse.object.pos.x + mouse.m
					+ sprites->mouse_im[mouse.object.dir].width < 239) {
				mouse.object.pos.x += mouse.m;
			}
		} else if (joystick_get_state(JOYSTICK_TOP)) {
			mouse.object.dir = NORTH;
			if (mouse.object.pos.y - mouse.m > 26) {
				mouse.object.pos.y -= mouse.m;
			}
		} else if (joystick_get_state(JOYSTICK_BOTTOM)) {
			mouse.object.dir = SOUTH;
			if (mouse.object.pos.y + mouse.m < 280) {
				mouse.object.pos.y += mouse.m;
			}
		} else {
			mouse.moving = false;
		}

		if (!mouse.moving && mouse.vitality < VITALITYMAX) {
			mouse.vitality++;
		} else if (mouse.moving && mouse.vitality > 0) {
			mouse.vitality--;
		}
		xQueueSend(mouseQueue, (void * ) &mouse, (portTickType ) 0);
	}
}

