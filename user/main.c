


#include <stm32f10x.h>
#include <mysystick.h>
#include <uart1.h>
#include <led.h>
#include "hardiic.h"
#include "oled.h"
#include "24c02.h"


/*
	���������� stm32f103r8t6 
	
	Ӳ��iic �Ѿ���ͨ  ��24c02 0.96OLED��
	
	���pack�汾 �� Keil.STM32F1xx_DFP.2.2.0.pack
	keil         �� 5.29.0
	������		 �� jlink
	ʱ��         �� 2020-05-04 
	�汾         �� 1.0  ��ʼ�汾����������stm32f407�Ĵ������ֲ
	��Ƶ�ο���   �� https://www.bilibili.com/read/cv5861436
*/




int main(void)
{
	int i = 0;
	unsigned char buf[32];
	//1.�жϷ�������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

	//2.24c02�˿�����
	AT24c02_init();
	
	//3.systick��ʼ�������е���ʱ����Ҫʹ����	
	my_systick_int_init(72);   //72��ʾʱ��Ƶ��
	
	//4.���ڳ�ʼ�������Ҫʹ��printf ��Ҫ��ѡ use Micro LIB
	uart1_int_init(115200);
	
	//5.OLED��ʼ��������г���������
	Oled_init();

	delay_ms(50);	
	
	//6.led��ʼ���������Լ���ʵ������޸�
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
	
	
	//OLED��ʾ
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



