#include "cmd.h"

char s[50];

/*
 * Gets a command from the user, non blocking
 * */
void get_cmd(void){
	//static string that will retain it's value until user presses enter
	//enters while loop as user types
	if((USART2->ISR & USART_ISR_RXNE)){
		char ch = usart2_getc(); //gets characters from user
		//if enter
		if(ch == '\r' || ch == '\n'){
			if(verify_cmd(s)){
				exec_cmd(s);
				usart2_put("\r\n>");
				memset(s, '\0', sizeof(s));
			}else{
				usart2_put("\r\nInvalid Instruction Please Try Again\r\n>");
				memset(s, '\0', sizeof(s));
			}
		}
		else if(ch == 0x7F){ //if backspace key hit
			s[strlen(s-1)] = '\0'; //clears the last character in the string
			usart2_putc(0x7F); //performs a backspace in putty
		}
		else{
			usart2_putc(ch); //print the typed character
			strncat(s, &ch, 1); //adds the last pressed key to the instruction sring
		}
	}
	return;
}

/*
 * This Functions verifies the user typed a proper cmd to be executed
 *
 * PARAMS: char* s - string that holds the users input
 * returns: 1 if input was valid, else 0
 * */
int verify_cmd(char* s){
	//check size - should have a lower and upper case letter
	int x = strlen(s);
	if(x != 2 || strstr(s, "x") || strstr(s, "X")){
		return 0;
	}
	if(strchr("PCRLNB", s[0]) && strchr("pcrlnb", s[1])){
		return 1;
	}
	return 0;
}
