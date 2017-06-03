#include "display.h"
#include "mouse.h"
#include "cat.h"
#include "lcd.h"
#include "dtmf_detection.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "define.h"
#include "task.h"

/* Description: detects collisions between 2 rectangular objects. As soon as the distance between
 * both objects if lower than or equal to the margin, the collision is detected.
 * Parameters:
 * 	obj1 and obj2: positions and directions of the objects concerned
 * 	obj1_im, obj2_im: reference to the object images (containing the dimensions)
 * Return: true if the collision is detected, false otherwise
 */
bool detect_collision(object_t obj1, image_t *obj1_im, object_t obj2, image_t *obj2_im, int margin)
{
	return ((obj1.pos.x-obj2.pos.x<(int)obj2_im[obj2.dir].width+margin-1) &&
		(obj2.pos.x-obj1.pos.x<(int)obj1_im[obj1.dir].width+margin-1) &&
		(obj2.pos.y-obj1.pos.y<(int)obj1_im[obj1.dir].height+margin-1) &&
		(obj1.pos.y-obj2.pos.y<(int)obj2_im[obj2.dir].height+margin-1));
}

void draw_interface(int old, int new) {
	if (old > new) {
		lcd_filled_rectangle(
				220,
				318 - old * 18 / VITALITYMAX,
				232,
				318 - new * 18 / VITALITYMAX,
				LCD_WHITE);
	} else if(old < new) {
		lcd_filled_rectangle(220,
				318 - new * 18 / VITALITYMAX,
				232,
				318 - old * 18 / VITALITYMAX,
		LCD_GREEN);
	}
}

void task_display(void *param) {
	sprites_t *sprites = (sprites_t*) param;
	mouse_t oldMouse, newMouse;
	init_mouse(&oldMouse);
	cat_t oldCat, newCat;
	init_cat(&oldCat);
	int scoreCat, scoreMouse;
	scoreCat = scoreMouse = 0;
	for (int i = 0; i < 3; i++) {
		display_bitmap16(sprites->cheese_im.bitmap, 28 + 80 * i, 0,
				sprites->cheese_im.width, sprites->cheese_im.height);
		lcd_filled_rectangle(30 + 80 * i, 313, 30 + 12 + 80 * i, 319,
				LCD_BLACK);		// mouse pits
		lcd_filled_circle(30 + 6 + 80 * i, 313, 6, LCD_BLACK);
	}
	lcd_empty_rectangle(219, 299, 233, 319, LCD_BLACK);			// empty gauge
	lcd_filled_rectangle(220, 318 - 18, 232, 318, LCD_GREEN);
	portTickType xLastWakeTime = xTaskGetTickCount();
	lcd_print(10,305, SMALLFONT, LCD_BLACK, LCD_WHITE,"%d", scoreMouse);
	lcd_print(10,5, SMALLFONT, LCD_BLACK, LCD_WHITE,"%d",scoreCat);
	while (1) {
		vTaskDelayUntil(&xLastWakeTime, 16 / portTICK_RATE_MS);
		if (xQueueReceive(mouseQueue, &newMouse, 0)) {
			draw_mouse(&oldMouse, &newMouse, sprites);
			draw_interface(oldMouse.vitality, newMouse.vitality);
			oldMouse = newMouse;
		}
		if (newMouse.object.pos.y - newMouse.m <= 26) {
			bool reset = true;
			xQueueSend(mouseResetQueue, (void * ) &reset, (portTickType ) 0);
			lcd_filled_rectangle(newMouse.object.pos.x, newMouse.object.pos.y,
							newMouse.object.pos.x + sprites->cat_im[newMouse.object.dir].width,
							newMouse.object.pos.y + sprites->cat_im[newMouse.object.dir].height,
							LCD_WHITE);
			scoreMouse++;
			lcd_print(10,305, SMALLFONT, LCD_BLACK, LCD_WHITE,"%d",scoreMouse);
		}
		if (xQueueReceive(catQueue, &newCat, 0)) {
			draw_cat(&oldCat, &newCat, sprites);
			oldCat = newCat;
		}
		if(detect_collision(newMouse.object, sprites->mouse_im, newCat.object, sprites->cat_im, 0)){
			bool reset = true;
			xQueueSend(mouseResetQueue, (void * ) &reset, (portTickType ) 0);
			xQueueSend(catResetQueue, (void * ) &reset, (portTickType ) 0);
			lcd_filled_rectangle(newMouse.object.pos.x, newMouse.object.pos.y,
										newMouse.object.pos.x + sprites->cat_im[newMouse.object.dir].width,
										newMouse.object.pos.y + sprites->cat_im[newMouse.object.dir].height,
										LCD_WHITE);
			lcd_filled_rectangle(newCat.object.pos.x, newCat.object.pos.y,
					newCat.object.pos.x + sprites->cat_im[newCat.object.dir].width,
					newCat.object.pos.y + sprites->cat_im[newCat.object.dir].height,
							LCD_WHITE);
			scoreCat++;
			lcd_print(10,5, SMALLFONT, LCD_BLACK, LCD_WHITE,"%d",scoreCat);
		}
	}

}
