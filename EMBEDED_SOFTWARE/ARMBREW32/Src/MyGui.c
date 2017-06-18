/*
 * MyGui.c
 *
 *  Created on: Apr 18, 2016
 *      Author: Rotule
 */


#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_lcd.h"

//#include "fr_graph.h"
#include "fr_button.h"
#include "fr_display_led.h"
#include "MyGui.h"
#include "GlobalVar.h"


#include <string.h>


#include "tm_stm32_touch.h"
#include "tm_stm32_touch_ft5336.h"

TM_TOUCH_t TS;
TM_TOUCH_DRIVER_t Driver;
int CurrentPage=1;

//GUI_HWIN hWin;


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



/* Menu buttons */
GUI_BUTTON bt_menu1;
GUI_BUTTON bt_menu2;
GUI_BUTTON bt_menu3;
GUI_BUTTON bt_menu4;
GUI_BUTTON bt_menu5;
//FR_GUI_GRAPH_T temp_plot;

/* Page 2 Widgets HEATER CTRL*/
GUI_BUTTON bt_pwm_plus;
GUI_BUTTON bt_pwm_minus;
GUI_BUTTON bt_pwm_on;
GUI_BUTTON bt_pwm_pct;
GUI_BUTTON bt_pwm_temp;

GUI_BUTTON bt_pwm_set_pct_100;
GUI_BUTTON bt_pwm_set_pct_75;
GUI_BUTTON bt_pwm_set_pct_50;
GUI_BUTTON bt_pwm_set_pct_25;
GUI_BUTTON bt_pwm_set_pct_0;

GUI_LED ld_pwm_on;
GUI_LED ld_pwm_output;


/* Page 3 Widgets */

/* Page 4 Widgets */

/* Page 5 Widgets Settings*/
GUI_BUTTON bt_setting_save;


TM_FONT_SIZE_t FontSize;

void Draw_menu(void);

void Menu1Touch(void);
void Menu2Touch(void);
void Menu3Touch(void);
void Menu4Touch(void);
void Menu5Touch(void);

void bt_pwm_plus_touch(void);
void bt_pwm_minus_touch(void);
void bt_pwm_on_touch(void);
void bt_pwm_set_100_touch(void);
void bt_pwm_set_75_touch(void);
void bt_pwm_set_50_touch(void);
void bt_pwm_set_25_touch(void);
void bt_pwm_set_0_touch(void);

void bt_setting_save_touch(void);



void DrawPage1(void);
void DrawPage2(void);
void DrawPage3(void);
void DrawPage4(void);
void DrawPage5(void);

void InitGui(void);
void GuiLoop(void);
void updateUI(void);
//void updateUI(WM_MESSAGE * pMsg);

