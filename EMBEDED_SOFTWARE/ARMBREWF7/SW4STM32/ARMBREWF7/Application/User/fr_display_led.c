/*
 * fr_display_led.c
 *
 *  Created on: Apr 10, 2016
 *      Author: Rotule
 */

#include "fr_display_led.h"

void Draw_led_struct(GUI_LED* led){
	GUI_SetColor(led->Contour_color);
	GUI_FillCircle(led->x,led->y,(led->d)/2);
	GUI_SetColor(led->Fill_color);
	GUI_FillCircle(led->x,led->y,(led->d-2)/2);
}

void GUI_LED_INIT(GUI_LED* led,int x,int y,int d,uint32_t Contour_color,uint32_t Fill_color){

	led->x=x;
	led->y=y;
	led->d=d;
	led->Contour_color=Contour_color;
	led->Fill_color=Fill_color;

}
