#ifndef Frank_disp_led_H
#define Frank_disp_led_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//#include "GUI.h"
#include "tm_stm32_touch.h"


typedef struct {
	int x;
	int y;
	int d;

	uint32_t Contour_color;
	uint32_t Fill_color;

}GUI_LED;

void Draw_led_struct(GUI_LED* led);

void GUI_LED_INIT(GUI_LED* led,int x,int y,int d,uint32_t Contour_color,uint32_t Fill_color);







/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif
