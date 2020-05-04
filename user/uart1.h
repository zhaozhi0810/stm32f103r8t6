
#include <stm32f10x.h>
#include <stdio.h>
//typedef void FILE ;


void uart1_init(uint32_t bandrate);
void uart1_int_init(uint32_t bandrate);


void uart1_putchar(uint8_t dat);
void uart1_putstr(uint8_t dat[]);
uint8_t	uart1_getchar(void);

int fputc(int ch, FILE *f);


extern uint8_t recv_buf[8];   //�����洢���ڵ�����
extern uint8_t recv_flag; //���ձ�־��Ϊ1ʱ��ʾ���յ������ݣ�Ϊ0��ʱ���ʾû�н��յ�����




