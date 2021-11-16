#include "stm32l4xx_hal.h"
#define WAVE_BUFFER_SIZE 8190

int sample(double min, double max, int* dacmn, int* dacmx);
void update_timer(TIM_HandleTypeDef timer, int freq, int sample);
void update_data(void);

