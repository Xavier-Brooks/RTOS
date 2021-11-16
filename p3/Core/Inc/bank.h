#ifndef INC_TELLER_TASK_H_
#define INC_TELLER_TASK_H_

//includes
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"

//defines
#define NUM_TELLERS 3
#define MAX_NUM_CUSTOMERS 25
#define MAX_CUSTOMER_TIME 800 //800ms yields 8 minutes in simulation time
#define MIN_CUSTOMER_TIME 50  //50ms yields 30 seconds in simulation time

//definition of teller structure
typedef struct {
	// some parameters to track
	int instance;
	int customers_served;
	SemaphoreHandle_t available; //used to determin if tellet is available
} TELLER_t;

//definition of customer structure
typedef struct {
	// some parameters to track
	unsigned int time_in;
	unsigned int time_out;
} CUSTOMER_t;

typedef struct{
	int len;
	int max_time;
	SemaphoreHandle_t mutex;
	QueueHandle_t queue;
}customer_queue;

//function declaration
int teller_task_init(int num);
int customer_queue_init(void);
int help_customer(void);

#endif /* INC_TELLER_TASK_H_ */
