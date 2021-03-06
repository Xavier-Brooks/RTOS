#include "stm32l4xx_hal.h"
#define WAVE_BUFFER_SIZE 100
#define SCC 64000000

void dc2analog(double min, double max, int* dacmn, int* dacmx);
void update_timer(TIM_HandleTypeDef* timer, DAC_HandleTypeDef* dac, int freq);
void update_data(uint32_t* buffer, char form, int min, int max);
void populate_triangle_wave(uint32_t* buffer, int min, int max);
void populate_rectangle_wave(uint32_t* buffer, int min, int max);
void populate_sine_wave(uint32_t* buffer, int min, int max);
