#include "stmflash.h"
#include "delay.h"
#include "usart.h"
#include "HAL.h"
#include "nrf24l01.h"

#define EE_RC_THR_MAX	0
#define EE_RC_THR_MIN	1
#define EE_RC_THR_TRIM	2
#define EE_RC_YAW_MAX	3
#define EE_RC_YAW_MIN	4
#define EE_RC_YAW_TRIM	5
#define EE_RC_PIT_MAX	6
#define EE_RC_PIT_MIN	7
#define EE_RC_PIT_TRIM	8
#define EE_RC_ROL_MAX	9
#define EE_RC_ROL_MIN	10
#define EE_RC_ROL_TRIM	11
#define EE_RC_TX_ADDR   12


u32 VirtAddVarTab[] = {
		0xEE00, 0xEE02, 0xEE04,//THR_(MAX,MIN,TRIM)
		0xEE06, 0xEE08, 0xEE0A,//YAW_(MAX,MIN,TRIM)
		0xEE1C, 0xEE1E, 0xEE20,//PIT_(MAX,MIN,TRIM)
		0xEE22, 0xEE24, 0xEE26,//ROL_(MAX,MIN,TRIM)
		0xEE28, 0xEE2A, 0xEE2C,//(rc_tx_addr)
		0xEE2E, 0xEE30, 0xEE32,//
		0xEE34, 0xEE36, 0xEE38,//
		0xEE3A, 0xEE3C, 0xEE3E,//
		0xEE40, 0xEE42, 0xEE44,//
};

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2013/7/27
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	
//********************************************************************************
//V1.1�޸�˵��
//������STMFLASH_Write������ַƫ�Ƶ�һ��bug.
//////////////////////////////////////////////////////////////////////////////////

																		 
/*******************************************************************************
*FLASH as EEPROM
*Fuction
*
*
********************************************************************************/

void Para_ResetToFactorySetup()
{

}

void EE_SAVE_RC_TX_ADDR(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_RC_TX_ADDR],TX_ADDRESS[4]);
}

void EE_READ_RC_TX_ADDR(void)
{
	int16_t temp;

	EE_ReadVariable(VirtAddVarTab[EE_RC_TX_ADDR],&temp);

	TX_ADDRESS[4] = (uint8_t)temp;
}


void EE_SAVE_RC_SET(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_RC_THR_MAX],rc.adc_max[rc_thr_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_THR_MIN],rc.adc_min[rc_thr_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_THR_TRIM],rc.trim[rc_thr_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_YAW_MAX],rc.adc_max[rc_yaw_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_YAW_MIN],rc.adc_min[rc_yaw_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_YAW_TRIM],rc.trim[rc_yaw_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_ROL_MAX],rc.adc_max[rc_rol_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_ROL_MIN],rc.adc_min[rc_rol_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_ROL_TRIM],rc.trim[rc_rol_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_PIT_MAX],rc.adc_max[rc_pit_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_PIT_MIN],rc.adc_min[rc_pit_num]);
	EE_WriteVariable(VirtAddVarTab[EE_RC_PIT_TRIM],rc.trim[rc_pit_num]);



}
void EE_READ_RC_SET(void)
{
	EE_ReadVariable(VirtAddVarTab[EE_RC_THR_MAX],  &rc.adc_max[rc_thr_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_THR_MIN],  &rc.adc_min[rc_thr_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_THR_TRIM], &rc.trim[rc_thr_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_YAW_MAX],  &rc.adc_max[rc_yaw_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_YAW_MIN],  &rc.adc_min[rc_yaw_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_YAW_TRIM], &rc.trim[rc_yaw_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_ROL_MAX],  &rc.adc_max[rc_rol_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_ROL_MIN],  &rc.adc_min[rc_rol_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_ROL_TRIM], &rc.trim[rc_rol_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_PIT_MAX],  &rc.adc_max[rc_pit_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_PIT_MIN],  &rc.adc_min[rc_pit_num]);
	EE_ReadVariable(VirtAddVarTab[EE_RC_PIT_TRIM], &rc.trim[rc_pit_num]);
}



u8 EE_WriteVariable(u32 addr,u16 data)
{
	STMFLASH_Write(STM32_FLASH_BASE+(u32)addr,&data,2);
	return 0;
}

u8 EE_ReadVariable(u32 addr,u16* data)
{
	STMFLASH_Read(STM32_FLASH_BASE+(u32)addr,data,2);
	return 0;
}

//����STM32��FLASH
void STMFLASH_Unlock(void)
{
  FLASH->KEYR=MY_FLASH_KEY1;//д���������.
  FLASH->KEYR=MY_FLASH_KEY2;
}
//flash����
void STMFLASH_Lock(void)
{
  FLASH->CR|=1<<7;//����
}
//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //æ
	else if(res&(1<<2))return 2;	//��̴���
	else if(res&(1<<4))return 3;	//д��������
	return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
		delay_us(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//ҳ����
		FLASH->AR=paddr;//����ҳ��ַ 
		FLASH->CR|=1<<6;//��ʼ����		  
		res=STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res!=1)//��æ
		{
			FLASH->CR&=~(1<<1);//���ҳ������־.
		}
	}
	return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//���ʹ��
		*(vu16*)faddr=dat;//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res!=1)//�����ɹ�
		{
			FLASH->CR&=~(1<<0);//���PGλ.
		}
	} 
	return res;
} 
//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))){
		SendChar("error flash\r\n");
		return;//�Ƿ���ַ
	}
	STMFLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	STMFLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}
















