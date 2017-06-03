/**
 * Name        : tp2_cat_and_mouse.c
 * Date        : 5.5.2017
 * Description : cat and mouse game with 2 mylab2 boards. FreeRTOS
 *               and LCD display used.
 */

#include <cr_section_macros.h>
#include <ethmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"
#include "debug.h"
#include "trace_mgt.h"
#include "objects.h"
#include "mouse.h"
#include "dtmf_detection.h"
#include "display.h"
#include "cat.h"


int main(void)
{
	static sprites_t sprites;
	int i;

	//ethernet_power_down();				// remove potential noise on ADC due to Ethernet module
	init_lcd();
	clear_screen(LCD_WHITE);
	// load all necessary sprites
	sprites.cat_im[0].bitmap=read_bmp_file("cat 24x48 up.bmp", &sprites.cat_im[0].width, &sprites.cat_im[0].height);
	sprites.cat_im[1].bitmap=read_bmp_file("cat 24x48 right.bmp", &sprites.cat_im[1].width, &sprites.cat_im[1].height);
	sprites.cat_im[2].bitmap=read_bmp_file("cat 24x48 down.bmp", &sprites.cat_im[2].width, &sprites.cat_im[2].height);
	sprites.cat_im[3].bitmap=read_bmp_file("cat 24x48 left.bmp", &sprites.cat_im[3].width, &sprites.cat_im[3].height);
	sprites.mouse_im[0].bitmap=read_bmp_file("mouse 12x20 up.bmp", &sprites.mouse_im[0].width, &sprites.mouse_im[0].height);
	sprites.mouse_im[1].bitmap=read_bmp_file("mouse 12x20 right.bmp", &sprites.mouse_im[1].width, &sprites.mouse_im[1].height);
	sprites.mouse_im[2].bitmap=read_bmp_file("mouse 12x20 down.bmp", &sprites.mouse_im[2].width, &sprites.mouse_im[2].height);
	sprites.mouse_im[3].bitmap=read_bmp_file("mouse 12x20 left.bmp", &sprites.mouse_im[3].width, &sprites.mouse_im[3].height);
	if ((sprites.cheese_im.bitmap=read_bmp_file("cheese 32x26.bmp", &sprites.cheese_im.width, &sprites.cheese_im.height))==NULL)
		EXIT("Not enough space to create image!");
	for (i=0; i<4; i++)
		if (sprites.cat_im[i].bitmap==NULL || sprites.mouse_im[i].bitmap==NULL)
			EXIT("Not enough space to create image!");
	init_traces(115200, 2, true);		// to be removed if you implement your own traces

	mouseQueue = xQueueCreate(1, sizeof(mouse_t));
	if(mouseQueue == 0){
		EXIT("Failed to create mouse queue!");
	}
	catQueue = xQueueCreate(1, sizeof(cat_t));
	if(catQueue == 0){
			EXIT("Failed to create cat queue!");
	}

	if ((xQueue = xQueueCreate( 1, sizeof(int))) == 0) {
		EXIT("Fail to create DMA queue !");
	};

	lcd_print(20, 150, BIGFONT, LCD_BLACK, LCD_WHITE, "Bon travail!");

	//init_dtmf();

	if (xTaskCreate(task_mouse, (signed portCHAR *)"mouse", configMINIMAL_STACK_SIZE, &sprites, 1, NULL) != pdPASS) {
		while (1);
	}
	if (xTaskCreate(task_cat, (signed portCHAR *)"cat", configMINIMAL_STACK_SIZE, &sprites, 1, NULL) != pdPASS) {
		while (1);
	}
	if (xTaskCreate(task_display, (signed portCHAR *)"display", configMINIMAL_STACK_SIZE, &sprites, 1, NULL) != pdPASS) {
		while (1);
	}
	vTaskStartScheduler();

	return 1;
}
