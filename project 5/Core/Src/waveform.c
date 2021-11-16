#include "main.h"

int sample(double min, double max, int* dacmn, int* dacmx){
	*dacmn = (int) ((min * 4095)/3.3); //assigns the minimum voltage the dac can output
	*dacmx = (int) ((max * 4095)/3.3); //assigns maximum voltage the dac can output
	return (dacmx - dacmn); // returns the number of samples
}

void update_timer(TIM_HandleTypeDef timer, int freq, int sample){
	HALDAC_
	HAL_TIM_BASE_Stop(&timer);

	return;
}


