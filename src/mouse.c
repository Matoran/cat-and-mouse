/*
 * mouse.c
 *
 *  Created on: 24 mai 2017
 *      Author: selement
 */
#include "mouse.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

void init_mouse(sprites_t sprites){
	LPC_GPIO1->FIODIR &= ~(0b11111 << 19);
	mouse.new_posx = 100;
	mouse.new_posy = 100;
	mouse.old_posx = 100;
	mouse.old_posy = 100;
	mouse.new_dir = 0;
	mouse.old_dir = 0;
	mouse.not_moving = 0;
	mouse.vitality = VITALITYMAX;
	mouse.m = 0;
	display_bitmap16(sprites.mouse_im[mouse.new_dir].bitmap, mouse.new_posx, mouse.new_posy, sprites.mouse_im[mouse.new_dir].width, sprites.mouse_im[mouse.new_dir].height);
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

void task_mouse(sprites_t sprites){
	if(mouse.new_dir == mouse.old_dir){
		switch(mouse.new_dir){
			case 0:
				lcd_filled_rectangle(mouse.new_posx, mouse.new_posy+sprites.mouse_im[mouse.new_dir].height, mouse.new_posx+sprites.mouse_im[mouse.new_dir].width, mouse.new_posy+sprites.mouse_im[mouse.new_dir].height+mouse.m, LCD_WHITE);
				break;
			case 1:
				lcd_filled_rectangle(mouse.new_posx-mouse.m, mouse.new_posy, mouse.new_posx, mouse.new_posy+sprites.mouse_im[mouse.new_dir].height, LCD_WHITE);
				break;
			case 2:
				lcd_filled_rectangle(mouse.new_posx, mouse.new_posy-mouse.m,mouse.new_posx+sprites.mouse_im[mouse.new_dir].width,mouse.new_posy, LCD_WHITE);
				break;
			case 3:
				lcd_filled_rectangle(mouse.new_posx+sprites.mouse_im[mouse.new_dir].width,mouse.new_posy,mouse.new_posx+sprites.mouse_im[mouse.new_dir].width+mouse.m,mouse.new_posy+sprites.mouse_im[mouse.new_dir].height, LCD_WHITE);
				break;
		}
	}else{
		lcd_filled_rectangle(mouse.old_posx, mouse.old_posy, mouse.old_posx+sprites.mouse_im[mouse.old_dir].width, mouse.old_posy+sprites.mouse_im[mouse.old_dir].height, LCD_WHITE);
	}

	mouse.m = (3*mouse.vitality+VITALITYMAX/2)/VITALITYMAX + 1;

	if (joystick_get_state(JOYSTICK_LEFT)) {
		mouse.old_posx = mouse.new_posx;
		mouse.new_posx-=mouse.m;
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = 3;
	}else{
		mouse.not_moving++;
	}

	if (joystick_get_state(JOYSTICK_RIGHT)) {
		mouse.old_posx = mouse.new_posx;
		mouse.new_posx+=mouse.m;
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = 1;
	}else{
		mouse.not_moving++;
	}

	if (joystick_get_state(JOYSTICK_TOP)) {
		mouse.old_posy = mouse.new_posy;
		mouse.new_posy-=mouse.m;
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = 0;
	}else{
		mouse.not_moving++;
	}

	if (joystick_get_state(JOYSTICK_BOTTOM)) {
		mouse.old_posy = mouse.new_posy;
		mouse.new_posy+=mouse.m;
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = 2;
	}else{
		mouse.not_moving++;
	}

	if(mouse.not_moving == 4){
		if(mouse.vitality < VITALITYMAX){
			mouse.vitality++;
			lcd_filled_rectangle(220, 319-mouse.vitality*20/VITALITYMAX, 231, 319, LCD_GREEN);
		}
	}else{
		if(mouse.vitality > 0){
			lcd_filled_rectangle(220, 318-mouse.vitality*20/75, 231, (318-mouse.vitality*20/75)+1, LCD_WHITE);
			mouse.vitality--;
		}
	}
	mouse.not_moving = 0;

	display_bitmap16(sprites.mouse_im[mouse.new_dir].bitmap, mouse.new_posx, mouse.new_posy, sprites.mouse_im[mouse.new_dir].width, sprites.mouse_im[mouse.new_dir].height);

}



