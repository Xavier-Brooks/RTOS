#include "stm32l476xx.h"

//opcodes for the instructions
#define MOV 0x20
#define WAIT 0x40
#define LOOP 0x80
#define LOOPE 0xA0
#define END 0x00

#define PERIOD 20000u

//Duty cycle registers for servos
#define SERVO1 TIM4->CCR1
#define SERVO2 TIM4->CCR2

void operation(int servo, char opt);
void exec_cmd(char* cmd);
void exec_recipe(int servo);
void move_servo(int servo, int current_pos,int new_pos);
