

#include <stm32f10x.h>



void my_systick_init(void);

//�жϷ�ʽ
//���� �� sysclock AHBʱ�ӣ���λΪMHz
void my_systick_int_init(uint8_t sysclock);


/*
	us ����ʱʱ��798915us 
	//ȡһ���������ü���

	us����ʱ���ܳ���70,0000us

*/
void delay_us(uint32_t dus);


/*
	������ʱ
*/
void delay_ms(uint32_t dms);

