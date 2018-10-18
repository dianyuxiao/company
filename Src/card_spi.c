/******************************************************************************
*																			  *
*				IC��-�ײ�����-C												  *
*																			  *
******************************************************************************/

#include "card_spi.h"
#include "delay.h"
unsigned char value;

void IC_GPIO_Init(void)
 {
	  AFIO_GPxConfig(SPI_MASTER_SEL_AFIO_PORT, SPI_MASTER_SEL_AFIO_PIN, AFIO_FUN_GPIO);
	  AFIO_GPxConfig(SPI_MASTER_SCK_AFIO_PORT, SPI_MASTER_SCK_AFIO_PIN, AFIO_FUN_GPIO);
	  AFIO_GPxConfig(SPI_MASTER_MOSI_AFIO_PORT,SPI_MASTER_MOSI_AFIO_PIN,AFIO_FUN_GPIO);
	  AFIO_GPxConfig(SPI_MASTER_MISO_AFIO_PORT,SPI_MASTER_MISO_AFIO_PIN,AFIO_FUN_GPIO);
	  AFIO_GPxConfig(RES_IO_GPIO_ID,RES_IO_PIN,AFIO_FUN_GPIO);
	
	  GPIO_DirectionConfig(SPI_MASTER_SEL_GPIO_ID, SPI_MASTER_SEL_AFIO_PIN, GPIO_DIR_OUT);
	  GPIO_DirectionConfig(SPI_MASTER_SCK_GPIO_ID, SPI_MASTER_SCK_AFIO_PIN, GPIO_DIR_OUT);
	  GPIO_DirectionConfig(SPI_MASTER_MOSI_GPIO_ID, SPI_MASTER_MOSI_AFIO_PIN, GPIO_DIR_OUT);
	  GPIO_DirectionConfig(SPI_MASTER_MISO_GPIO_ID, SPI_MASTER_MISO_AFIO_PIN, GPIO_DIR_IN);
	  GPIO_DirectionConfig(RES_IO_GPIO_PORT,RES_IO_PIN, GPIO_DIR_OUT);
	  /* Configure GPIO pull resistor of input pins                                                             */
	  GPIO_PullResistorConfig(SPI_MASTER_SEL_GPIO_ID, SPI_MASTER_SEL_AFIO_PIN, GPIO_PR_UP);
	  GPIO_PullResistorConfig(SPI_MASTER_SCK_GPIO_ID, SPI_MASTER_SCK_AFIO_PIN, GPIO_PR_UP);
	  GPIO_PullResistorConfig(SPI_MASTER_MOSI_GPIO_ID, SPI_MASTER_MOSI_AFIO_PIN, GPIO_PR_UP);
      GPIO_PullResistorConfig(SPI_MASTER_MISO_GPIO_ID, SPI_MASTER_MISO_AFIO_PIN, GPIO_PR_UP);
	 
	 
      GPIO_InputConfig(SPI_MASTER_MISO_GPIO_ID,SPI_MASTER_MISO_AFIO_PIN,ENABLE);
	  
	  SET_SPI_CS;//CS�˿�Ϊ�ߵ�ƽ��cs��Ч 
		
 }
 


