

#include <stm32f10x.h>




#define OLED_CMD  0		//Ð´ÃüÁî
#define OLED_DATA 0x40	//Ð´Êý¾Ý
#define u8 unsigned char
#define u32 unsigned int


#define SLAVER_ADDR_OLED 0x78


void Oled_init(void);  //

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 size1);

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_Refresh(void);
void OLED_Clear(void);