void GuiLoop(void){

	static int temp=0;

	temp++;

	if(temp>50){
		temp=0;
		TM_TOUCH_Init(&Driver, &TS);
	}

	  //GUI_Delay(25);
	  TM_TOUCH_Read(&TS);
	  //TM_LCD_SetLayer1();
	  TM_LCD_Layer1To2();
	  TM_LCD_SetLayer2Opacity(255);
	  TM_LCD_SetLayer1Opacity(0);
	  TM_LCD_SetLayer1();
	  TM_LCD_Fill(LCD_COLOR_BLACK);
	  //TM_LCD_
	  updateUI();
	  //WM_Paint(hWin);
	  //TM_LCD_ChangeLayers();
	  TM_LCD_SetLayer2Opacity(0);
	  TM_LCD_SetLayer1Opacity(255);
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

    TM_LCD_Init();

    /* Fill LCD with color */
    TM_LCD_Fill(LCD_COLOR_BLACK);

    /* Put string on the middle of LCD */
    TM_LCD_SetFont(&TM_Font_7x10);

    TM_LCD_SetOrientation(1);

    /* Put string to LCD */
    TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);



	  //GUI_SetFont(&GUI_Font20_1);

	  TM_TOUCH_Init(&Driver, &TS);
	  	  //GUI_TOUCH_Exec()
	  /* Create the 5 Buttons Menu */
	  x=distance;
	  y=distance;
	  GUI_BUTTON_INIT(&bt_menu1,x,y,rect_width,rect_height,"Temp Graph",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&Menu1Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu2,x,y,rect_width,rect_height,"HEAT CTRL",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&Menu2Touch,NULL);
	  y=y+rect_height+distance;
	  //GUI_BUTTON_INIT(&bt_menu3,x,y,rect_width,rect_height,"Menu3",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&Menu3Touch,NULL);
	  y=y+rect_height+distance;
	  //GUI_BUTTON_INIT(&bt_menu4,x,y,rect_width,rect_height,"Menu4",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&Menu4Touch,NULL);
	  y=y+rect_height+distance;
	  GUI_BUTTON_INIT(&bt_menu5,x,y,rect_width,rect_height,"Settings",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&Menu5Touch,NULL);

	  /* Buttons on Page 1 */

	  /* Buttons on Page 2 */
	  x=150;
	  y=100;
	  GUI_BUTTON_INIT(&bt_pwm_pct,x,y,95,50,"NULL",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,NULL,NULL);
	  y+=55;
	  GUI_BUTTON_INIT(&bt_pwm_temp,x,y,95,50,"NULL",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,NULL,NULL);

	  x=250;
	  y=100;
	  GUI_BUTTON_INIT(&bt_pwm_plus,x,y,50,50,"+",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_plus_touch,NULL);
	  y+=55;
	  GUI_BUTTON_INIT(&bt_pwm_minus,x,y,50,50,"-",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_minus_touch,NULL);
	  x+=55;
	  GUI_BUTTON_INIT(&bt_pwm_on,x,100,50,105,"OFF",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_on_touch,NULL);


	  x=400;
	  y=35;
	  GUI_BUTTON_INIT(&bt_pwm_set_pct_100,x,y,55,40,"SET 100",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_set_100_touch,NULL);
	  y+=45;
	  GUI_BUTTON_INIT(&bt_pwm_set_pct_75,x,y,55,40,"SET 75",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_set_75_touch,NULL);
	  y+=45;
	  GUI_BUTTON_INIT(&bt_pwm_set_pct_50,x,y,55,40,"SET 50",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_set_50_touch,NULL);
	  y+=45;
	  GUI_BUTTON_INIT(&bt_pwm_set_pct_25,x,y,55,40,"SET 25",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_set_25_touch,NULL);
	  y+=45;
	  GUI_BUTTON_INIT(&bt_pwm_set_pct_0,x,y,55,40,"SET 0",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_pwm_set_0_touch,NULL);

	  GUI_LED_INIT(&ld_pwm_on,315,75,25,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	  GUI_LED_INIT(&ld_pwm_output,345,75,25,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);

	  /* Buttons on Page 3 */

	  /* Buttons on Page 4 */

	  /* Buttons on Page 5 */

	  GUI_BUTTON_INIT(&bt_setting_save,400,215,55,40,"SAVE",&TS,LCD_COLOR_YELLOW,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,LCD_COLOR_YELLOW,&bt_setting_save_touch,NULL);

	/*  temp_plot.x=150;
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
*/


	  //hWin = WM_CreateWindowAsChild(0,0,480,272, WM_HBKWIN, WM_CF_SHOW, &updateUI, 0);
	  //WM_Init();
	  //hWin = WM_CreateWindow(0,0,480,272, WM_CF_SHOW, &updateUI,0);


	 // WM_Activate();
  /*while(1){

	  GUI_Delay(25);
	  TM_TOUCH_Read(&TS);
	  WM_Paint(hWin);

  }*/
}

void updateUI(void){//WM_MESSAGE * pMsg){

char str[1000]={0};

	//switch(pMsg->MsgId)
	//{
	//case WM_PAINT:
	//{

	//GUI_Clear();
	Draw_menu();
	TM_LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	//GUI_SetColor(GUI_RED);

	sprintf(str,"%i",G_SecondCounter);

	TM_LCD_SetXY(440,8);
	TM_LCD_Puts(str);
	//GUI_DispStringAt(str,440,8);

	//TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	//GUI_SetColor(GUI_YELLOW);
	TM_LCD_DrawRoundedRectangle(105,2,369,267,5,LCD_COLOR_YELLOW);
		  //GUI_DrawRoundedRect(105,2,474,269,5);
	TM_LCD_SetFont(&TM_Font_7x10);
		 // GUI_SetFont(&GUI_Font16_1);
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
		  	    	//TM_LCD_SetColors(LCD_COLOR_GRAY,LCD_COLOR_BLACK);
		  	    	//GUI_SetColor(GUI_GRAY);
		  	    	TM_LCD_DrawCircle(TM_LCD_GetWidth()-TS.X[i], TM_LCD_GetHeight()-TS.Y[i],5,LCD_COLOR_GRAY);
		  	      	//GUI_DrawCircle(LCD_GetXSize()-TS.X[i], LCD_GetYSize()-TS.Y[i],5);

		  	      }
		  	  }
		  	//break;
		  	//}
	//	  	default:
	//	  	{
		  	//WM_DefaultProc(pMsg);
	//	  	}
	//}
}


void Draw_menu(void){

	  Draw_button_struct(&bt_menu1);
	  Draw_button_struct(&bt_menu2);
	  //Draw_button_struct(&bt_menu3);
	  //Draw_button_struct(&bt_menu4);
	  Draw_button_struct(&bt_menu5);

}

