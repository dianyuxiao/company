#include "card_spi.h"
#include "card.h"
#include "string.h"
#include "delay.h"
unsigned char RevBuffer[30];//数据缓冲
unsigned char IC_ID[4];	    //卡号
unsigned char *NewKey;
unsigned char PassWd[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char Read_Data[30];
unsigned char *WriteData; 
char fan = 0;

void InitRC522(void)
{

	PcdReset();//芯片复位
	PcdAntennaOff();
    PcdAntennaOn();
	M500PcdConfigISOType('A');
	PcdAntennaOff();
}
int16_t RC522_GetCard(uint8_t *str)
{
    uint8_t temp;
    uint8_t num=0;
    uint8_t g_cRfidRxBuff_double[8];
    uint8_t g_cRfidRxBuff[20];
	PcdAntennaOn();
    *str=0;
    memset(g_cRfidRxBuff,0,sizeof(g_cRfidRxBuff));
    memset(g_cRfidRxBuff_double,0,sizeof(g_cRfidRxBuff_double));
    
    temp = PcdRequest(PICC_REQIDL, g_cRfidRxBuff);               //PICC_REQIDL  PICC_REQALL
    if(temp == MI_OK)
    {
		temp = PcdAnticoll(g_cRfidRxBuff);
        if(temp == MI_OK)
        {
			temp = PcdSelect(g_cRfidRxBuff);
			for(num=0;num<4;num++)
			{
                g_cRfidRxBuff_double[2*num]=((g_cRfidRxBuff[num]>>4)&0x0F) + '0';
                g_cRfidRxBuff_double[2*num+1]=(g_cRfidRxBuff[num]&0x0F) + '0';
			}
			memcpy(str,g_cRfidRxBuff_double,8);
			PcdAntennaOff();

        	return SUCCESS;
        }
		return ERROR;
    }
	PcdAntennaOff();
    return ERROR;
}


static void RC522_SoftSpiWrByte(uint8_t data)
{
    uint8_t i;
    
    for(i=0;i<8;i++)
    {
		CLR_SPI_CK;
        if(data & 0x80)
            SET_SPI_MOSI;
        else
            CLR_SPI_MOSI;
            
        data = data << 1;
        SET_SPI_CK;
        us_delay(1);
    }
    CLR_SPI_CK;
    CLR_SPI_MOSI;
}
static uint8_t RC522_SoftSpiRdByte(void)
{
    uint8_t i;
    uint8_t data=0x00;
    
    for(i=0;i<8;i++)
    {
        CLR_SPI_CK;
        us_delay(1);
        SET_SPI_CK;        
        data = data << 1;
        
        if(STU_SPI_MISO)
            data = data | 0x01;   
    }
    CLR_SPI_CK;
    
    return data;
}
/***************************************************************
* 函数名:RC522_WriteData（）
* MCU通过SPI写数据
* 输入参数: void
* 输出参数: 无
* 返回:     void
*****************************************************************/
void RC522_WriteData(uint8_t addr,uint8_t wrdata)
{
    CLR_SPI_CS;
    us_delay(1);
    addr = ((addr<<1) & 0x7E);
    RC522_SoftSpiWrByte(addr);
    RC522_SoftSpiWrByte(wrdata);
    SET_SPI_CS;
}
/***************************************************************
* 函数名:RC522_ReadData()
* MCU通过SPI读数据
* 输入参数: uint8_t addr
* 输出参数: 无
* 返回:     uint8_t
*****************************************************************/
uint8_t RC522_ReadData(uint8_t addr)
{
    uint8_t rddata;

    CLR_SPI_CS;
    us_delay(1);
    
    addr = ((addr<<1) & 0x7E) | 0x80;
    RC522_SoftSpiWrByte(addr);
    rddata = RC522_SoftSpiRdByte();
    
    SET_SPI_CS;
    
    return rddata;	
}

