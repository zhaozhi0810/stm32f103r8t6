

#include "hardiic.h"
#include <mysystick.h>
#include <stdio.h>


/*
	PB6  SCL 
	PB7  SDA 
*/
static int i2c1_ready = 0; //初始化了吗？？


//i2c1的硬件初始化
void i2c1_init(void)
{
		
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	if(i2c1_ready == 0)
	{	
		//1.时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		
		//2.gpio的功能配置，复用功能
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;		
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;  //	
		GPIO_Init(GPIOB, &GPIO_InitStruct);  //这个函数，让结构体的内容配置到寄存器中
		
		
		//3.iic控制器的初始化
		I2C_StructInit(&I2C_InitStruct);     //应答是非应答
		I2C_InitStruct.I2C_ClockSpeed = 400000;   //最大不能超过400k
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;        /*应答使能 */
		I2C_Init(I2C1, &I2C_InitStruct);
		
		I2C_Cmd(I2C1, ENABLE);

		i2c1_ready = 1; //表示已经初始化了，下次调用这个函数就不再初始化
	}
}





int8_t i2c1_start(void)
{
	uint16_t times = 0;
	
	
	//产生起始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	
	//检查EV5
	times = 0;
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		times++;
		if(times > 500)
		{
		
			I2C_GenerateSTOP(I2C1, ENABLE);
		
		}
	}
	return 0;
}

void i2c1_stop(void)
{
	I2C_GenerateSTOP(I2C1, ENABLE);	
}


int8_t i2c1_put_addr(uint8_t dev_addr,uint8_t read)
{
	uint32_t envent;
	uint16_t times = 0;
	
	if(read)
	{
		I2C_Send7bitAddress(I2C1,dev_addr, I2C_Direction_Receiver);
		envent = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;
	}
	else
	{
		I2C_Send7bitAddress(I2C1,dev_addr, I2C_Direction_Transmitter);
		envent = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
	}

	//检查EV6
	times = 0;
	while(I2C_CheckEvent(I2C1, envent) != SUCCESS)
	{
		times++;
		if(times > 2000)
		{
			i2c1_stop();
			return -1;
		}
	}	
	return	0;
}



int8_t i2c1_put_byte_data(uint8_t dat)
{			
	I2C_SendData(I2C1, dat);
	
	return	0;
	
}

uint8_t i2c1_get_ack(void)
{
	uint16_t times = 0;
	//检查EV8_2,发送完成吗？
	times = 0;
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		times++;
		if(times > 2000)
		{
			i2c1_stop();
			return 1;
		}
	}
	return 0;
}


uint8_t i2c1_get_byte_data(void)
{
	uint8_t dat;
	uint16_t times = 0;

	//ev7
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
	{
		times++;
		if(times > 1000)
		{
			i2c1_stop();
			return 254;
		}
	}
			
	dat = I2C_ReceiveData(I2C1);
	
	return dat;
}



void i2c1_send_ack(uint8_t ack)
{
	if(ack == NO_ACK)
	{
		I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
	}
	else
	{
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}
}






int8_t i2c1_write_data(uint8_t dev_addr,uint8_t wd_addr,uint8_t *dat,uint16_t len)
{
	uint16_t i;
	
	//1.发送起始信号
	if(i2c1_start())
	{
		printf("i2c start error!\n");
		return -1;
	}
	
	//2.发送设备地址		
	if(i2c1_put_addr(dev_addr, 0) != 0)   //最低位是0，表示写操作
	{
		printf("i2c  OLED_WR_Byte send dev addr error!\n");
		return -2;
	
	}
	
	
	i2c1_put_byte_data(wd_addr);
	if(i2c1_get_ack())  //如果没有应答，直接退出
	{
		printf("send wd_addr error!\n");
		i2c1_stop();     //发送停止信号，总线就空闲了
		return 2;
	}
	
	
	if(len == 0) //24c02需要特殊处理一下
	{			//len==1表示只有发送空间地址
		return 5;
	}
	
		
	//3.连续数据
	for(i=0;i<len;i++)
	{			
		i2c1_put_byte_data(dat[i]);
		if(i2c1_get_ack())  //如果没有应答，直接退出
		{
			printf("send 0x78 error!\n");
			i2c1_stop();     //发送停止信号，总线就空闲了
			return 2;
		}
	}
	

	//5.结束，结束总线的占用
	i2c1_stop();
	
	delay_ms(100);
	
	return 0;
}

