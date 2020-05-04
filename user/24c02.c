


#include "24c02.h"
#include <stdio.h>

/*
	��Ҫ�Ƕ�д24c02
	
	

*/

void AT24c02_init(void)
{
	i2c1_init();
}




/*
	�ӵ�ǰ��ַ��һ���ֽ�
	���ṩ����Ľӿ�
*/	
static uint8_t AT24c02_read_byte_cur(uint8_t *dat,uint8_t len)
{
	uint8_t i;
	
	//1.������ʼ�ź�
	if(i2c1_start())
	{
		printf("i2c start error!\n");
		return 254;
	}
	
	//2.�����豸��ַ
	if(i2c1_put_addr(SLAVER_ADDR , 1) != 0)   //���λ��1����ʾ������
	{
		printf("i2c  read_byte_cur send dev addr error!\n");
		return 253;
	}

	
	for(i=0;i<len;i++)
	{	
		//4.ֻ�����һ�����ݷ��ͷ�Ӧ��
		if(i == len -1)
		{
			i2c1_send_ack(NO_ACK);
		}
		else
		{
			i2c1_send_ack(ACK);
		}
				
		//3.���һ���ֽڵ�����
		dat[i] = i2c1_get_byte_data();		
	}
	
	//.����stopʱ��
	i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����

	return 0;
}



/*
	���������
	���� �� word_addr ָ����Ҫ��ȡ��λ��
			dat ��ʾ�洢���ݻ�����׵�ַ
			len ��ʾ��Ҫ��ȡ���ݵĸ���
	����ֵ�� 0 ��ʾ�ɹ�
*/
uint8_t AT24c02_read_byte(uint8_t word_addr,uint8_t *dat,uint8_t len)
{
	AT24c02_write_byte(word_addr,(void*)0,0);
		
	AT24c02_read_byte_cur(dat,len);
	
	return 0;
}

/*
	24c02 ��Ƭ��д�������
	������
		word_addr �ռ��ַ������Ҫ���ֽ�datд��24c02����һ���洢��Ԫ��ȥ
		dat       ʵ�����ݵ��׵�ַ��
		len       ʵ����Ҫд�����ݵĸ���
	����ֵ��
		0  ��     �ɹ�
		��0��     ʧ��
*/
uint8_t AT24c02_write_byte(uint8_t word_addr,uint8_t *dat,uint8_t len)
{
	uint8_t i;
	
	//1.������ʼ�ź�
	if(i2c1_start())
	{
		printf("i2c start error!\n");
		return 254;
	}
	
	//2.�����豸��ַ
	if(i2c1_put_addr(SLAVER_ADDR , 0) != 0)   //���λ��0����ʾд����
	{
		printf("i2c  AT24c02_write_byte send dev addr error!\n");
		return 253;
	}
	

	//4.���Ϳռ��ַ
	i2c1_put_byte_data(word_addr);  
	
	//3.��24c02��ack
	if(i2c1_get_ack())  //���û��Ӧ��ֱ���˳�
	{
		printf("send word addr error!\n");
		i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����
		return 2;
	}
	
	//len����0��ʱ������Ϊ���������һ����Ч����
	if(len == 0)
	{
		return 5;
	}
	
	for(i=0;i<len;i++)
	{		
		//4.��������
		i2c1_put_byte_data(dat[i]); 
		
		//3.��24c02��ack
		if(i2c1_get_ack())  //���û��Ӧ��ֱ���˳�
		{
			printf("send data error!\n");
			i2c1_stop();     //����ֹͣ�źţ����߾Ϳ�����
			return 3;
		}
	}
	
	//5.�������������ߵ�ռ��
	i2c1_stop();
	
	return 0;
}



