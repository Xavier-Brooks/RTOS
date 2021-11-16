//official headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

//user defined headers
#include "../inc/post.h"
#include "../inc//timers.h"

//gets the new lower limit from the user
int set_timer_base(int min);
//receives a line of text from the user
void get_line(char* buf, int len);
//ask user if they would like to readjust limits
int request_adj(int per);
//prints current minimum
void print_num(int min);
//records data
void record(uint32_t* buffer);
//print the results
void print_hist(uint32_t* resuults);
//handles input captures for the timer
void TIM2_IRQHandler(void);