void DrawPage1(void){
	//GUI_DispStringAt("Temperature Graph",113,8);

	//Draw_graph_struct(&temp_plot);
}
void DrawPage2(void){
	//char tempstr[255]={0};

	if(G_pwm_output_status){
		ld_pwm_on.Fill_color=LCD_COLOR_RED;//GUI_RED;
		bt_pwm_on.Filled_color=LCD_COLOR_RED;//GUI_RED;
		bt_pwm_on.text_color=LCD_COLOR_BLACK;//GUI_BLACK;
	}else{
		ld_pwm_on.Fill_color=LCD_COLOR_BLACK;//GUI_BLACK;
		bt_pwm_on.Filled_color=LCD_COLOR_BLACK;//GUI_BLACK;
		bt_pwm_on.text_color=LCD_COLOR_YELLOW;//GUI_YELLOW;
	}

	if(G_pwm_output){
		ld_pwm_output.Fill_color=LCD_COLOR_RED;//GUI_RED;
	}else{
		ld_pwm_output.Fill_color=LCD_COLOR_BLACK;//GUI_BLACK;
	}


	//GUI_DispStringAt("HEATING ELEMENT PWM",113,8);
	TM_LCD_SetXY(113,8);
	TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	TM_LCD_Puts("HEATING ELEMENT PWM");

	sprintf(bt_pwm_pct.text,"%i %%",G_pwm_pct_value);
	Draw_button_struct(&bt_pwm_pct);
	//G_temperature=22.5;
	sprintf(bt_pwm_temp.text,"%.2f C",G_temperature);
	Draw_button_struct(&bt_pwm_temp);

	Draw_button_struct(&bt_pwm_plus);
	Draw_button_struct(&bt_pwm_minus);
	Draw_button_struct(&bt_pwm_on);

	Draw_button_struct(&bt_pwm_set_pct_100);
	Draw_button_struct(&bt_pwm_set_pct_75);
	Draw_button_struct(&bt_pwm_set_pct_50);
	Draw_button_struct(&bt_pwm_set_pct_25);
	Draw_button_struct(&bt_pwm_set_pct_0);

	//sprintf(tempstr,"%i pts",temp_plot.Data.pointQTY);
	//GUI_DispStringAt(tempstr,113,25);



	Draw_led_struct(&ld_pwm_on);

	Draw_led_struct(&ld_pwm_output);

}
void DrawPage3(void){
	//GUI_DispStringAt("Page3 for future use",113,8);
	TM_LCD_SetXY(113,8);
	TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	TM_LCD_Puts("Page3 for future use");
}
void DrawPage4(void){
	//GUI_DispStringAt("Page4 for future use",113,8);
	TM_LCD_SetXY(113,8);
		TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
		TM_LCD_Puts("Page4 for future use");
}
void DrawPage5(void){
	//GUI_DispStringAt("Setting Page",113,8);
	TM_LCD_SetXY(113,8);
		TM_LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
		TM_LCD_Puts("Setting Page");

	Draw_button_struct(&bt_setting_save);

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

void bt_pwm_plus_touch(void){
	//PWM++
	if(G_pwm_pct_value<100){
		G_pwm_pct_value+=1;
	}
}
void bt_pwm_minus_touch(void){
	//PWM--
	if(G_pwm_pct_value>0){
		G_pwm_pct_value-=1;
	}
}

void bt_pwm_on_touch(void){
	//PWM TOOGLE

	if(G_pwm_output_status==0){
		G_pwm_output_status=1;
		strcpy(bt_pwm_on.text,"ON");
	}else{
		G_pwm_output_status=0;
		strcpy(bt_pwm_on.text,"OFF");
	}
}

void bt_pwm_set_100_touch(void){
	G_pwm_pct_value=100;
}
void bt_pwm_set_75_touch(void){
	G_pwm_pct_value=75;
}
void bt_pwm_set_50_touch(void){
	G_pwm_pct_value=50;
}
void bt_pwm_set_25_touch(void){
	G_pwm_pct_value=25;
}
void bt_pwm_set_0_touch(void){
	G_pwm_pct_value=0;
}

void bt_setting_save_touch(void){

}

/*
void UpdateGraph(int val){

    FR_DATA_POINT_T* this_point;


    rotate_first_point_to_end(&(temp_plot.Data));
	this_point=temp_plot.Data.first_point->next;

	while(this_point!=NULL){
		this_point->x=(this_point->x-1);

		this_point=this_point->next;

	}

	temp_plot.Data.first_point->x=0;

	temp_plot.Data.first_point->y=val;


}*/

/*************************** End of file ****************************/