/********************************************************************
��    �ܣ�дRC522�Ĵ���
����˵����Address[IN]:�Ĵ�����ַ
          value[IN]:д���ֵ
********************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{
	unsigned char ucAddr,serialNum = 0;

	CLR_SPI_CS;
	CLR_SPI_CK;
	ucAddr = ((Address << 1) & 0x7E);
    for(serialNum=0;serialNum<8;serialNum++)    
    {  
		if ( ucAddr & 0x80)
		{
			SET_SPI_MOSI;
		}
		else
		{
			CLR_SPI_MOSI;
		} 
        //us_delay(100);  
		CLR_SPI_CK;
   
        //us_delay(100);  
		SET_SPI_CK;
        
		ucAddr <<= 1;
    }
	
	
	for(serialNum=0;serialNum<8;serialNum++)    
    {  
		if ( value & 0x80)
		{
			SET_SPI_MOSI;
		}
		else
		{
			CLR_SPI_MOSI;
		} 
        //us_delay(100);  
		CLR_SPI_CK;
   
        //us_delay(100);  
		SET_SPI_CK;
        
		value <<= 1;
    }
	SET_SPI_CS;	
	SET_SPI_CK;	
}
/*******************************************************************
��    �ܣ���RC522�Ĵ���
����˵����Address[IN]:�Ĵ�����ַ
��    �أ�������ֵ
**********************************************************************/
unsigned char ReadRawRC(unsigned char Address)
{
	unsigned char  ucAddr,serialNum = 0,dataValue=0;

	CLR_SPI_CS;
	CLR_SPI_CK;
    ucAddr = ((Address << 1) & 0x7E) | 0x80;
	
    for(serialNum=0;serialNum<8;serialNum++)    
    {  
		if ( ucAddr & 0x80)
		{
			SET_SPI_MOSI;
		}
		else
		{
			CLR_SPI_MOSI;
		} 
        //us_delay(100);  
		CLR_SPI_CK;
   
        //us_delay(100);  
		SET_SPI_CK;
        
		ucAddr <<= 1;
    }
	
     for(serialNum=0;serialNum<=7;serialNum++) 
    {  
		dataValue <<=1;
        CLR_SPI_CK;  
        if(STU_SPI_MISO) dataValue|=0x01;
		//us_delay(100); 
		SET_SPI_CK;
        //us_delay(100);  
    }  
	
	SET_SPI_CS;
	SET_SPI_CK;	
    return dataValue; 

}
/*****************************************************************
��    �ܣ���RC522�Ĵ���λ
����˵����reg[IN]:�Ĵ�����ַ
          mask[IN]:��λֵ
*******************************************************************/
void SetBitMask(unsigned char reg, unsigned char mask)
{
		unsigned char tmp = 0x0;
		tmp = ReadRawRC(reg);
		WriteRawRC(reg, tmp | mask);  // set bit mask
}

/********************************************************************
��    �ܣ���RC522�Ĵ���λ
����˵����reg[IN]:�Ĵ�����ַ
          mask[IN]:��λֵ
***********************************************************************/
void ClearBitMask(unsigned char reg, unsigned char mask)
{
		unsigned char tmp = 0x0;
		tmp = ReadRawRC(reg);
		WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}
/**********************************************************************
��    �ܣ���λRC522
��    ��: �ɹ�����MI_OK
***********************************************************************/
unsigned char PcdReset(void)
{
//    unsigned char i,data;
    //RC522��λ
	GPIO_WriteOutBits(RES_IO_GPIO_PORT,RES_IO_PIN,SET);
    ms_delay(1);  
    GPIO_WriteOutBits(RES_IO_GPIO_PORT,RES_IO_PIN,RESET);
    ms_delay(1);
    GPIO_WriteOutBits(RES_IO_GPIO_PORT,RES_IO_PIN,SET);
    ms_delay(1); 
	
    WriteRawRC(CommandReg, PCD_RESETPHASE); 
    ms_delay(1); 
    
    WriteRawRC(ModeReg, 0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL, 30);         //  	
    WriteRawRC(TReloadRegH, 0);
    WriteRawRC(TModeReg, 0x8D);
    WriteRawRC(TPrescalerReg, 0x3E);
    WriteRawRC(TxAutoReg, 0x40);
    return MI_OK;
}
/***********************************************************
��������  
ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
************************************************************/
void PcdAntennaOn(void)
{
	unsigned char i;
	i = ReadRawRC(TxControlReg);
	if (!(i & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}
//�ر�����
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}
/**************************************************************************
��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
����˵����Command[IN]:RC522������
          pInData[IN]:ͨ��RC522���͵���Ƭ������
          InLenByte[IN]:�������ݵ��ֽڳ���
          pOutData[OUT]:���յ��Ŀ�Ƭ��������
          *pOutLenBit[OUT]:�������ݵ�λ����
***************************************************************************/
unsigned char PcdComMF522(unsigned char Command, 
			unsigned char *pInData,
			unsigned char InLenByte, 
			unsigned char *pOutData,
			unsigned int *pOutLenBit)
{
	unsigned char status = MI_ERR;
	unsigned char irqEn = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
	}

	WriteRawRC(ComIEnReg, irqEn | 0x80);
	ClearBitMask(ComIrqReg, 0x80);
	WriteRawRC(CommandReg, PCD_IDLE);
	SetBitMask(FIFOLevelReg, 0x80);

	for (i = 0; i < InLenByte; i++)
	{
		WriteRawRC(FIFODataReg, pInData[i]);
	}
	WriteRawRC(CommandReg, Command);

	if (Command == PCD_TRANSCEIVE)
	{
		SetBitMask(BitFramingReg, 0x80);
	}

	i = 800; //600;//????????,??M1???????25ms
	do
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	} while ((i != 0) && !(n & 0x01) && !(n & waitFor));
	ClearBitMask(BitFramingReg, 0x80);

	if (i != 0)
	{
		if (!(ReadRawRC(ErrorReg) & 0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{
				status = MI_NOTAGERR;
			}
			if (Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{
					*pOutLenBit = (n - 1) * 8 + lastBits;
				}
				else
				{
					*pOutLenBit = n * 8;
				}
				if (n == 0)
				{
					n = 1;
				}
				if (n > MAXRLEN)
				{
					n = MAXRLEN;
				}
				for (i = 0; i < n; i++)
				{
					pOutData[i] = ReadRawRC(FIFODataReg);
				}
			}
		}
		else
		{
			status = MI_ERR;
		}
	}
	SetBitMask(ControlReg, 0x80);           // stop timer now
	WriteRawRC(CommandReg, PCD_IDLE);
	return status;
}
/***************************************************************************
��    �ܣ�Ѱ��
����˵��: req_code[IN]:Ѱ����ʽ
                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26 = Ѱδ��������״̬�Ŀ�
          pTagType[OUT]����Ƭ���ʹ���
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
��    ��: �ɹ�����MI_OK
********************************************************************************/
unsigned char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
	unsigned char status;
	unsigned int unLen=0;
	unsigned char ucComMF522Buf[MAXRLEN];

	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x07);
        //status=ReadRawRC(BitFramingReg);
        //status++;
	SetBitMask(TxControlReg, 0x03);

	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf,
			&unLen);
