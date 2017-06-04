/**
 * @authors: LOPES Marco, ISELI Cyril and RINGOT Gaëtan
 * Purpose: cat
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

void init_cat(cat_t *cat) {
	cat->object.pos.x = 110;
	cat->object.pos.y = 30;
	cat->object.dir = SOUTH;
}

void task_cat(void *param) {
	sprites_t *sprites = (sprites_t*) param;
	portTickType xLastWakeTime = xTaskGetTickCount();
	cat_t cat;
	init_cat(&cat);
	int old_direction = cat.object.dir;
	int x = (sprites->cat_im[WEST].width - sprites->cat_im[NORTH].width) / 2;
	int y = (sprites->cat_im[NORTH].height - sprites->cat_im[WEST].height) / 2;
	;

	int dir;
	int buf;
	unsigned short *buf1 = init_dtmf();
	unsigned short *buf2 = buf1 + BUF_SIZE;
	while (1) {
		vTaskDelayUntil(&xLastWakeTime, 50 / portTICK_RATE_MS);
		old_direction = cat.object.dir;

		xQueueReceive(xQueue, &buf,0);


		if (buf == 0) {
			dir = direction(buf1);
			if (dir != -1) {
				cat.object.dir = dir;
			}
		}else if (buf == 1){
			dir = direction(buf1);
			if (dir != -1) {
				cat.object.dir = dir;
			}
		}

		//Case of queue is empty don't move;
		if (((buf == 1) || (buf == 0))&&(dir!=0)) {
			if (cat.object.dir == WEST) {
				if (old_direction == NORTH || old_direction == SOUTH) {
					cat.object.pos.x -= x;
					cat.object.pos.y += y;
				}
				if (cat.object.pos.x - STEP_CAT > 0) {
					cat.object.pos.x -= STEP_CAT;
				}
			} else if (cat.object.dir == EAST) {
				if (old_direction == NORTH || old_direction == SOUTH) {
					cat.object.pos.x -= x;
					cat.object.pos.y += y;
				}
				if (cat.object.pos.x + STEP_CAT < MAX_POS_X - 48) {
					cat.object.pos.x += STEP_CAT;
				}
			} else if (cat.object.dir == NORTH) {
				if (old_direction == WEST || old_direction == EAST) {
					cat.object.pos.x += x;
					cat.object.pos.y -= y;
				}
				if (cat.object.pos.y - STEP_CAT > 26) {
					cat.object.pos.y -= STEP_CAT;
				}
			} else if (cat.object.dir == SOUTH) {
				if (old_direction == WEST || old_direction == EAST) {
					cat.object.pos.x += x;
					cat.object.pos.y -= y;
				}
				if (cat.object.pos.y + STEP_CAT < 252 - 48) {
					cat.object.pos.y += STEP_CAT;
				}
			}
		}

		xQueueSend(catQueue, (void * ) &cat, (portTickType ) 0);
	}
}
////////////////////////////////////////////////////////////////
//////////// JOYSTICK CONTROL /////////////////////////////////

/*if (joystick_get_state(JOYSTICK_LEFT)) {
			cat.object.dir = WEST;
			if (old_direction == NORTH || old_direction == SOUTH) {
				cat.object.pos.x -= x;
				cat.object.pos.y += y;
			}
			if (cat.object.pos.x - STEP_CAT > 0) {
				cat.object.pos.x -= STEP_CAT;
			}
		} else if (joystick_get_state(JOYSTICK_RIGHT)) {
			cat.object.dir = EAST;
			if (old_direction == NORTH || old_direction == SOUTH) {
				cat.object.pos.x -= x;
				cat.object.pos.y += y;
			}
			if (cat.object.pos.x + STEP_CAT < MAX_POS_X - 48) {
				cat.object.pos.x += STEP_CAT;
			}
		} else if (joystick_get_state(JOYSTICK_TOP)) {
			cat.object.dir = NORTH;
			if (old_direction == WEST || old_direction == EAST) {
				cat.object.pos.x += x;
				cat.object.pos.y -= y;
			}
			if (cat.object.pos.y - STEP_CAT > 26) {
				cat.object.pos.y -= STEP_CAT;
			}
		} else if (joystick_get_state(JOYSTICK_BOTTOM)) {
			cat.object.dir = SOUTH;
			if (old_direction == WEST || old_direction == EAST) {
				cat.object.pos.x += x;
				cat.object.pos.y -= y;
			}
			if (cat.object.pos.y + STEP_CAT < 252 - 48) {
				cat.object.pos.y += STEP_CAT;
			}
		}*/

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


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
