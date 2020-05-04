
#include  <stm32f10x.h>

#define NO_ACK 0 /*/��ʾ���Ͳ�Ӧ���ź�*/
#define ACK 1 /*��ʾ����Ӧ���ź� */



void i2c1_init(void);

int8_t i2c1_start(void);

void i2c1_stop(void);

int8_t i2c1_put_byte_data(uint8_t dat);

uint8_t i2c1_get_ack(void);


uint8_t i2c1_get_byte_data(void);

void i2c1_send_ack(uint8_t ack);

int8_t i2c1_put_addr(uint8_t dev_addr,uint8_t read);

int8_t i2c1_write_data(uint8_t dev_addr,uint8_t wd_addr,uint8_t *dat,uint16_t len);




