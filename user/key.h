

#include <stm32f10x.h>



void key_init(void);
uint8_t get_key_val(void);
void key_int_init(void);


extern uint8_t key_flag;   //��0��ʾ��⵽�������£�0��ʾû�м�⵽����

void key_scan(void);



