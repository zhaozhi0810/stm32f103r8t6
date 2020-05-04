


#include <stm32f10x.h>
#include <mysystick.h>
#include <uart1.h>
#include <led.h>
#include "hardiic.h"
#include "oled.h"
#include "24c02.h"


/*
	本程序用于 stm32f103r8t6 
	
	硬件iic 已经调通  （24c02 0.96OLED）
	
	软件pack版本 ： Keil.STM32F1xx_DFP.2.2.0.pack
	keil         ： 5.29.0
	调试器		 ： jlink
	时间         ： 2020-05-04 
	版本         ： 1.0  初始版本，基本来自stm32f407的代码的移植
	视频参考：   ： https://www.bilibili.com/read/cv5861436
*/




int main(void)
{
	int i = 0;
	unsigned char buf[32];
	//1.中断分组配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

	//2.24c02端口设置
	AT24c02_init();
	
	//3.systick初始化，所有的延时都需要使用它	
	my_systick_int_init(72);   //72表示时钟频率
	
	//4.串口初始化，如果要使用printf 需要勾选 use Micro LIB
	uart1_int_init(115200);
	
	//5.OLED初始化，会进行初步的配置
	Oled_init();

	delay_ms(50);	
	
	//6.led初始化，根据自己的实际情况修改
	led_init();

	printf("%d-%d-%d\n",2020,5,3);
	
	for(i=0;i<20;i++)
	{
		buf[i] = i+30;
	}


//	if(AT24c02_write_byte(16,buf,15)==0)
//	{
//		printf("write 24c02 success!\n");
//	}
//	else
//	{
//		printf("write 24c02 failed!\n");
//	}
	
	delay_ms(100);
//	AT24c02_read_byte_cur(buf,20);
	AT24c02_read_byte(10,buf,20);
	for(i=0;i<20;i++)
	{
		printf("%d ",buf[i]);
	}
	printf("\n");
	
	
	//OLED显示
	OLED_ShowString(0,0,"2020-05-03 123456789abcdefg",16);

	OLED_ShowString(0,32,"2020-05-03",24);
	
	while(1)
	{
		led_set_status(LED1,LEDON);	
		delay_ms(700);	
		led_set_status(LED1,LEDOFF);
		delay_ms(700);
		printf("%d-%d-%d\n",2020,5,3);
		
		
		i++;
		sprintf((void*)buf,"%d",i);
		OLED_ShowString(0,32,buf,24);
	}
	
//	return 0;
}



