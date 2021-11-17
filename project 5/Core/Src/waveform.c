#include "main.h"

void sample(double min, double max, int* dacmn, int* dacmx, int* samp){
	*dacmn = (int) ((min * 4095)/3.3); //assigns the minimum voltage the dac can output
	*dacmx = (int) ((max * 4095)/3.3); //assigns maximum voltage the dac can output
	*samp =  2 * (*dacmx - *dacmn); // returns the number of samples
	return;
}

void update_timer(TIM_HandleTypeDef* timer, DAC_HandleTypeDef* dac, int freq, int sample){
	//stops the timer and dac until we are finished updating
	HAL_DAC_Stop_DMA(dac, DAC_CHANNEL_1);
	HAL_TIM_Base_Stop(timer);
	int x = (int) (SCC/(freq * sample));
	x = sqrt(x) - 1;
	(timer->Init).Period = x;
	(timer->Init).Prescaler = x;
	return;
}

void update_data(TIM_HandleTypeDef timer, DAC_HandleTypeDef dac, uint16_t buffer, char form, int samples, int min, int max){
	switch (form){
	case 't':
	case 'T':
		populate_triangle_wave(buffer, samples, min, max);
		break;
	case 'r':
	case 'R':
		populate_rectangle_wave(buffer, samples, min, max);
		break;
	case 's':
	case 'S':
		break;
	default:
		break;
	}
	HAL_DAC_Start_DMA(&dac, DAC_CHANNEL_1, buffer, samples, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(&timer);
	return;
}

void populate_triangle_wave(uint16_t* buffer, int samples, int min, int max){
	int idx = (samples/2);
	int temp = max;
	for(int i = 0; i < idx; i++){
		buffer[i] = min + i;
	}
	for(int j = idx; j < samples; j++){
		buffer[j] = temp;
		temp--;
	}
}

void populate_rectangle_wave(uint16_t* buffer, int samples, int min, int max){
	int idx = (samples/2);
	for(int i = 0; i < idx; i++){
		buffer[i] = max;
	}
	for(int j = idx; j < samples; j++){
		buffer[j] = min;
	}
}
