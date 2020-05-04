

#include "uart1.h"


/*
	����1��ʹ��
	 115200��8N1
	
	��Ӧ������
	GPA9��GPA10  ��TX,RX��

*/
uint8_t recv_buf[8];   //�����洢���ڵ�����
uint8_t recv_flag = 0;  //���ձ�־��Ϊ1ʱ��ʾ���յ������ݣ�Ϊ0��ʱ���ʾû�н��յ�����



void uart1_init(uint32_t bandrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//1.ʱ��ʹ�ܣ�����ʹ�ܣ��ⲿ�豸����ʹ�ã�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//���õĹ��ܺܶ࣬���þ���ĸ��ù���
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	
	//2.����IO�˿ڵ�ģʽ�����ù��ܣ�
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
		
	//3.��ʼ�����ڿ�����
	USART_StructInit(& USART_InitStruct);  //��ʼ���ṹ��
	USART_InitStruct.USART_BaudRate = bandrate;
	USART_Init(USART1, &USART_InitStruct);



	//4.�������ڿ�����
	USART_Cmd(USART1, ENABLE);
}



void uart1_int_init(uint32_t bandrate)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	uart1_init(bandrate);   //��ʼ������
		
	//1.���ڵ��ж��кü���������ֻʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//2.����NVIC��������1���жϣ������������ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
}




//�������ֻ������ļ�ʹ�ã������ļ����ܵ����������
static void uart1_putchar(uint8_t dat)
{
	//��ѯһ���ǲ��Ƿ��ͼĴ���Ϊ��
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));  //USART_GetFlagStatus����ֵ��0����ʾ���ͻ��治Ϊ��
	
	USART_SendData(USART1, dat);
}


//���ڷ����ַ���
void uart1_putstr(uint8_t dat[])
{	
	while(*dat)
	{
		uart1_putchar(*dat++);
	}
}





//��Ҫʹ��printf,��Ҫʵ��fputc�������
int fputc(int ch, FILE *f)
{
	uart1_putchar(ch);
	return 1;
}




//���մ��ڵ�����
uint8_t	uart1_getchar(void)
{
	//��ѯ�ǲ��������� ��������
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE)); //����ֵ��0����ʾû�н��յ����ݣ�����
	
	return USART_ReceiveData(USART1);  //�ѽ��յ����ݶ�����
	
	
}



//����1���жϴ�����
//��Ϊֻ�����˽����жϣ�ֻ�н��յ����ݣ��Ż������ж�
void USART1_IRQHandler(void)
{
	static uint8_t n = 0;
	
	//��ȡ�жϽ��յ����ݵ�ȫ�ֱ�����	
	recv_buf[n++] = USART_ReceiveData(USART1);
		
	if(n>7)  //��ֹ�������
	{
		n = 0;
	}
	
	if(n==1)  //�Ѿ����յ���һ������
	{
		if(recv_buf[0] != 0x55)
		{
			n = 0;
		}
	}
	else if(n == 2) //�Ѿ����յ��ڶ�������
	{
		if(recv_buf[1] != 0xaa)
		{
			n = 0;
		}
	}
	else if(n > 3) //�Ѿ��յ�֡ͷ���������ݵĳ���
	{
		if(n == recv_buf[2]+3 )
		{
			n = 0;
			recv_flag = 1;	   //��ʾ�ҽ��յ���һ֡����������
		}
	}
	
	//���жϱ�־��
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}



