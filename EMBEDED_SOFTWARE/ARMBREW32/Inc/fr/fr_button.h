#ifndef Frank_buttons_H
#define Frank_buttons_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//#include "GUI.h"
#include "tm_stm32_touch.h"
#include "strings.h"

typedef struct {
	int x;
	int y;
	int w;
	int h;
	char text[20];
	int touchstatus;
	int touchstatus_mem;
	TM_TOUCH_t* TS;

	uint32_t Contour_color;
	uint32_t Filled_color;
	uint32_t Touched_fill_color;
	uint32_t Touched_text_color;
	uint32_t text_color;

	void (*TouchAction)(void);
	void (*ReleaseAction)(void);
}GUI_BUTTON;

void Draw_button(int x,int y,int w,int h,const char* text,uint32_t color,uint32_t fill_color,uint32_t textcolor);

void Draw_button_struct(GUI_BUTTON* bt);

void GUI_BUTTON_INIT(GUI_BUTTON* Button,int x,int y,int w,int h,const char* text,TM_TOUCH_t* TS,
		uint32_t Contour_color,uint32_t Touched_fill_color,uint32_t Touched_text_color,uint32_t text_color,
void (*TouchAction)(void),void (*ReleaseAction)(void));







/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif
