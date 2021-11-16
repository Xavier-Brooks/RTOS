#include "main.h"

int curr_min = 50;
static uint32_t histogram[101];
static int buckets  = 0;

int main(void) {
	//perform some hardware initialization here1
	usart2_init();
	gpio_init();
	usart2_putc(0x0C); //clears putty terminal

	usart2_put("Will begin post test...\r\n");

    // Main loop runs forever
    while(1)
    {
    	while(!post()){
    		usart2_put("post test has failed please, press any key to re-run\r\n");
    		wait();
    	}
    	usart2_put("post was successful");
		usart2_put("The Current Lower Limit for the Program is: ");
		print_num(curr_min);
		usart2_put("\r\n");

    	//prompts the user if they would like to update settings
    	curr_min = request_adj(curr_min);

    	//start recording
    	record(histogram);

    	//print results.
    	print_hist(histogram);
    	wait();
    }
}

void print_hist(uint32_t* results){
	 char s[20] = {0};
	for(int i = 0; i < 100; i++){
		sprintf(s, "%d:%d\r\n",(int)(i + curr_min),(int)results[i]);
		usart2_put(s);
	}
	memset(results, '0', 100);
}

void record(uint32_t* buff){
	int start_flag = 0;
	uint32_t temprg = TIM2->CCR1;
	memset(buff, '\0', (sizeof(uint32_t)*101));
	for(int buckets = 0; buckets < 1000; buckets++){
		if(!start_flag && (TIM2->SR & TIM_SR_CC1IF)){
			start_flag = 1;
			temprg = TIM2->CCR1;
			continue;
		}
		else if(start_flag && (TIM2->SR & TIM_SR_CC1IF)){
			temprg = TIM2->CCR1 - temprg;
			if(((temprg - curr_min) > 99) || ((temprg - curr_min) < 0)){
				break;
			}
			buff[temprg - (uint32_t)curr_min] += 1;
		}
	}
	return;
}

//retrieves the new lower limit from the user 
int set_timer_base(int min){
	char input[6] = {0};
	//runs until user gives proper lower limit
	while(1){
		usart2_put("Please Provide a Lower Limit Between 50 & 9950us\r\n");
		usart2_put("The Lower Limit: ");
		get_line(input, 6);
		min = atoi(input);
		if(min >= 50 && min <= 9950){break;}
	}
	return min;
}

//gets a string of text from the user in teh putty terminal
void get_line(char* buf, int len){
	int i = 0;
	while(i < len){
		char ch = usart2_getc();
		if(ch == '\r' || ch == '\n'){
			usart2_put("\r\n");
			break;
		}
		else if(ch < 48 || ch > 57){
			i--;
			continue;
		}
		usart2_putc(ch);
		buf[i] = ch;
		i++;
	}
	usart2_put("\r\n");
}

//prompts the user if they want to change limit settings and responds accordingly
int request_adj(int per){
	char c = 0x00;
	usart2_put("The Current lower limit is: ");
	print_num(per);
	usart2_put("\r\nWould You Like To Update This Limit (y/n)?: ");
	while(c == 0x00){
		c = usart2_getc();
		if(c == 'y'){
			usart2_putc(c);
			usart2_put("\r\n");
			per = set_timer_base(per);
			return per;
		}
		else if(c == 'n'){
			usart2_putc(c);
			break;
		}
		c = 0x00;
		continue; //do nothing and print nothing until y/n is typed
	}
	return per; 
}

void print_num(int min){
	char num[10] = {0};
	sprintf(num, "%d", min);
	usart2_put(num);
}


