#include "bank.h"
#include "main.h"

SemaphoreHandle_t print_mutex; //used to determin if tellet is available
void mutex_sprintf(char* str, int teller_ins, int served);

//variables
extern TELLER_t tellers[NUM_TELLERS];
TELLER_t tellers[NUM_TELLERS];

customer_queue customers;

// private methods
static void teller_task(void *params);
static void customer_task(void* param);

/**
 * This public method creates a number of teller tasks
 */
int teller_task_init(int num_tellers) {
	for(int ii=0; ii<num_tellers; ii++) {
		TELLER_t *t = &tellers[ii];
		t->available = xSemaphoreCreateMutex();
		memset(t, 0, sizeof(TELLER_t));
		t->instance = ii;
		BaseType_t err = xTaskCreate(teller_task, "TellerTask", 128, &tellers[ii], 12, NULL);
		assert(err == pdPASS);
	}
	return 0;
}

int customer_queue_init(void){
	  customers.mutex = xSemaphoreCreateMutex(); //creates mutex to handle queue data
	  xSemaphoreTake(customers.mutex, (TickType_t) 10);
	  customers.queue = xQueueCreate(25, sizeof(CUSTOMER_t)); //creates the space in memory for the queue
	  xSemaphoreGive(customers.mutex);
	  BaseType_t err = xTaskCreate(customer_task, "CustomerTask", 128, &customers, 12, NULL);
	  assert(err == pdPASS);
}

/**/
static void teller_task(void *param) {
	TELLER_t *t = (TELLER_t *)param;
	int instance = t->instance+1;	// which teller am I?  1, 2, or 3?

	char *nl = "";
	if(instance==1)
		nl = "\r\n";				// newline if instance == 1

	while(1) {
		vTaskDelay(instance*1000);
		BaseType_t pd = xSemaphoreTake(t->available, (TickType_t) 10);
		if(pd == pdPASS){
			help_customer();
			t->customers_served += 1;
			xSemaphoreGive(t->available);
		}

		//mutex_sprintf(nl, t->instance, t->customers_served);
	}
}

static void customer_task(void* params){
	while(1){
		customer_queue* p = (customer_queue *)params;
		vTaskDelay(rand()*1000);
		CUSTOMER_t c;
		xQueueSendToBack(p->queue, &c, (TickType_t) 10);
		xSemaphoreTake(p->mutex, (TickType_t) 10);
		p->len += 1;
		xSemaphoreGive(p->mutex);
	}
}

int help_customer(){
	CUSTOMER_t c;
	xQueueReceive(customers.queue, &c, (TickType_t) 10);
	xSemaphoreTake(customers.mutex, (TickType_t) 10);
	customers.len -= 1;
	xSemaphoreGive(customers.mutex);
	return 0;
}

void mutex_sprintf(char* str, int teller_ins, int served){
	char buffer[20];				// for printing

	xSemaphoreTake(print_mutex, (TickType_t) 10);
	int len = sprintf(buffer, "%sTeller %d, %d \r\n", str, teller_ins, served);
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, ~0);
	xSemaphoreGive(print_mutex);
	return;
}