//   UART_send_byte(status);
	if ((status == MI_OK) && (unLen == 0x10))
	{
		*pTagType = ucComMF522Buf[0];
		*(pTagType + 1) = ucComMF522Buf[1];
	}
	else
	{
		status = MI_ERR;
	}

	return status;
}
/**********************************************************************
��    �ܣ�����ײ
����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
***********************************************************************/
unsigned char PcdAnticoll(unsigned char *pSnr)
{
	unsigned char status;
	unsigned char i, snr_check = 0;
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN];

	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x00);
	ClearBitMask(CollReg, 0x80);

	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

	if (status == MI_OK)
	{
		for (i = 0; i < 4; i++)
		{
			*(pSnr + i) = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
		{
			status = MI_ERR;
		}
	}

	SetBitMask(CollReg, 0x80);
	return status;
}
/*********************************************************************
��    �ܣ�ѡ����Ƭ
����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
************************************************************************/
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
            ucComMF522Buf[i+2] = *(pSnr+i);
            ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);

    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);

    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/*******************************************************************
��    �ܣ���֤��Ƭ����
����˵��: auth_mode[IN]: ������֤ģʽ
                 0x60 = ��֤A��Կ
                 0x61 = ��֤B��Կ 
          addr[IN]�����ַ
          pKey[IN]������
          pSnr[IN]����Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
**********************************************************************/               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
//   memcpy(&ucComMF522Buf[2], pKey, 6); 
//   memcpy(&ucComMF522Buf[8], pSnr, 4); 

    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }

    return status;
}

/**************************************************************
��    �ܣ���ȡM1��һ������
����˵��: addr[IN]�����ַ
          pData[OUT]�����������ݣ�16�ֽ�
��    ��: �ɹ�����MI_OK
***************************************************************/ 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
//   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }

    return status;
}
/*********************************************************************
��    �ܣ�д���ݵ�M1��һ��
����˵��: addr[IN]�����ַ
          pData[IN]��д������ݣ�16�ֽ�
��    ��: �ɹ�����MI_OK
***********************************************************************/                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    
			ucComMF522Buf[i] = *(pData+i);   
		}
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }

    return status;
}

/*****************************************************************
��    �ܣ����Ƭ��������״̬
��    ��: �ɹ�����MI_OK
**********************************************************************/
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}

/********************************************************************
��MF522����CRC16����
*********************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len;i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

//����RC522�Ĺ�����ʽ 
char M500PcdConfigISOType(unsigned char type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(ModeReg,0x3D);//3F
		WriteRawRC(RxSelReg,0x86);//84
		WriteRawRC(RFCfgReg,0x7F);   //4F
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);

		us_delay(100);
		PcdAntennaOn();
   }
   else{ return MI_ERR; }

   return MI_OK;
}








































	
	 
	 

