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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2013/7/27
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	
//********************************************************************************
//V1.1修改说明
//修正了STMFLASH_Write函数地址偏移的一个bug.
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

//解锁STM32的FLASH
void STMFLASH_Unlock(void)
{
  FLASH->KEYR=MY_FLASH_KEY1;//写入解锁序列.
  FLASH->KEYR=MY_FLASH_KEY2;
}
//flash上锁
void STMFLASH_Lock(void)
{
  FLASH->CR|=1<<7;//上锁
}
//得到FLASH状态
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //忙
	else if(res&(1<<2))return 2;	//编程错误
	else if(res&(1<<4))return 3;	//写保护错误
	return 0;						//操作完成
}
//等待操作完成
//time:要延时的长短
//返回值:状态.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		delay_us(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除页
//paddr:页地址
//返回值:执行情况
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//等待上次操作结束,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//页擦除
		FLASH->AR=paddr;//设置页地址 
		FLASH->CR|=1<<6;//开始擦除		  
		res=STMFLASH_WaitDone(0X5FFF);//等待操作结束,>20ms  
		if(res!=1)//非忙
		{
			FLASH->CR&=~(1<<1);//清除页擦除标志.
		}
	}
	return res;
}
//在FLASH指定地址写入半字
//faddr:指定地址(此地址必须为2的倍数!!)
//dat:要写入的数据
//返回值:写入的情况
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//编程使能
		*(vu16*)faddr=dat;//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成
		if(res!=1)//操作成功
		{
			FLASH->CR&=~(1<<0);//清除PG位.
		}
	} 
	return res;
} 
//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))){
		SendChar("error flash\r\n");
		return;//非法地址
	}
	STMFLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	STMFLASH_Lock();//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}
















