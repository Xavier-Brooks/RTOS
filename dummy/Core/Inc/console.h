//defines
#define BUFFER_SETTING_SIZE 20

//function prototypes
void startup(UART_HandleTypeDef uart);
void get_cmd(UART_HandleTypeDef uart, char* buffer);

void parse(char* str, char* types, int* frequency, double* minimum, double* maximum);
void assign(char* str, int c, int* frequency, double* minimum, double* maximum);
int check_input(void);
