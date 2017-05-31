#include "display.h"
#include "mouse.h"


void draw_vitality(int vitality){
	static int lastVitality = VITALITYMAX;
	if(lastVitality > vitality){
		lcd_filled_rectangle(220, 318-mouse.vitality*20/VITALITYMAX, 231, (318-mouse.vitality*20/75)+1, LCD_WHITE);
	}else{
		lcd_filled_rectangle(220, 319-mouse.vitality*20/VITALITYMAX, 231, 319, LCD_GREEN);
	}
	lastVitality = vitality;

}

void display(){
	mouse_t mouse;
	draw_vitality(mouse.vitality);
	draw_mouse(mouse);
	draw_cat(cat);
}
