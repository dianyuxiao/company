 #ifndef  _CARD_H__
 #define  __CARD_H__
#include "ht32.h"

static const u8 Card_ID[4]={
0x3A,0x3D,0x3C,0x38
};

void InitRC522(void);  
int16_t RC522_GetCard(uint8_t *str);
void RC522_WriteData(uint8_t addr,uint8_t wrdata);
uint8_t RC522_ReadData(uint8_t addr);



#endif	 
/********************************************************************/
//		　　　　　	END FILE
/********************************************************************/
