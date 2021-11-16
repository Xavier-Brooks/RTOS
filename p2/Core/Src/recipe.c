#include "main.h"

//keeps track of how far back instruction counter should jump when it reaches the end of each loop
int Branch = 0;
int Branch2 = 0;

//recipe buffers for both of the servos
char recipe1[20] = {(MOV | 1), (MOV | 6), (MOV | 1), (MOV | 4),  (LOOP | 0), (MOV | 1), (MOV | 5), (LOOPE), (MOV | 1), (MOV | 3), (WAIT | 0),(MOV | 3), (MOV | 4), (WAIT | 31),(WAIT | 31), (WAIT | 31), (MOV | 1)}; //main demo recipe to be executed simultaneously
char recipe2[10] = {(LOOP | 2), (MOV | 1), (MOV | 2), (MOV | 3), (MOV | 4),(MOV | 5), (MOV | 6), (LOOPE), END, (MOV | 1)}; //test continue functoinality

//structure thta holds recipe information for each of the servos
typedef struct{
	char* instructions;
	int ins_ptr; //points to the current instruction
	int pos; //keeps track of the current position of a servo(betweenn 1 and 6)
	int loop_cnt; //keeps track of the number of loops that have executed in a recipe
	int loop_started; //used to determine if a loop is already running(avoid nested loops)
	int paused; //paused recipe(or not)
}recipe;

//holds recipe metadata for both servos
recipe rec[2] = {{.instructions = recipe1 , .ins_ptr = 0, .pos = 2, .loop_cnt = 0, .loop_started = 0, .paused = 1}, //servo 1, index 0
				 {.instructions = recipe2, .ins_ptr = 0, .pos = 2, .loop_cnt = 0, .loop_started = 0, .paused = 1} //servo 2, index 1
};

//executes commands from the user
void exec_cmd(char* cmd){
	char ch;
	ch = cmd[0];
	operation(1, ch); //executes command for first servo, specified by first letter (Capital)
	ch = cmd[1];
	operation(2, ch); //executes command for second servo, specified by second letter (lower)
	return;
}

/*
 * This function execute the instructions in a recipe for a servo(1 or 2)
 * */
void exec_recipe(int servo){
while(!(USART2->ISR & USART_ISR_RXNE) ){
	int index = servo - 1;
	int opcode = (rec[index].instructions[rec[index].ins_ptr] & 0xE0); //extracts the opcode from the instruction
	int params = (rec[index].instructions[rec[index].ins_ptr] & 0x1F); //extracts the opcode from the instruction
	if(rec[index].paused == 1){return;}

	switch(opcode){
		case(0x20): //MOV
				move_servo(servo, rec[index].pos, params);
				int diff = abs(rec[index].pos - params);
				HAL_Delay(diff*100);
				//updates the servo position
				rec[index].pos = params;
				//updates instruction counter
				rec[index].ins_ptr++;
				break;
		case(0x40): //WAIT
				HAL_Delay((params*100) + 100); //delays
				rec[index].ins_ptr++;
				break;
		case(0x80): //LOOP
				if(rec[index].loop_started){//detects if a nested loop has been encountered
					USART_Write(huart2.Instance, (uint8_t *)"LOOP ERROR\r\n>", strlen("LOOP ERROR\r\n>"));
					rec[index].paused = 1;
					rec[index].ins_ptr += 1;
					break;
				}
				rec[index].loop_started = 1; //stes the flag to notify that a loop is currently running
				rec[index].loop_cnt = params; //keeps track of the number of loops left to execute
				rec[index].ins_ptr += 1;

				//instruction pointer jumps to first instruction in a loop
				if(index == 0){Branch = rec[index].ins_ptr;}
				Branch2 = rec[index].ins_ptr;
				break;
		case(0xA0): //LOOP END
				//if the end of the loop is met and there are more iterations
				if(rec[index].loop_started && rec[index].loop_cnt > 0){
					//adjust the program counter for each of teh servos
					if(index == 0){
						rec[index].ins_ptr = Branch;
					}else{
						rec[index].ins_ptr = Branch2;
					}
					rec[index].loop_cnt -= 1;
					break;
				}
				//done with looping go to next instruction after
				rec[index].loop_started = 0;
				rec[index].ins_ptr += 1;
				break;
		default: //END OF RECIPE
			rec[index].paused = 1;
			return;
	}
  }
}

void operation(int servo, char opt){
	int index = servo-1; //index for the rec structure that holds servo 1/2 data

	//simply moves the servos or executes the commands as specified at program startup
	switch(opt){
		case 'B':
		case 'b':
			rec[index].ins_ptr = 0;
			rec[index].paused = 0;
			break;
		case 'P':
		case 'p':
			rec[index].paused = 1;
			break;
		case 'L':
		case 'l':
			move_servo(servo, rec[index].pos, (rec[index].pos-1));
			if(rec[index].pos <= 6 && rec[index].pos > 1){ //keep the servo pos value with proper bounds
				rec[index].pos -= 1;
			}
			break;
		case 'R':
		case 'r':
			move_servo(servo, rec[index].pos, (rec[index].pos+1));
			if(rec[index].pos < 6 && rec[index].pos >= 1){ //keeps servo position with bounds
				rec[index].pos += 1;
			}
			break;
		case 'C':
		case 'c':
			rec[index].paused = 0;
			break;
		default: //No-op
			return;
	}
}

/*
 * Adjust the duty cycle to reposition the servos from right to left(1 to 6)
 * */
void move_servo(int servo, int current_pos, int new_pos){
	if(new_pos > 6 || new_pos < 1){ //if the servo motor is in the farthest position left/right do nothing
		return;
	}
	switch(new_pos){
	case (1):
		if(servo == 1){
			SERVO1 = (PERIOD/10);
			break;
		}
		SERVO2 = (PERIOD/10);
		break;
	case (2):
		if(servo == 1){
			SERVO1 = (PERIOD/11);
			break;
		}
		SERVO2 = (PERIOD/11);
		break;
	case (3):
		if(servo == 1){
			SERVO1 = (PERIOD/12);
			break;
		}
		SERVO2 = (PERIOD/12);
		break;
	case (4):
		if(servo == 1){
			SERVO1 = (PERIOD/14);
			break;
		}
		SERVO2 = (PERIOD/14);
		break;
	case (5):
		if(servo == 1){
			SERVO1 = (PERIOD/16);
			break;
		}
		SERVO2 = (PERIOD/16);
		break;
	case (6):
		if(servo == 1){
			SERVO1 = (PERIOD/18);
			break;
		}
		SERVO2 = (PERIOD/18);
		break;
	}
}
