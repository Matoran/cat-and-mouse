/**
 * @authors: LOPES Marco, ISELI Cyril and RINGOT Gaëtan
 * Purpose: All function for the cat
 * Language:  C
 * Date : june 2017
 */

#include "cat.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "mouse.h"
#include "dtmf_detection.h"
#include <stdio.h>
#include <stdbool.h>
#include "dtmf_detection.h"
#include "define.h"

#define BUF_SIZE 400

/**
 * Draw cat this function is call in the task display
 * @param old cat with precedent position
 * @param new cat with the new position
 * @param sprites image of cat
 */
void draw_cat(cat_t *old, cat_t *new, sprites_t *sprites) {
	if (new->object.dir == old->object.dir) {
		switch (new->object.dir) {
		case NORTH:
			lcd_filled_rectangle(new->object.pos.x,
					new->object.pos.y + sprites->cat_im[new->object.dir].height,
					old->object.pos.x + sprites->cat_im[old->object.dir].width,
					old->object.pos.y + sprites->cat_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case EAST:
			lcd_filled_rectangle(new->object.pos.x, new->object.pos.y,
					old->object.pos.x,
					old->object.pos.y + sprites->cat_im[old->object.dir].height,
					LCD_WHITE);
			break;
		case SOUTH:
			lcd_filled_rectangle(new->object.pos.x, new->object.pos.y,
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
		lcd_filled_rectangle(old->object.pos.x, old->object.pos.y,
				old->object.pos.x + sprites->cat_im[old->object.dir].width,
				old->object.pos.y + sprites->cat_im[old->object.dir].height,
				LCD_WHITE);
	}

	display_bitmap16(sprites->cat_im[new->object.dir].bitmap, new->object.pos.x,
			new->object.pos.y, sprites->cat_im[new->object.dir].width,
			sprites->cat_im[new->object.dir].height);
}

/**
 * Initialize the cat
 * @param cat structure cat
 */
void init_cat(cat_t *cat) {
	cat->object.pos.x = 110;
	cat->object.pos.y = 30;
	cat->object.dir = SOUTH;
}

/**
 * This is the routine for the cat
 * @param param sprites for the cat
 */
void task_cat(void *param) {
	sprites_t *sprites = (sprites_t*) param;
	portTickType xLastWakeTime = xTaskGetTickCount();
	cat_t cat;
	init_cat(&cat);
	int old_direction = cat.object.dir;
	int dir;
	//for the rotation of cat
	int x = (sprites->cat_im[WEST].width - sprites->cat_im[NORTH].width) / 2;
	int y = (sprites->cat_im[NORTH].height - sprites->cat_im[WEST].height) / 2;

	int buf;
	unsigned short *buf1 = init_dtmf();
	unsigned short *buf2 = buf1 + BUF_SIZE;
	bool reset;
	while (1) {
		vTaskDelayUntil(&xLastWakeTime, 50 / portTICK_RATE_MS);
		if (xQueueReceive(catResetQueue, &reset, 0)) {
			init_cat(&cat);
		}
		old_direction = cat.object.dir;
		//Get the direction
		if (xQueueReceive(xQueue, &buf, 0)) {
			if (buf == 0) {
    			dir = direction(buf1);
    			if (dir != -1) {
    				cat.object.dir = dir;
    			}
    		}else if (buf == 1){
    			dir = direction(buf2);
    			if (dir != -1) {
    				cat.object.dir = dir;
    			}
    		}
		}

		switch (cat.object.dir) {
			case NORTH:
				if (old_direction == WEST || old_direction == EAST) {
					cat.object.pos.x += x;
					cat.object.pos.y -= y;
				}
				if (cat.object.pos.y - STEP_CAT > 26) {
					cat.object.pos.y -= STEP_CAT;
				}
				break;
			case SOUTH:
				if (old_direction == WEST || old_direction == EAST) {
					cat.object.pos.x += x;
					cat.object.pos.y -= y;
				}
				if (cat.object.pos.y + STEP_CAT < 252 - 48) {
					cat.object.pos.y += STEP_CAT;
				}
				break;
			case WEST:
				if (old_direction == NORTH || old_direction == SOUTH) {
					cat.object.pos.x -= x;
					cat.object.pos.y += y;
				}
				if (cat.object.pos.x - STEP_CAT > 0) {
					cat.object.pos.x -= STEP_CAT;
				}
				break;
			case EAST:
				if (old_direction == NORTH || old_direction == SOUTH) {
					cat.object.pos.x -= x;
					cat.object.pos.y += y;
				}
				if (cat.object.pos.x + STEP_CAT < MAX_POS_X - 48) {
					cat.object.pos.x += STEP_CAT;
				}
				break;
			default:
				break;
		}
		xQueueSend(catQueue, (void * ) &cat, (portTickType ) 0);
	}
}

