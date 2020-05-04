

#include  <stm32f10x.h>
#include "hardi2c1.h"



#define SLAVER_ADDR 0xa0 //��7λ��ʾ��Ч�豸��ַ�����λ��ʾ��д
#define NO_ACK 0         //��ʾ���Ͳ�Ӧ���ź�
#define ACK 1            //��ʾ����Ӧ���ź�


void AT24c02_init(void);

uint8_t AT24c02_write_byte(uint8_t word_addr,uint8_t *dat,uint8_t len);

uint8_t AT24c02_read_byte(uint8_t word_addr,uint8_t *dat,uint8_t len);

