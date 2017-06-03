#include "display.h"
#include "mouse.h"
#include "cat.h"
#include "lcd.h"
#include "dtmf_detection.h"
#include "queue.h"
#include "task.h"

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
	while (1) {
		vTaskDelayUntil(&xLastWakeTime, 16 / portTICK_RATE_MS);
		if (xQueueReceive(mouseQueue, &newMouse, 0)) {
			draw_mouse(&oldMouse, &newMouse, sprites);
			draw_interface(oldMouse.vitality, newMouse.vitality);
			oldMouse = newMouse;
		}
		if (xQueueReceive(catQueue, &newCat, 0)) {
			draw_cat(&oldCat, &newCat, sprites);
			oldCat = newCat;
		}
		if(detect_collision(newMouse.object, sprites->mouse_im[newMouse.object.dir], newCat.object, sprites->cat_im[newCat.object.dir])){

		}
	}

}
