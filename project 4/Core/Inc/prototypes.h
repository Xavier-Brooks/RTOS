#include "stdio.h"
#include "stdint.h"

//function prototypes
void populate_array(void);
void write_dac(int idx);
void read_adc(int idx, int value);
int get_cnt(void);
int get_adc(void);
int get_dac(void);
int get_idx(void);
void print(void);

//defines
#define BUFFER_SIZE 8190

//variables
