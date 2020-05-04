
#include "led.h"
//#include <stm32f4xx.h>





/*
	led ��ʼ��
	PC11
	PD2
	PA8
*/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
	
	//2.����Ϊ���ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
}



void led_set_status(ledn_t ledn,led_sta_t status)
{	
	if(ledn == LED1)
	{
		if(status == LEDOFF)
		{			
			GPIO_SetBits(GPIOA, GPIO_Pin_8);   //�����������Ϊ�ߵ�ƽ
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_8); //�����������Ϊ�͵�ƽ
		}
	}
	else if(ledn == LED2)
	{
		if(status == LEDOFF)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_11);   //�����������Ϊ�ߵ�ƽ
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_11); //�����������Ϊ�͵�ƽ
		}
	}
	else if(ledn == LED3)
	{
		if(status == LEDOFF)
		{			
			GPIO_SetBits(GPIOD, GPIO_Pin_2);   //�����������Ϊ�ߵ�ƽ
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_2); //�����������Ϊ�͵�ƽ
		}
	}

	return;
	
}


/*
	�ܹ���ʾ�ľ���0-7�Ķ����Ʊ�ʾ
	Ϩ���ʾ0��������ʾ1��led1�����λ
*/
void led_show_num(uint8_t num)
{
	num = ~num; //0,��ʾҪϨ�����еĵ�	
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_8, (BitAction)(num&1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)((num>>1) & 1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_10, (BitAction)((num>>2) & 1));
}


