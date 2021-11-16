#include "main.h"

char s[50];

/*
 * Gets a command from the user, non blocking
 * */
void get_cmd(UART_HandleTypeDef huart){
	//static string that will retain it's value until user presses enter
	//enters while loop as user types
	if((USART2->ISR & USART_ISR_RXNE)){
		char ch = USART_Read(huart.Instance); //gets characters from user
		//if enter
		if(ch == '\r' || ch == '\n'){
			if(verify_cmd(s)){ //checks if the command is legit
				exec_cmd(s);
				USART_Write(huart.Instance, "\r\n>", strlen("\r\n>"));
				memset(s, '\0', sizeof(s));
			}else{
				USART_Write(huart.Instance,"\r\nInvalid/Canceled Instruction - Try Another\r\n>", strlen("\r\nInvalid/Canceled Instruction - Try Another\r\n>"));
				memset(s, '\0', sizeof(s));
			}
		}
		else if(ch == 0x7F){ //if backspace key hit
			s[strlen(s-1)] = '\0'; //clears the last character in the string
			USART_Write(huart.Instance, 0x7F, 1); //removes character from putty terminal
		}
		else{
			USART_Write(huart.Instance, &ch, 1); //prints character
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
	//if 'x' is in the string or their aren't 2 characters then return its a cancelled improper command
	if(x != 2 || strstr(s, "x") || strstr(s, "X")){
		return 0;
	}

	//returns null if the character isn't in the string specified by the first parameter
	//checking if the first character is capital and one of the proper letters, same for the lower case
	if(strchr("PCRLNB", s[0]) && strchr("pcrlnb", s[1])){
		return 1;
	}
	return 0;
}
