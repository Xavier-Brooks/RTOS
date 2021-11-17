#include "stm32l4xx_hal.h"
#define WAVE_BUFFER_SIZE 8190
#define SCC 64000000

void sample(double min, double max, int* dacmn, int* dacmx, int* samp);
void update_timer(TIM_HandleTypeDef* timer, DAC_HandleTypeDef* dac, int freq, int sample);
void update_data(TIM_HandleTypeDef timer, DAC_HandleTypeDef dac, uint16_t buffer, char form, int samples, int min, int max);
void populate_triangle_wave(uint16_t* buffer, int samples, int min, int max);
void populate_rectangle_wave(uint16_t* buffer, int samples, int min, int max);
