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
	time_t t;
	srand((unsigned) time(&t));
	int pos_ale = rand() % 3;
	LPC_GPIO1->FIODIR &= ~(0b11111 << 19);
	mouse.new_posx = pos_ale*80+34;
	mouse.new_posy = 280;
	mouse.old_posx = mouse.new_posx;
	mouse.old_posy = mouse.new_posy;
	mouse.new_dir = NORTH;
	mouse.old_dir = NORTH;
	mouse.moving = false;
	mouse.vitality = VITALITYMAX;
	mouse.m = 0;
	mouse.score = 0;
	display_bitmap16(sprites.mouse_im[mouse.new_dir].bitmap, mouse.new_posx, mouse.new_posy, sprites.mouse_im[mouse.new_dir].width, sprites.mouse_im[mouse.new_dir].height);
}

void reset_mouse(sprites_t sprites){
	lcd_filled_rectangle(mouse.new_posx, mouse.new_posy, mouse.new_posx+sprites.mouse_im[mouse.new_dir].width, mouse.new_posy+sprites.mouse_im[mouse.new_dir].height, LCD_WHITE);
	int pos_ale = rand() % 3;
	mouse.old_posx = pos_ale*80+34;
	mouse.old_posy = 280;
	mouse.new_posx = pos_ale*80+34;
	mouse.new_posy = 280;
	mouse.new_dir = NORTH;
	mouse.vitality = VITALITYMAX;
	mouse.m = 0;
	mouse.score++;
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
			case NORTH:
				lcd_filled_rectangle(mouse.new_posx, mouse.new_posy+sprites.mouse_im[mouse.new_dir].height, mouse.old_posx+sprites.mouse_im[mouse.old_dir].width, mouse.old_posy+sprites.mouse_im[mouse.old_dir].height, LCD_WHITE);
				break;
			case EAST:
				lcd_filled_rectangle(mouse.new_posx, mouse.new_posy, mouse.old_posx, mouse.old_posy+sprites.mouse_im[mouse.old_dir].height, LCD_WHITE);
				break;
			case SOUTH:
				lcd_filled_rectangle(mouse.new_posx, mouse.new_posy, mouse.old_posx+sprites.mouse_im[mouse.old_dir].width,mouse.old_posy, LCD_WHITE);
				break;
			case WEST:
				lcd_filled_rectangle(mouse.new_posx+sprites.mouse_im[mouse.old_dir].width,mouse.new_posy,mouse.old_posx+sprites.mouse_im[mouse.old_dir].width+mouse.m,mouse.old_posy+sprites.mouse_im[mouse.old_dir].height, LCD_WHITE);
				break;
		}
	}else{
		lcd_filled_rectangle(mouse.old_posx, mouse.old_posy, mouse.old_posx+sprites.mouse_im[mouse.old_dir].width, mouse.old_posy+sprites.mouse_im[mouse.old_dir].height, LCD_WHITE);
	}

	mouse.m = (3*mouse.vitality+VITALITYMAX/2)/VITALITYMAX + 1;

	mouse.moving = true;
	if (joystick_get_state(JOYSTICK_LEFT)) {
		if(mouse.new_posx - mouse.m > 0){
			mouse.old_posx = mouse.new_posx;
			mouse.new_posx-=mouse.m;
		}
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = WEST;
	}else if (joystick_get_state(JOYSTICK_RIGHT)) {
		if(mouse.new_posx + mouse.m + sprites.mouse_im[mouse.new_dir].width < 239){
			mouse.old_posx = mouse.new_posx;
			mouse.new_posx+=mouse.m;
		}
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = EAST;
	}else if (joystick_get_state(JOYSTICK_TOP)) {
		if(mouse.new_posy - mouse.m > 26){
			mouse.old_posy = mouse.new_posy;
			mouse.new_posy-=mouse.m;
		}else{
			reset_mouse(sprites);
		}
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = NORTH;
	}else if (joystick_get_state(JOYSTICK_BOTTOM)) {
		if(mouse.new_posy + mouse.m < 280){
			mouse.old_posy = mouse.new_posy;
			mouse.new_posy+=mouse.m;
		}
		mouse.old_dir = mouse.new_dir;
		mouse.new_dir = SOUTH;
	}else{
		mouse.moving = false;
	}

	if(!mouse.moving){
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

	display_bitmap16(sprites.mouse_im[mouse.new_dir].bitmap, mouse.new_posx, mouse.new_posy, sprites.mouse_im[mouse.new_dir].width, sprites.mouse_im[mouse.new_dir].height);
}



