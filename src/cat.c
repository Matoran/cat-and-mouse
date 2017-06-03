/*
 * cat.c
 *
 *  Created on: Jun 2, 2017
 *      Author: matoran
 */

#include "cat.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "mouse.h"
#include <stdio.h>
#include <stdbool.h>

void draw_cat(cat_t *old, cat_t *new, sprites_t *sprites){
	if (new->object.dir == old->object.dir) {

		switch (new->object.dir) {
		case NORTH:
			lcd_filled_rectangle(
					new->object.pos.x,
					new->object.pos.y + sprites->cat_im[new->object.dir].height,
					old->object.pos.x + sprites->cat_im[old->object.dir].width,
					old->object.pos.y + sprites->cat_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case EAST:
			lcd_filled_rectangle(
					new->object.pos.x,
					new->object.pos.y,
					old->object.pos.x,
					old->object.pos.y + sprites->cat_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case SOUTH:
			lcd_filled_rectangle(
					new->object.pos.x,
					new->object.pos.y,
					old->object.pos.x + sprites->cat_im[old->object.dir].width,
					old->object.pos.y,
					LCD_WHITE);
			break;
		case WEST:
			lcd_filled_rectangle(
					new->object.pos.x + sprites->cat_im[old->object.dir].width,
					new->object.pos.y,
					old->object.pos.x + sprites->cat_im[old->object.dir].width,
					old->object.pos.y + sprites->cat_im[old->object.dir].height,
					LCD_WHITE);
			break;
		}
	} else {
		lcd_filled_rectangle(
				old->object.pos.x,
				old->object.pos.y,
				old->object.pos.x + sprites->cat_im[old->object.dir].width,
				old->object.pos.y + sprites->cat_im[old->object.dir].height,
				LCD_WHITE);
	}

	display_bitmap16(sprites->cat_im[new->object.dir].bitmap, new->object.pos.x,
			new->object.pos.y, sprites->cat_im[new->object.dir].width,
			sprites->cat_im[new->object.dir].height);
}

void init_cat(cat_t *cat){
	cat->object.pos.x = 110;
	cat->object.pos.y = 30;
	cat->object.dir = SOUTH;
}

void task_cat(void *param){
	portTickType xLastWakeTime = xTaskGetTickCount();
	cat_t cat;
	init_cat(&cat);
	int old_direction = cat.object.dir;
	while(1){
		vTaskDelayUntil(&xLastWakeTime, 50 / portTICK_RATE_MS);


		if (joystick_get_state(JOYSTICK_LEFT)) {
			old_direction = cat.object.dir;
			cat.object.dir = WEST;
			if (cat.object.pos.x - STEP_CAT > 0) {
				cat.object.pos.x -= STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_RIGHT)) {
			old_direction = cat.object.dir;
			cat.object.dir = EAST;
			if (cat.object.pos.x + STEP_CAT < MAX_POS_X-48) {
				cat.object.pos.x += STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_TOP)) {
			old_direction = cat.object.dir;
			cat.object.dir = NORTH;
			if (cat.object.pos.y - STEP_CAT > 26) {
				cat.object.pos.y -= STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_BOTTOM)) {
			old_direction = cat.object.dir;
			cat.object.dir = SOUTH;
			if (cat.object.pos.y + STEP_CAT < 252-48) {
				cat.object.pos.y += STEP_CAT;
			}
		}

		if (((old_direction == NORTH)||(old_direction == SOUTH)) && ((cat.object.dir == WEST)||(cat.object.dir == EAST))) {
			cat.object.pos.x -= 16;
			cat.object.pos.y += 16;
		}else if (((old_direction == WEST)||(old_direction == EAST)) && ((cat.object.dir == NORTH)||(cat.object.dir == SOUTH))) {
			cat.object.pos.x += 16;
			cat.object.pos.y -= 16;
		}

		xQueueSend(catQueue, ( void * ) &cat, (portTickType) 0);
	}
}

//void cat_move(){


	//init_dtmf();
	//cat_t cat = init_cat();

	//int pos;
	/*int buf;
	unsigned short *buf1 = init_dtmf();
	unsigned short *buf2 = buf1 + BUF_SIZE;*/

	//while(1){

		/*xQueueReceive(xQueue, &buf,portMAX_DELAY);

		if (buf == 0) {
			cat.oldDirection = cat.direction;
			cat.direction = direction(buf1);
		}else if (buf == 1){
			cat.oldDirection = cat.direction;
			cat.direction = direction(buf2);
		}*/

		/*if (joystick_get_state(JOYSTICK_LEFT)) {
			cat.oldDirection = cat.direction;
			cat.direction = WEST;
			if (cat.object.pos.x - STEP_CAT > 0) {
				cat.object.pos.x -= STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_RIGHT)) {
			cat.oldDirection = cat.direction;
			cat.direction = EAST;
			if (cat.object.pos.x + STEP_CAT < MAX_POS_X-48) {
				cat.object.pos.x += STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_TOP)) {
			cat.oldDirection = cat.direction;
			cat.direction = NORTH;
			if (cat.object.pos.y - STEP_CAT > 26) {
				cat.object.pos.y -= STEP_CAT;
			}
		}else if (joystick_get_state(JOYSTICK_BOTTOM)) {
			cat.oldDirection = cat.direction;
			cat.direction = SOUTH;
			if (cat.object.pos.y + STEP_CAT < 252-48) {
				cat.object.pos.y += STEP_CAT;
			}
		}

		if (((cat.oldDirection == NORTH)||(cat.oldDirection == SOUTH)) && ((cat.direction == WEST)||(cat.direction == EAST))) {
			cat.object.pos.x -= 16;
			cat.object.pos.y -= 16;
		}else if (((cat.oldDirection == WEST)||(cat.oldDirection == EAST)) && ((cat.direction == NORTH)||(cat.direction == SOUTH))) {
			cat.object.pos.x += 16;
			cat.object.pos.y += 16;
		}*/

		/*switch (cat.direction) {
			case NORTH:
				if (cat.object.pos.y - STEP > 26) {
					cat.object.pos.y -= STEP;
				}
				break;
			case EAST:
				if (cat.object.pos.x + STEP < MAX_POS_X-48) {
					cat.object.pos.x += STEP;
				}
				break;
			case SOUTH:
				if (cat.object.pos.y + STEP < 252-48) {
					cat.object.pos.y += STEP;
				}
				break;
			case WEST:
				if (cat.object.pos.x - STEP > 0) {
					cat.object.pos.x -= STEP;
				}
				break;
		}*/
		//xQueueSendToBack(catQueue, &cat, portMAX_DELAY);
	//}
//}
