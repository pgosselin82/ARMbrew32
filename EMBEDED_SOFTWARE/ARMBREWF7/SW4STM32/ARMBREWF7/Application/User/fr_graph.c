/*
 * Graph.c
 *
 *  Created on: Apr 8, 2016
 *      Author: Rotule
 */

#include "fr_graph.h"

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void drawpoint(FR_DATA_POINT_T* actualpoint,FR_DATA_POINT_T* nextpoint,FR_GUI_GRAPH_T* gr);
void draw_data(FR_GUI_GRAPH_T* gr,FR_DATA_T* data_struct);

void Draw_graph_struct(FR_GUI_GRAPH_T* gr){

	TM_TOUCH_t* TS;
	point_t* zero_loc;


	TS=gr->TS;
	zero_loc=&(gr->zero_loc);

	// Touch Status
	gr->touchstatus=0;
		if (TS->NumPresses) {
			int i;
			/* Go through all presses on LCD */
			for (i = 0; i < TS->NumPresses; i++) {
				/* Draw circle */
			    int xtouch;
			    int ytouch;

			    xtouch=LCD_GetXSize()-TS->X[i];
			    ytouch=LCD_GetYSize()-TS->Y[i];
			    if((xtouch>(gr->x))&&(xtouch<((gr->x)+(gr->w)))&&(ytouch>(gr->y))&&(ytouch<((gr->y)+(gr->h)))){
			    	gr->touchstatus=1;
			    }
			}

		}


		//Draw
		zero_loc->x=gr->x;
		zero_loc->y=gr->y+gr->h;

		//x_axis
		GUI_DrawLine(zero_loc->x,zero_loc->y,gr->x+gr->w,gr->zero_loc.y);
		//Arrow
		GUI_DrawLine(zero_loc->x+gr->w,zero_loc->y+5,zero_loc->x+gr->w,zero_loc->y-5);
		GUI_DrawLine(zero_loc->x+gr->w+5,zero_loc->y,zero_loc->x+gr->w,zero_loc->y+5);
		GUI_DrawLine(zero_loc->x+gr->w+5,zero_loc->y,zero_loc->x+gr->w,zero_loc->y-5);

		// Draw Xaxis function should be called here
		//Draw axis Label and Ticks

		//Y_axis
		GUI_DrawLine(gr->zero_loc.x,gr->zero_loc.y,gr->zero_loc.x,gr->y);

		//Arrow
		GUI_DrawLine(zero_loc->x-5,gr->y,zero_loc->x+5,gr->y);
		GUI_DrawLine(zero_loc->x-5,gr->y,zero_loc->x,gr->y-5);
		GUI_DrawLine(zero_loc->x+5,gr->y,zero_loc->x,gr->y-5);

		// Draw Yaxis function should be called here
		//Draw axis Label and Ticks

		//Draw Graph title
		GUI_SetFont(&GUI_Font16_1);
		GUI_DispStringHCenterAt(gr->Title,gr->x+((gr->w)/2),(gr->y)-16);

		//Draw Graph here

		draw_data(gr,&(gr->Data));


		/* Do actions */
		if(gr->touchstatus && !(gr->touchstatus_mem) && gr->TouchAction!=NULL){
			gr->TouchAction();
		}

		if(!(gr->touchstatus) && gr->touchstatus_mem && gr->ReleaseAction!=NULL){
			gr->ReleaseAction();
		}
		gr->touchstatus_mem=gr->touchstatus;


}


void draw_data(FR_GUI_GRAPH_T* gr,FR_DATA_T* data_struct){




	if(data_struct->pointQTY>=2){

		FR_DATA_POINT_T* actualpoint;
		FR_DATA_POINT_T* nextpoint;

		actualpoint=data_struct->first_point;

		do{
			nextpoint=actualpoint->next;

			//GUI_DrawLine(actualpoint->x,actualpoint->y,nextpoint->x,nextpoint->y);

			drawpoint(actualpoint,nextpoint,gr);

			actualpoint=nextpoint;
		}while(actualpoint!=NULL);





	}



}

void drawpoint(FR_DATA_POINT_T* actualpoint,FR_DATA_POINT_T* nextpoint,FR_GUI_GRAPH_T* gr){
	xaxis_t* xaxis;
	yaxis_t* yaxis;
	int x0,y0,x1,y1;

	xaxis=&(gr->x_axis);
	yaxis=&(gr->y_axis);

	if(actualpoint->x>xaxis->minEU && actualpoint->x<xaxis->maxEU && nextpoint->x>xaxis->minEU && nextpoint->x<xaxis->maxEU && actualpoint->y>yaxis->minEU && actualpoint->y<yaxis->maxEU && nextpoint->y>yaxis->minEU && nextpoint->y<yaxis->maxEU){

		int deltaXEU;
		int deltaYEU;


		deltaXEU=abs(xaxis->maxEU-xaxis->minEU);
		deltaYEU=abs(yaxis->maxEU-yaxis->minEU);

		x0=(int)((((double)(actualpoint->x-xaxis->minEU)/deltaXEU)*gr->w)+gr->x);
		y0=(int)((-((double)(actualpoint->y-yaxis->minEU)/deltaYEU)*gr->h)+gr->y+gr->h);

		x1=(int)((((double)(nextpoint->x-xaxis->minEU)/deltaXEU)*gr->w)+gr->x);
		y1=(int)((-((double)(nextpoint->y-yaxis->minEU)/deltaYEU)*gr->h)+gr->y+gr->h);

		GUI_DrawLine(x0,y0,x1,y1);
	}



}

void add_data_point(FR_DATA_T* data_struct,int x,int y){
	FR_DATA_POINT_T* newpoint;

	newpoint=malloc(sizeof(FR_DATA_POINT_T));

	if(newpoint!=NULL){

	newpoint->x=x;
	newpoint->y=y;
	newpoint->next=NULL;

	if(data_struct->pointQTY){
		FR_DATA_POINT_T* lastpoint;
		lastpoint=data_struct->last_point;

		lastpoint->next=newpoint;
		data_struct->last_point=newpoint;

	}else{
		data_struct->first_point=newpoint;
		data_struct->last_point=newpoint;

	}
	newpoint->next=NULL;
	data_struct->pointQTY+=1;
}
}


void remove_data_point_beginning(FR_DATA_T* data_struct){



	if(data_struct->pointQTY){
		FR_DATA_POINT_T* firstpoint;

		firstpoint=data_struct->first_point;

		data_struct->first_point=firstpoint->next;

		free(firstpoint);

	}


}

