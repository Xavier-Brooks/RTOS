#include "main.h"

void dc2analog(double min, double max, int* dacmn, int* dacmx){
	*dacmn = (int) ((min * 4095)/3.3); //assigns the minimum voltage the dac can output
	*dacmx = (int) ((max * 4095)/3.3); //assigns maximum voltage the dac can output
	return;
}

void update_timer(TIM_HandleTypeDef* timer, DAC_HandleTypeDef* dac, int freq){
	//stops the timer and dac until we are finished updating
	HAL_DAC_Stop_DMA(dac, DAC_CHANNEL_1);
	HAL_TIM_Base_Stop(timer);
	int x = (int) (SCC/(freq * (WAVE_BUFFER_SIZE/2)));
	x = sqrt(x) - 1;
	(timer->Instance)->ARR = (x/2); //division by calibrates the ARR to produce the proper frequency
	(timer->Instance)->PSC = x;
	return;
}

void update_data(uint32_t* buffer, char form, int min, int max){
	switch (form){
	case 't':
	case 'T':
		populate_triangle_wave(buffer, min, max);
		break;
	case 'r':
	case 'R':
		populate_rectangle_wave(buffer, min, max);
		break;
	case 's':
	case 'S':
		populate_sine_wave(buffer, min, max);
		break;
	default:
		break;
	}
	return;
}

void populate_triangle_wave(uint32_t* buffer, int min, int max){
	int idx = (WAVE_BUFFER_SIZE/2);
	double steps = 2.0*((max-min)/WAVE_BUFFER_SIZE); //calculates the steping size to increment/decrement each sample point and make the wave as smooth as possible
	for(int i = 0; i < idx; i++){
		buffer[i] = (uint32_t)(min + (i*steps));
	}
	for(int j = idx; j < WAVE_BUFFER_SIZE; j++){
		buffer[j] =(uint32_t) max;
		max -= steps;
	}
}

void populate_rectangle_wave(uint32_t* buffer, int min, int max){
	int idx = (WAVE_BUFFER_SIZE/2);
	for(int i = 0; i < idx; i++){
		buffer[i] = max;
	}
	for(int j = idx; j < WAVE_BUFFER_SIZE; j++){
		buffer[j] = min;
	}
}

void populate_sine_wave(uint32_t* buffer, int min, int max){
	double amp = ((max-min)/2); // calculates the amplitude/midpoint of the sine wave so that it fits with the min and max values
	for(int i = 0; i < WAVE_BUFFER_SIZE; i++){
		buffer[i] = (amp * sin(2*i*M_PI/100)) + min + amp;
	}

}
