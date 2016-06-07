/*
 * MyGui.c
 *
 *  Created on: Apr 18, 2016
 *      Author: Rotule
 */


#include "GUI.h"
#include "WM.h"
#include "fr_graph.h"
#include "fr_button.h"
#include "MyGui.h"
#include "GlobalVar.h"


#include <string.h>


#include "tm_stm32_touch.h"
#include "tm_stm32_touch_ft5336.h"

TM_TOUCH_t TS;
TM_TOUCH_DRIVER_t Driver;
int CurrentPage=1;

GUI_HWIN hWin;


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/




GUI_BUTTON bt_menu1;
GUI_BUTTON bt_menu2;
GUI_BUTTON bt_menu3;
GUI_BUTTON bt_menu4;
GUI_BUTTON bt_menu5;
FR_GUI_GRAPH_T temp_plot;

void Draw_menu(void);

void Menu1Touch(void);
void Menu2Touch(void);
void Menu3Touch(void);
void Menu4Touch(void);
void Menu5Touch(void);

void DrawPage1(void);
void DrawPage2(void);
void DrawPage3(void);
void DrawPage4(void);
void DrawPage5(void);

void InitGui(void);
void GuiLoop(void);

void updateUI(WM_MESSAGE * pMsg);

void GuiLoop(void){
	  //GUI_Delay(25);
	  TM_TOUCH_Read(&TS);
	  WM_Paint(hWin);
}

void InitGui(void) {
	int distance=2;
	int rect_height=52;
	int rect_width=100;
	int y;
	int x;
	int i=0;

 //HAL_NVIC_DisableIRQ(

	Driver.Init=(&TM_TOUCH_FT5336_Init);
	Driver.Read=(&TM_TOUCH_FT5336_Read);

	  GUI_SetFont(&GUI_Font20_1);

	  TM_TOUCH_Init(&Driver, &TS);

	  /* Create the 5 Buttons Menu */
	  x=distance;
	  y=distance;
	  GUI_BUTTON_INIT(&bt_menu1,x,y,rect_width,rect_height,"Temp Graph",&TS,GUI_YELLOW,GUI_YELLOW,GUI_BLACK,GUI_YELLOW,&Menu1Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu2,x,y,rect_width,rect_height,"Menu2",&TS,GUI_YELLOW,GUI_YELLOW,GUI_BLACK,GUI_YELLOW,&Menu2Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu3,x,y,rect_width,rect_height,"Menu3",&TS,GUI_YELLOW,GUI_YELLOW,GUI_BLACK,GUI_YELLOW,&Menu3Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu4,x,y,rect_width,rect_height,"Menu4",&TS,GUI_YELLOW,GUI_YELLOW,GUI_BLACK,GUI_YELLOW,&Menu4Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu5,x,y,rect_width,rect_height,"Settings",&TS,GUI_YELLOW,GUI_YELLOW,GUI_BLACK,GUI_YELLOW,&Menu5Touch,NULL);


	  temp_plot.x=150;
	  temp_plot.y=50;
	  temp_plot.w=275;
	  temp_plot.h=175;
	  strcpy(temp_plot.Title,"Temperature[°C]/Time[S]");

	  temp_plot.TS=&TS;
	  temp_plot.TouchAction=NULL;
	  temp_plot.ReleaseAction=NULL;

	  temp_plot.x_axis.w=temp_plot.w;
	  temp_plot.x_axis.minEU=-60;//0;
	  temp_plot.x_axis.maxEU=0;

	  temp_plot.y_axis.h=temp_plot.h;
	  temp_plot.y_axis.minEU=-1;
	  temp_plot.y_axis.maxEU=100;

	  temp_plot.Data.first_point=NULL;
	  temp_plot.Data.last_point=NULL;
	  temp_plot.Data.pointQTY=0;

	  for(i=119;i>=0;i--){


			  add_data_point(&(temp_plot.Data),-i,0);

	  }



	  //hWin = WM_CreateWindowAsChild(0,0,480,272, WM_HBKWIN, WM_CF_SHOW, &updateUI, 0);
	  //WM_Init();
	  hWin = WM_CreateWindow(0,0,480,272, WM_CF_SHOW, &updateUI,0);



	 // WM_Activate();
  /*while(1){

	  GUI_Delay(25);
	  TM_TOUCH_Read(&TS);
	  WM_Paint(hWin);

  }*/
}

void updateUI(WM_MESSAGE * pMsg){

char str[1000]={0};

	switch(pMsg->MsgId)
	{
	case WM_PAINT:
	{
	GUI_Clear();
	Draw_menu();
	GUI_SetColor(GUI_RED);

	sprintf(str,"%i",G_SecondCounter);

	GUI_DispStringAt(str,440,8);


	GUI_SetColor(GUI_YELLOW);
		  GUI_DrawRoundedRect(105,2,474,269,5);
		  GUI_SetFont(&GUI_Font16_1);
		  switch(CurrentPage){
		  	  case 1:
		  		  DrawPage1();
		  		  break;
		  	  case 2:
		  		  DrawPage2();
		  		  break;
		  	  case 3:
		  		  DrawPage3();
		  		  break;
		  	  case 4:
		  		  DrawPage4();
		  		  break;
		  	  case 5:
		  		  DrawPage5();
		  		  break;
		  	  }

		  	  //Draw circle on pressed Area
		  	  if (TS.NumPresses) {
		  		  int i;
		  	      /* Go through all presses on LCD */
		  	      for (i = 0; i < TS.NumPresses; i++) {
		  	      	/* Draw circle */
		  	    	GUI_SetColor(GUI_GRAY);
		  	      	GUI_DrawCircle(LCD_GetXSize()-TS.X[i], LCD_GetYSize()-TS.Y[i],5);

		  	      }
		  	  }
		  	break;
		  	}
		  	default:
		  	{
		  	WM_DefaultProc(pMsg);
		  	}
	}
}


void Draw_menu(void){

	  Draw_button_struct(&bt_menu1);
	  Draw_button_struct(&bt_menu2);
	  Draw_button_struct(&bt_menu3);
	  Draw_button_struct(&bt_menu4);
	  Draw_button_struct(&bt_menu5);

}

void DrawPage1(void){
	//GUI_DispStringAt("Temperature Graph",113,8);

	Draw_graph_struct(&temp_plot);
}
void DrawPage2(void){
	GUI_DispStringAt("Page2 for future use",113,8);

}
void DrawPage3(void){
	GUI_DispStringAt("Page3 for future use",113,8);
}
void DrawPage4(void){
	GUI_DispStringAt("Page4 for future use",113,8);
}
void DrawPage5(void){
	GUI_DispStringAt("Setting Page",113,8);
}

void Menu1Touch(void){
	CurrentPage=1;
}
void Menu2Touch(void){
	CurrentPage=2;
}
void Menu3Touch(void){
	CurrentPage=3;
}
void Menu4Touch(void){
	CurrentPage=4;
}
void Menu5Touch(void){
	CurrentPage=5;
}

void UpdateGraph(int val){
	FR_DATA_POINT_T* this_point;

	remove_data_point_beginning(&(temp_plot.Data));

	this_point=temp_plot.Data.first_point;

	while(this_point!=NULL){
		this_point->x=(this_point->x-1);
		this_point=this_point->next;
	}
	add_data_point(&(temp_plot.Data),0,val);

}

/*************************** End of file ****************************/
