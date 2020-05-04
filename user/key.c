
#include "key.h"




/*
	KEY1��2 ��ӦPA�˿ڵ�13,15����


	����ʱ���͵�ƽ
	�ɿ�ʱ���ߵ�ƽ
*/

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	
	//2.��ʼ��Ϊ����ģʽ��key1��2��3��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}




/*
	�����жϷ�ʽ�ĳ�ʼ��
	
*/
//void key_int_init(void)
//{
//	EXTI_InitTypeDef  EXTI_InitStruct;
//	NVIC_InitTypeDef  NVIC_InitStruct;
//	
//	//�ж�ģʽ��ʱ�򣬰�����io�˿���Ȼ����Ϊ����ģʽ
//	key_init();
//	
//	//syscfg��ʱ��ʹ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //ʱ��ʹ��
//	
//	
//	//1.�����ж����� exti4 ѡ�� PE4�������
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);    //Pe4����Ϊ�ⲿ�ж�4
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource5);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
//	
//	
//	
//	//2.���ⲿ�ж�4�������ã�ʹ�ܣ����ô�����ʽ
//	EXTI_InitStruct.EXTI_Line = EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line13;   //ָ����һ���ⲿ�ж�
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //ʹ�ܻ��ǽ�ֹ
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     //�жϻ����¼�
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  //������ʽ���½��� rise fall
//	EXTI_Init(&EXTI_InitStruct);
//	
//	//3.NVIC�ĳ�ʼ�������ȼ����ж�����
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;   //�ⲿ�ж�4
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;    //��ռ���ȼ� 0-3
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;           //��Ӧ���ȼ� 0-3
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_Init(&NVIC_InitStruct);
//}





/*
	���ܣ� ��ȡ������״̬
	ע�⣺��֧�ֶ������ͬʱ���µ������
	���� �� ��
	����ֵ�� 0 ��ʾû�а�������
			 1 ��ʾ����1������
			 2 ��ʾ����2������
			 3 ��ʾ����3������
			 4 ��ʾ����4������
*/
uint8_t get_key_val(void)
{
	//��������֮���ǵ͵�ƽ���ɿ�֮���Ǹߵ�ƽ
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == Bit_RESET)
	{
		return 1;
	}	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == Bit_RESET)
	{
		return 2;
	}
//	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == Bit_RESET)
//	{
//		return 3;
//	}
//	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_RESET)
//	{
//		return 4;
//	}
	

	
	return 0;
	
}



/*
	ʹ�ö�ʱ��ɨ�谴�� ÿ20msɨ��һ��
	����ɨ��ͬһ������ֵ��ʾ�а�������
	������Ƕ���

*/

uint8_t key_flag = 0;   //��0��ʾ��⵽�������£�0��ʾû�м�⵽����

void key_scan(void)
{
	uint8_t key_val = 0;
	static uint8_t key_val_old = 0;  //������һ��ɨ��İ�����ֵ
	static uint8_t key_stat = 0;     //0��ʾû�м�⵽������1��ʾ�Ѿ���⵽����������û���ɿ�
	
	
	key_val = get_key_val();   //��ð�����ֵ
	
	if(key_stat && key_val == key_val_old)
	{
		return;
	}
	else if(key_val != key_val_old)  //�������ֵ��֮ǰ�İ���ֵ��ͬ
	{
		key_stat = 0;
	}
	
	//�м�ⰴ����������һ�μ�⵽�ĺ���һ�μ�����һ������
	if(key_val != 0 && key_val == key_val_old)
	{
		key_flag = key_val;   //kay_flag��ֵ��ʾ��һ������������
		key_stat = 1;
	}
	else //���ܵ������1.û�м�⵽���� 2.���μ���ֵ��ͬ
	{
		key_val_old = key_val;
	}
	
}















//�жϴ�����
/*�������ֻ�����жϵĴ�����ʱ�򱻵��ã�����Ҫmain����������������
	����Ӳ���Լ���ɵ���ת��������ʽȥ����
	1.û�в�����û�з���ֵ
	2.������������
	3.Ӧ��Խ���˳��жϴ���Խ�ã���Ȼ��Ҫ����һЩ���ܴ���
*/

//void EXTI4_IRQHandler(void)
//{	
//	//��תһ��led1��״̬
//	GPIO_ToggleBits(GPIOE, GPIO_Pin_8);
//	
//	//����жϱ�־
//	EXTI_ClearITPendingBit(EXTI_Line4);
//}

///*
//	����������Դ���5���ⲿ�жϣ���������һ���ж�
//*/
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line5))  //Ϊ1��ʱ�򣬱�ʾ�������ⲿ�ж�5
//	{
//		//��תһ��led1��״̬
//		GPIO_ToggleBits(GPIOE, GPIO_Pin_9);
//		//����жϱ�־
//		EXTI_ClearITPendingBit(EXTI_Line5);
//	}
//	
//	else if(EXTI_GetITStatus(EXTI_Line6))
//	{
//		//��תһ��led1��״̬
//		GPIO_ToggleBits(GPIOE, GPIO_Pin_10);
//		//����жϱ�־
//		EXTI_ClearITPendingBit(EXTI_Line6);
//	}
//		
//}



//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line13))
//	{
//		//��תһ��led1��״̬
//		GPIO_ToggleBits(GPIOE, GPIO_Pin_8);
//		
//		//����жϱ�־
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}
//}

