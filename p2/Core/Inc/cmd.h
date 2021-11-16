#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "usart.h"
#include "main.h"
#include "recipe.h"

void get_cmd(UART_HandleTypeDef huart);
int verify_cmd(char* s);
