
#include "led.h"
//#include <stm32f4xx.h>





/*
	led 初始化
	PC11
	PD2
	PA8
*/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
	
	//2.设置为输出模式
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
			GPIO_SetBits(GPIOA, GPIO_Pin_8);   //设置这个引脚为高电平
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_8); //设置这个引脚为低电平
		}
	}
	else if(ledn == LED2)
	{
		if(status == LEDOFF)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_11);   //设置这个引脚为高电平
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_11); //设置这个引脚为低电平
		}
	}
	else if(ledn == LED3)
	{
		if(status == LEDOFF)
		{			
			GPIO_SetBits(GPIOD, GPIO_Pin_2);   //设置这个引脚为高电平
		}
		else //if(status == LEDON)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_2); //设置这个引脚为低电平
		}
	}

	return;
	
}


/*
	能够显示的就是0-7的二进制表示
	熄灭表示0，点亮表示1，led1是最低位
*/
void led_show_num(uint8_t num)
{
	num = ~num; //0,表示要熄灭所有的灯	
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_8, (BitAction)(num&1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction)((num>>1) & 1));
	GPIO_WriteBit(GPIOE, GPIO_Pin_10, (BitAction)((num>>2) & 1));
}


