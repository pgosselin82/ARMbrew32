/*
 * fr_button.c
 *
 *  Created on: Apr 10, 2016
 *      Author: Rotule
 */

#include "fr_button.h"


void GUI_BUTTON_INIT(GUI_BUTTON* Button,int x,int y,int w,int h,const char* text,TM_TOUCH_t* TS,
		uint32_t Contour_color,uint32_t Touched_fill_color,uint32_t Touched_text_color,uint32_t text_color,
void (*TouchAction)(void),void (*ReleaseAction)(void)){

	Button->x=x;
	Button->y=y;
	Button->w=w;
	Button->h=h;
	Button->Contour_color=Contour_color;
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

		    xtouch=LCD_GetXSize()-TS->X[i];
		    ytouch=LCD_GetYSize()-TS->Y[i];
		    if((xtouch>(bt->x))&&(xtouch<((bt->x)+(bt->w)))&&(ytouch>(bt->y))&&(ytouch<((bt->y)+(bt->h)))){
		    	bt->touchstatus=1;
		    }
		}

	}

	/* Draw Button */
	if(bt->touchstatus){
		Draw_button(bt->x,bt->y,bt->w,bt->h,bt->text,bt->Touched_fill_color,bt->Touched_text_color,1);
	}else{
		Draw_button(bt->x,bt->y,bt->w,bt->h,bt->text,bt->Contour_color,bt->text_color,0);
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

void Draw_button(int x,int y,int w,int h,const char* text,uint32_t color,uint32_t textcolor,int filled){
	uint32_t colormemory;
	uint32_t bkcolormemory;

	bkcolormemory=GUI_GetBkColor();
	colormemory=GUI_GetColor();

	GUI_SetColor(color);
	if(filled){
		GUI_FillRoundedRect(x,y,x+w,y+h,10);
		GUI_SetBkColor(color);
	}else{
		GUI_DrawRoundedRect(x,y,x+w,y+h,10);
	}
	GUI_SetFont(&GUI_Font16_1);
	GUI_SetColor(textcolor);

	GUI_DispStringHCenterAt(text,x+(w/2),y+h/2-8);

	GUI_SetBkColor(bkcolormemory);
	GUI_SetColor(colormemory);

}

