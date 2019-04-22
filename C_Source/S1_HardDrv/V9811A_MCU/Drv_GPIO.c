/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_GPIO.c
**Author		: maji
**date			: 2016-04-20 
**description	: GPIOģ���Ӳ������C����
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/*******************************************************************************************
** ��������: Init_GPIO_run
** ��������: ����GPIO���ܣ���������ģʽ�£�
** �������: ��
** �������: �� 
** ˵��    : 
*******************************************************************************************/
void Init_GPIO_run(void)                  
{
   // P0 port  [0:3]  use for debug////JTAG
    P0OE&=(~(BIT0+BIT1+BIT2+BIT3)); //JTAG ��� 0
    P0OD&=(~(BIT0+BIT1+BIT2+BIT3));
//	P9FC = 0;	 //all IS GPIO FUNCTION //
	P9OE =0xff&(~(BIT3+BIT0+BIT2)); // SET OUTPUT FUNCTION, =0 enable output//
	P9IE = (BIT3+BIT1); 							  // SET INPUT FUNCTION, =1 enable input//
	P9DO = 0;
	P9DO |= BIT2;//�ø߹رձ�����
	UART4_INITLIZE_ON();      //TTLͨѶʹ��
   	KEY_ERR_ADJ_INITLIZE();
//	LSD_RF_SET_DIO0_DIR_IN();
	PIN_CF_CLOSE();
	//�����
	P25FS = 0x00;
	P2OE &= (~BIT5); 
	P2OD |= BIT5;
	//�·�����
	P14FS = 0x00;
	P1IE |= BIT4;//����ʹ��
	// �ż�ⰴ��
	P13FS = 0x00;
	P1IE |= BIT3;//����ʹ��
	//��������
	P9FC &= (~BIT1);
	P9IE |= BIT1;

//	IP &= ~BIT5;
//	EIP |= BIT0;
}

/*******************************************************************************************
** ��������: Init_GPIO_sleep
** ��������: ����GPIO���ܣ��͹�������ģʽ�£�
** �������: ��
** �������: �� 
** ˵��    : 
*******************************************************************************************/
void Init_GPIO_sleep(void)                  
{
    P0OE=0XFF;
    P0IE=0;
    P1OE=0XFF;	
    P1IE=0 ;
    P2OE=0XFF;
    P2IE=0;	
    P3OE=0XFF;
    P3IE=0;
    P4OE=0XFF;	
    P4IE=0;
    P5OE=0XFF;
    P5IE=0;
    P6OE=0XFF;	   
    P6IE=0;
    P7OE=0XFF;
    P7IE=0;
    P8OE=0XFF;	   
    P8IE=0;
    P9OE=0XFF;	   
    P9IE=0;
}



/*******************************************************************************************
**    END
*******************************************************************************************/
