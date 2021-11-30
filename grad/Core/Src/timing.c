#include "main.h"

void pulse(){
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	  delayus(10);
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	  return;
}
