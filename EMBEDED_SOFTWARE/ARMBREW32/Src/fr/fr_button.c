/*
 * fr_button.c
 *
 *  Created on: Apr 10, 2016
 *      Author: Rotule
 */

#include "fr_button.h"

#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_lcd.h"

#include <string.h>

TM_FONT_SIZE_t FontSize;

void GUI_BUTTON_INIT(GUI_BUTTON* Button,int x,int y,int w,int h,const char* text,TM_TOUCH_t* TS,
		uint32_t Contour_color,uint32_t Touched_fill_color,uint32_t Touched_text_color,uint32_t text_color,
void (*TouchAction)(void),void (*ReleaseAction)(void)){

	Button->x=x;
	Button->y=y;
	Button->w=w;
	Button->h=h;
	Button->Contour_color=Contour_color;
	Button->Filled_color=LCD_COLOR_BLACK;//GUI_BLACK;
	Button->Touched_fill_color=Touched_fill_color;
	Button->Touched_text_color=Touched_text_color;
	Button->text_color=text_color;
	Button->TouchAction=TouchAction;
	Button->ReleaseAction=ReleaseAction;
	Button->TS=TS;
	strcpy(Button->text,text);
}

void Draw_button_struct(GUI_BUTTON* bt){
	TM_TOUCH_t* TS;

	TS=bt->TS;

	bt->touchstatus=0;
	if (TS->NumPresses) {
		int i;
		/* Go through all presses on LCD */
		for (i = 0; i < TS->NumPresses; i++) {
			/* Draw circle */
		    int xtouch;
		    int ytouch;

		    xtouch=TM_LCD_GetWidth()-TS->X[i];
		    ytouch=TM_LCD_GetHeight()-TS->Y[i];
		    if((xtouch>(bt->x))&&(xtouch<((bt->x)+(bt->w)))&&(ytouch>(bt->y))&&(ytouch<((bt->y)+(bt->h)))){
		    	bt->touchstatus=1;
		    }
		}

	}

	/* Draw Button */
	if(bt->touchstatus){
		Draw_button(bt->x,bt->y,bt->w,bt->h,bt->text,bt->Contour_color,bt->Touched_fill_color,bt->Touched_text_color);
	}else{
		Draw_button(bt->x,bt->y,bt->w,bt->h,bt->text,bt->Contour_color,bt->Filled_color,bt->text_color);
	}

	/* Do actions */
	if(bt->touchstatus && !(bt->touchstatus_mem) && bt->TouchAction!=NULL){
		bt->TouchAction();
	}

	if(!(bt->touchstatus) && bt->touchstatus_mem && bt->ReleaseAction!=NULL){
		bt->ReleaseAction();
	}
	bt->touchstatus_mem=bt->touchstatus;
}

void Draw_button(int x,int y,int w,int h,const char* text,uint32_t color,uint32_t fill_color,uint32_t textcolor){
	uint32_t colormemory;
	uint32_t bkcolormemory;


	bkcolormemory=LCD_COLOR_BLACK;
	colormemory=LCD_COLOR_YELLOW;

	//bkcolormemory=GUI_GetBkColor();
	//colormemory=GUI_GetColor();

	//GUI_SetColor(color);
	//if(filled){

		TM_LCD_DrawRoundedRectangle(x,y,w,h,10,color);
		TM_LCD_DrawFilledRoundedRectangle(x+2,y+2,w-4,h-4,8,fill_color);

		//GUI_DrawRoundedRect(x,y,x+w,y+h,10);
		//GUI_SetColor(fill_color);
		//GUI_FillRoundedRect(x+2,y+2,x+w-2,y+h-2,8);
		//GUI_SetBkColor(color);
	//}else{
	//	GUI_DrawRoundedRect(x,y,x+w,y+h,10);
	//}

	TM_LCD_SetFont(&TM_Font_7x10);
//	GUI_SetFont(&GUI_Font16_1);
//	GUI_SetColor(textcolor);
//	GUI_SetBkColor(fill_color);
//	GUI_DispStringHCenterAt(text,x+(w/2),y+h/2-8);

	TM_FONT_GetStringSize(text, &FontSize, &TM_Font_7x10);

    TM_LCD_SetXY(x+(w - FontSize.Width) / 2, y+(h - FontSize.Height) / 2);

    /* Put string to LCD */
    TM_LCD_SetColors(textcolor,fill_color);
    TM_LCD_Puts(text);


    TM_LCD_SetColors(colormemory,bkcolormemory);
	//GUI_SetBkColor(bkcolormemory);
	//GUI_SetColor(colormemory);

}

