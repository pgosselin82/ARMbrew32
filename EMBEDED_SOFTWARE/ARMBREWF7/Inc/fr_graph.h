#ifndef Frank_Graph_H
#define Frank_Graph_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "GUI.h"
#include "tm_stm32_touch.h"

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	point_t Starting_pos;
	int minEU;
	int maxEU;
	int w;
	int main_ticks;
	int secondary_ticks;
} xaxis_t;

typedef struct {
	point_t Starting_pos;
	int minEU;
	int maxEU;
	int h;
	int main_ticks;
	int secondary_ticks;
} yaxis_t;

typedef struct FR_DATA_POINT_T FR_DATA_POINT_T;

struct FR_DATA_POINT_T
{
	FR_DATA_POINT_T* next;
	int x;
	int y;
};

/*typedef struct {
	FR_DATA_POINT_T* next;
	int x;
	int y;
} FR_DATA_POINT_T;
*/
typedef struct {

	FR_DATA_POINT_T* first_point;
	FR_DATA_POINT_T* last_point;

	int pointQTY;
} FR_DATA_T;

typedef struct {
	int x;
	int y;
	int w;
	int h;
	char Title[50];

	xaxis_t x_axis;
	yaxis_t y_axis;

	point_t zero_loc;

	int touchstatus;
	int touchstatus_mem;

	FR_DATA_T Data;


	TM_TOUCH_t* TS;
	void (*TouchAction)(void);
	void (*ReleaseAction)(void);
} FR_GUI_GRAPH_T;




//void gui_graph_init(void);

void Draw_graph_struct(FR_GUI_GRAPH_T* gr);

void add_data_point(FR_DATA_T* data_struct,int x,int y);
void remove_data_point_beginning(FR_DATA_T* data_struct);


/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif
