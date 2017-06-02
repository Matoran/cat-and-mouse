/**
 * Name        : tp2_cat_and_mouse.c
 * Date        : 5.5.2017
 * Description : cat and mouse game with 2 mylab2 boards. FreeRTOS
 *               and LCD display used.
 */

#include <cr_section_macros.h>
#include <dtmf_detection.h>
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


int main(void)
{
	static sprites_t sprites;
	int i;

	ethernet_power_down();				// remove potential noise on ADC due to Ethernet module
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



	lcd_print(20, 150, BIGFONT, LCD_BLACK, LCD_WHITE, "Bon travail!");

	init_dtmf();

	while(1);
	return 1;
}
