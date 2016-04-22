
#ifndef rot_PID_H
#define rot_PID_H 100

#include "stm32f4xx_hal.h"

typedef struct {
	double Input;
	uint16_t TE;
	uint8_t Reset;
	uint8_t Set;
	double SetValue;

	double Output;

	double Input_1;
	double Output_1;

} rot_INTEGRAL_t;

typedef struct {
	double SP;
	double PV;
	double CV;
	double E;
	double Bias;

	double CVmin;
	double CVmax;

	double Kp;
	double Ki;

	uint8_t Reset;
	uint8_t Integral_set;
	double Integral_set_value;

	uint16_t TE;


	double E_1;
	double Cp;
	double Ci;

	rot_INTEGRAL_t Integral;

	double CVbsat;
	uint8_t Saturated;

} rot_PI_t;




void PI_init(rot_PI_t* block,double KP,double KI,uint16_t TE,double CVmax,double CVmin);

void PI_reset(rot_PI_t* block);

double PI_calc(rot_PI_t* block);





void integral_init(rot_INTEGRAL_t* block,uint16_t TE);

double integral_calc(rot_INTEGRAL_t* block,double input);

void integral_reset(rot_INTEGRAL_t* block);







#endif
