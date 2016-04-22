
#include "rotPID.h"

#include "stm32f4xx_hal.h"

void PI_init(rot_PI_t* block,double KP,double KI,uint16_t TE,double CVmax,double CVmin){
	rot_INTEGRAL_t* intblock;

	intblock=&(block->Integral);

	block->Bias=0;
	integral_init(intblock,TE);
	block->TE=TE;
	block->Kp=KP;
	block->Ki=KI;
	block->CVmin=CVmin;
	block->CVmax=CVmax;
	block->Saturated=0;
	block->Integral_set=0;
	block->Reset=0;
	block->SP=0;
	block->PV=0;
	block->E_1=0;
}

void PI_reset(rot_PI_t* block){
	rot_INTEGRAL_t* intblock;

	intblock=&(block->Integral);

	block->E_1=0;
	block->CV=0;
	block->CVbsat=0;
	block->Saturated=0;
	block->Bias=0;
	integral_reset(intblock);

	block->Reset=0;
}


double PI_calc(rot_PI_t* block){

	rot_INTEGRAL_t* intblock;

	intblock=&(block->Integral);

	if(block->Reset){
		PI_reset(block);
	}

	//Calculate Error
	block->E=(block->SP-block->PV);

	//Calculate Proportionnal compensation
	block->Cp=(block->Kp*block->E);

	//Manage integral Set
	if(block->Integral_set){
		intblock->Set=block->Integral_set;
		intblock->SetValue=block->Integral_set_value;

		block->Integral_set=0;
	}
	intblock->TE=block->TE;

	//Manage Integral Component
	// Do not integrate when Saturated
	if(block->Saturated){
		integral_calc(intblock,0);
	}else{
		integral_calc(intblock,(block->Ki*block->E));
	}
	//Calculate Integral compensation
	block->Ci=intblock->Output;

	block->CV=block->Ci+block->Cp+block->Bias;

	block->CVbsat=block->CV;

	//SATURATE HERE
	//CV:=SATREAL(CV,DINT_TO_REAL(CVmax),DINT_TO_REAL(CVmin));


	if(block->CVbsat!=block->CV){
		block->Saturated=1;
	}else{
		block->Saturated=0;
	}

	block->E_1=block->E;

	return block->CV;
}






void integral_init(rot_INTEGRAL_t* block,uint16_t TE){
	block->Input=0;
	block->Input_1=0;
	block->Output=0;
	block->Output_1=0;
	block->Reset=0;
	block->Set=0;
	block->SetValue=0;
	block->TE=TE;
}

double integral_calc(rot_INTEGRAL_t * block,double input){
	block->Input=input;

	if(block->Reset){
		integral_reset(block);
	}else{
		if(block->Set){
			block->Output_1=block->SetValue;
			block->Set=0;
		}

		// TE is expressed in mS so /1000 is needed
		// Trapexoidal Aproximation
		//(T*(in+in1)+(2*out_1))/2
		block->Output=(((double)(block->TE)/1000)*(block->Input+block->Input_1)+(2*block->Output_1))/2;

		block->Input_1=block->Input;
		block->Output_1=block->Output;

		// If the result is too close to 0, we will interpret it as 0
		if((block->Output)<0.0001 && (block->Output)>-0.0001){
			block->Output=0;
			block->Output_1=0;
		}
	}

	return block->Output;
}

void integral_reset(rot_INTEGRAL_t* block){
	block->Reset=0;
	block->Input_1=0;
	block->Output=0;
	block->Output_1=0;
}

