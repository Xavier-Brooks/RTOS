#include "main.h"

/*
 * Simple function that prints a guide to Putty upon startup
 * RET: nothing
 * */
void startup(UART_HandleTypeDef uart){
	char *msg = "Welcome: Type freq min max\r\nType - type of wave S(sine) T(triangle) R(Rectangle)\r\n";
	char *msg2 = "freq - frequency of the wave (10Hz - 100kHz)\r\nmin - minimum voltage allowed(0V limit)\r\nmax - maximum voltage allowed(3.3V limit)\r\nMax Must Be Greater Than Min\r\n";
	USART_Write(uart.Instance, (uint8_t *)msg, strlen(msg));
	USART_Write(uart.Instance, (uint8_t *)msg2, strlen(msg2));
}

/*
 * Gets the waveform settings from the user and verifies they are proper
 * PARAMS
 * 	uart - the usart peripheral being used for printing
 * 	buffer - the string buffer the waveform settings are being written to
 * 	size - the size of the buffer being written too
 *
 * RETURN
 * 	1 if the settings are valid, 0 otherwise
 * */
void get_cmd(UART_HandleTypeDef uart, char* buffer, int size){
	//prints a prompt for the user to type the wave settings
	USART_Write(uart.Instance, "Wave Settings: ", strlen("Wave Settings: "));
	//enters while loop as user types
	while(1){
		char ch = USART_Read(uart.Instance); //gets characters from user
		if(ch == '\r' || ch == '\n'){ //if enter is pressed
				USART_Write(uart.Instance, (uint8_t *)"\r\n", strlen("\r\n"));
				return;
		}
		USART_Write(uart.Instance, &ch, 1); //prints character as user types
		strncat(buffer, &ch, 1); //adds the last pressed key to the instruction string
	}
}

void parse(char* str, char* types, int* frequency, double* minimum, double* maximum){
	char* tok = strtok(str, " ");
	*types = (char)(*tok);
	for(int i = 0; i <= 2; i++){
		tok = strtok(NULL, " "); //pull the next parameter
		assign(tok, i, frequency, minimum, maximum);
	}
	memset(str, '\0', sizeof(str)); //clears the buffer for the next waveform settings
	return;
}

void assign(char* str, int c, int* frequency, double* minimum, double* maximum){
	switch (c){
		case 0:
			*frequency = atoi(str);
			break;
		case 1:
			*minimum = (double)atof(str);
			break;
		case 2:
			*maximum = (double)atof(str);
			break;
		default:
			return;
	}
	return;
}

