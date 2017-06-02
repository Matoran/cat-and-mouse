#include "display.h"
#include "mouse.h"
#include "cat.h"
#include "lcd.h"


void draw_interface(int vitality, sprites_t *sprites){
	static int lastVitality = VITALITYMAX;
	if(lastVitality > vitality){
		lcd_filled_rectangle(220, 318-mouse.vitality*20/VITALITYMAX, 231, (318-mouse.vitality*20/75)+1, LCD_WHITE);
	}else{
		lcd_filled_rectangle(220, 319-mouse.vitality*20/VITALITYMAX, 231, 319, LCD_GREEN);
	}
	lastVitality = vitality;

	for (int i=0; i<3; i++)
	{
		display_bitmap16(sprites->cheese_im.bitmap, 28+80*i, 0, sprites->cheese_im.width, sprites->cheese_im.height);
		lcd_filled_rectangle(30+80*i, 313, 30+12+80*i, 319, LCD_BLACK);		// mouse pits
		lcd_filled_circle(30+6+80*i, 313, 6, LCD_BLACK);
	}
	lcd_empty_rectangle(219, 299, 233, 319, LCD_BLACK);						// empty gauge
}

void display(sprites_t *sprites){
	mouse_t mouse;
	draw_mouse(&mouse, sprites);
	draw_cat();
	if(1){

	}
	draw_interface(mouse.vitality, sprites);
}
