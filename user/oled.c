

#include "oled.h"
#include "hardi2c1.h"
#include "oledfont.h"
#include <stdio.h>

/*
	128*64
	
	列是128
	行是64行，每个字节控制一列的8行，总共需要8个字节
	
	目前只能支持16号字体（字母是8*16，汉字是16*16，列行式）

	因为高度是16，所以这个屏幕最大只能显示4行
	一行最多显示英文字符或数字16个
				汉字只能显示8个

	2020-04-05
*/


static uint8_t OLED_GRAM[8][128];   //8*8*128



int8_t OLED_WR_data(u8 *dat,uint16_t len,u8 mode)
{
	return i2c1_write_data(SLAVER_ADDR_OLED,mode,dat,len);
//	return 0;
}


//更新显存到OLED	
void OLED_Refresh(void)
{
	OLED_WR_data(OLED_GRAM[0],sizeof OLED_GRAM,OLED_DATA);
//	OLED_WR_data(OLED_GRAM[0],32,OLED_DATA);	
}


//清屏函数
void OLED_Clear(void)
{
	u8 i=0,n;
	for(i=0;i<8;i++)
	{
		for(n=0;n<128;n++)
		{
			OLED_GRAM[i][n] = 1<<i;//清除所有数据
		}
	}
	OLED_Refresh();//更新显示
}



////画点 
////x:0~127
////y:0~63
//void OLED_DrawPoint(u8 x,u8 y)
//{
//	u8 i,m,n;
//	i = y>>3;
//	m = y&7;
//	n = 1<<m;
//	
//	OLED_GRAM[x][i]|=n;
//}

////清除一个点
////x:0~127
////y:0~63
//void OLED_ClearPoint(u8 x,u8 y)
//{	
//	OLED_GRAM[x][y>>3] &=~(1<<(y&7));	
//}


//在指定位置显示一个字符,包括部分字符
//y:0~127
//x:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,chr1;
	u8 *temp;
	y /=  8;
	chr1 = chr - ' ';  //计算偏移后的值
	
	
	temp = (void*)asc2_1608;
	size1 = 16;
//	if(size1==12) //调用1206字体
//	{
//		temp = (void*)asc2_1206;
//	} 
//	else if(size1==16)//调用1608字体
//	{
//		temp = (void*)asc2_1608;
//	} 
//	else if(size1==24)//调用2412字体
//	{
//		temp = (void*)asc2_2412;
//	} 
//	else 
//		return;
		
	for(i=0;i<size1;i++)
	{		
		OLED_GRAM[y+(i>>3)][x+(i&7)] = temp[chr1*size1+i];	
	}
}





//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
	
		x += size1/2;
		if(x > 128-size1/2)  //换行
		{
			x=0;
			y+=16;
		}
		chr++;
	}
	OLED_Refresh();//更新显示
}





//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
static void OLED_ShowChinese(u8 x,u8 y,u8* str,u8 size1)
{
	uint8_t* temp ;
	uint8_t i,j;
	
	y /=  8;
	
//	if(num*2 > sizeof Hzk1 / (sizeof Hzk1[0]))
//	{
//		return ;
//	}
	
	temp = (void*)str;
		
	for(j=0;j<2;j++)
		for(i=0;i<size1;i++)
		{		
			OLED_GRAM[y+(i>>4)+j][x+(i&0xf)] = temp[i+j*size1];	
		}
}

/*
	x  x轴的起始位置
	y  y轴的起始位置（0，0在左上角）
	chr 字符数组的首地址
	len 表示显示汉字的个数
	size1 表示字体大小
*/

void OLED_Chinese(u8 x,u8 y,void* chr,uint8_t len,u8 size1)
{
	uint8_t i;
	
	for(i=0;i<len;i++)
	{
		OLED_ShowChinese(x+size1*i,y,(uint8_t*)chr+2*size1*i,size1);
	}
}




////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 size1)
{
	u8 t=0;
	u8 arr[16] = {0};
	
	t = 15;
	while(num)
	{		
		arr[--t] = num%10+'0';
		num /= 10;		
	}
	
	OLED_ShowString(x,y,arr+t,size1);
}


void Oled_init(void)
{
	const uint8_t cmd[] = {0xAE,0x00,0x10,0x40,0x81,0xCF,0xA1,0xC8,
							0xA6,0xA8,0x3f,0xD3,0x00,0xd5,0x80,0xD9,
							0xF1,0xDA,0x12,0xDB,0x40,0x20,0x00,0x8D,
							0x14,0xA4,0xA6,0xAF};
	
	
	i2c1_init(); //i2c接口初始化
	
							
	OLED_WR_data((void*)cmd,sizeof cmd,OLED_CMD);						
							
//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);
	
	OLED_Clear();
	
}




