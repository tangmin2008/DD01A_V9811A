/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_System_Interrupt.c
**Author		: maji
**date			: 2016-04-20
**description	: ϵͳ�ж������������C����
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>




//=======================================================
//��������Interrupt_Int0
//�������ã��˿��ж�0���ж�����				��03H��
//======================================================= 
void Interrupt_Int0 (void) interrupt 0 using 2
{
    TCON &= ~BIT1;
//  user coding begin
//    Int0();		
}
//=======================================================
//��������Interrupt_Timer0
//�������ã��ں�timer0�жϣ��ж�����		��0BH��
//Ӧ��˵��: 10ms��ʱ
//======================================================= 
void Interrupt_Timer0 (void) interrupt 1 using 2
{

    TL0  = LOBYTE(T0_10MS_CNT);         //10ms
    TH0  = HIBYTE(T0_10MS_CNT);

    // user coding begin
    gs_sys_run.back_fg |= BIT0_FONT_FG_10MS;   //����10ms��ʶ��Ч //

    if(gs_uart_iec_app_var[UART_CH0_IR].iec_status!=0) gs_uart_iec_app_var[UART_CH0_IR].delay_10ms++;
    if(gs_uart_iec_app_var[UART_CH0_IR].tx_delay_flg ==TRUE )
    {
        if(gs_uart_iec_app_var[UART_CH0_IR].tx_ready_10ms>0)  gs_uart_iec_app_var[UART_CH0_IR].tx_ready_10ms --;
    }
   

	gs_sys_globaL_var.qf_time++;

	
}

//=======================================================
//��������Interrupt_Int1
//�������ã��˿��ж�1���ж�����				��13H��
//======================================================= 
void Interrupt_Int1 (void) interrupt 2 using 2
{
    TCON &= ~BIT3;
}
//=======================================================
//��������Interrupt_Timer1
//�������ã��ں�timer1�жϣ��ж�����		��1BH��
//======================================================= 
void Interrupt_Timer1 (void) interrupt 3 using 2
{
    TCON &= ~BIT7;
	
	TL1  = LOBYTE(T1_80MS_CNT);         //10ms
    TH1  = HIBYTE(T1_80MS_CNT);
//	P9DO |= BIT2;
////	UART4_INITLIZE_OFF();
////	UART4_TX_DIR_OUT();
////	P21FS = 0;
	UART4_TX_HIGH();		//�����彫��������õ�
////	P2OD &= ~PIN_UART4_TX;
	TR1 = 0;
	ET1 = 0;


// user coding begin

}

//=======================================================
//��������Interrupt_Timer2
//�������ã��ں�timer2�жϣ��ж�����		��2BH��
//======================================================= 
void Interrupt_Timer2 (void) interrupt 5 using 2
{
	T2CON &= ~BIT7;
   //user coding begin
//   if(gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 > 10000)
//   {
   	  	TL2  = LOBYTE(T2_01MS_CNT);         //5ms
		TH2  = HIBYTE(T2_01MS_CNT);
//   }
//   else
//   {
//   		TL2  = LOBYTE(T0_10MS_CNT);         //10ms
//	    TH2  = HIBYTE(T0_10MS_CNT);
//   }
	CLRWDT(); 
	api_handl_bar_display_1ms();
//	 gs_sys_run.back_fg |= BIT3_FONT_FG_1MS;
	
}

//=======================================================
//��������Interrupt_Uart1
//�������ã��ں�uart1�жϣ��ж�����			��3BH��
//=======================================================
void Interrupt_Uart1 (void) interrupt 7 using 2
{
    if (SCON1 & BIT0)
    {// �����жϱ��
        SCON1 &= ~BIT0;
        // user coding begin
    }
    else if (SCON1 & BIT1)
    {// �����жϱ��
        SCON1 &= ~BIT1;
        // user coding begin
    }
}


//=======================================================
//��������Interrupt_ExInt2
//�������ã��ⲿ�ж�2���ж�����				��43H��
//ע�⣺������չ�жϱ�־λ�������ں��жϱ�־λ
//=======================================================
void Interrupt_ExInt2 (void) interrupt 8 using 2
{
 uint8 buf;
 uint8 temp_ifg;

    temp_ifg = ExInt2IE & ExInt2IFG;        // ��ֹ��δʹ���жϵ�λ���� //

////////////////////////////////////////////////////////////////////////
//  UART2  TX 	START
////////////////////////////////////////////////////////////////////////
//    if (temp_ifg & BIT0)
//    {// uart2 translate interrupt flag
//        ExInt2IFG &= ~BIT0;        //BIT0, ExInt2IFG�����жϱ�ʶ //
//        SCON2 &= ~BIT1;           //BIT1 , UART2��SCON2�еķ����жϱ�ʶ //
//        // user coding begin
//        if (gs_uart_drv_var[UART_CH0_IR].tx_len>0)
//        {
//            buf=(gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index]);
//            if(gs_uart_drv_var[UART_CH0_IR].bits_len==7)
//           {
//                 // 7λ���ݴ���ʽ //
//                 ACC=(buf &0x7F);
//                if(P)
//                {
//                    buf |= BIT7;
//                }
//                else
//                {
//                    buf&= ~BIT7;
//                }
//            }
//            SBUF2 = buf;
//            gs_uart_iec_app_var[UART_CH0_IR].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
//            gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index] =0;
//            gs_uart_drv_var[UART_CH0_IR].buff_index++;
//            gs_uart_drv_var[UART_CH0_IR].tx_len -- ;
//        }
//        else
//        {
//            //�������  //
//            gs_uart_drv_var[UART_CH0_IR].buff_index = 0;
//            gs_uart_drv_var[UART_CH0_IR].status = UART_STATUS_BUSY_TX_OVER;  
//					  En_UART4_rx();
//					  Init_UART_soft(UART_CH0_IR);
//        }
//    }

//////////////////////////////////////////////////////////////////////////
////  UART2  TX 	END
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////  UART2  RX 	START
//////////////////////////////////////////////////////////////////////////
//    if (temp_ifg & BIT1)
//    {// uart2 receive interrupt flag
//        ExInt2IFG &= ~BIT1;
//        SCON2 &= ~BIT0;
//        // user coding begin
//        buf = SBUF2;
//        gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index] = buf&0x7f;//  7λ����λ//
//        gs_uart_iec_app_var[UART_CH0_IR].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
//        gs_uart_drv_var[UART_CH0_IR].buff_index++;
//        if(gs_uart_drv_var[UART_CH0_IR].buff_index>4)
//        {
//            NOP();
//            NOP();
//        }
//	      gs_uart_drv_var[UART_CH0_IR].buff_index %= UART_BUFF_SIZE;
//        if(gs_uart_drv_var[UART_CH0_IR].buff_index>0x15)
//        {
//            NOP();
//            NOP();
//        }
//    }
////////////////////////////////////////////////////////////////////////
//  UART2  RX 	END
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//  UART4  TX 	START
////////////////////////////////////////////////////////////////////////
    if (temp_ifg & BIT2)
    {// uart4 translate interrupt flag
        ExInt2IFG &= ~BIT2;
        SCON4 &= ~BIT1;
		if (gs_uart_drv_var[UART_CH0_IR].tx_len>0)
	   {
		   buf=(gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index]);
		   if(gs_uart_drv_var[UART_CH0_IR].bits_len==7)
		  {
				// 7λ���ݴ���ʽ //
				ACC=(buf &0x7F);
			   if(P)
			   {
				   buf |= BIT7;
			   }
			   else
			   {
				   buf&= ~BIT7;
			   }
		   }
		   SBUF4 = buf;
		   gs_uart_iec_app_var[UART_CH0_IR].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
		   gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index] =0;
		   gs_uart_drv_var[UART_CH0_IR].buff_index++;
		   gs_uart_drv_var[UART_CH0_IR].tx_len -- ;
	   }
	   else
	   {
		   //�������  //
		   gs_uart_drv_var[UART_CH0_IR].buff_index = 0;
		   gs_uart_drv_var[UART_CH0_IR].status = UART_STATUS_BUSY_TX_OVER;	
	     En_UART4_rx(); 
			 Init_UART_soft(UART_CH0_IR);   
		 }
	 /*Uart4_Transmit();
        // user coding begin
    if(gs_RacCtrl.ucStatus==Rac_Send)
    {
      if(gs_RacCtrl.ucSendPoint<gs_RacCtrl.ucSendLen)   
      {
        i=Lib_check_bitnum(gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint]);            //������У��λ
        if(i==0)
        {    
          SCON4 |=0x08;                        //żУ���ֵ  0x1906��bit3 1
        }
        else
        {    
          SCON4 &=0xf7;                  //��У���ֵ  0x1906��bit3 0
        }
        SBUF4 = gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++];
      }
      else
      {
        gs_RacCtrl.ucStatus=Rac_Rev;  
      }																
    }*/
   }
////////////////////////////////////////////////////////////////////////
//  UART4  TX 	END
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//  UART4  RX 	START
////////////////////////////////////////////////////////////////////////
    if (temp_ifg & BIT3)
    {
		// uart4 receive interrupt flag
        ExInt2IFG &= ~BIT3;
        SCON4 &= ~BIT0;
		buf = SBUF4;
		   gs_uart_drv_var[UART_CH0_IR].buff[gs_uart_drv_var[UART_CH0_IR].buff_index] = buf&0x7f;//  7λ����λ//
		   gs_uart_iec_app_var[UART_CH0_IR].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
		   gs_uart_drv_var[UART_CH0_IR].buff_index++;
		   if(gs_uart_drv_var[UART_CH0_IR].buff_index>4)
		   {
			   NOP();
			   NOP();
		   }
		gs_uart_drv_var[UART_CH0_IR].buff_index %= 250;//UART_BUFF_SIZE;
		if(gs_uart_drv_var[UART_CH0_IR].buff_index>0x15)
		{
			NOP();
			NOP();
		}
       /* if(gs_RacCtrl.ucStatus==Rac_Rev)
        {
            if(gs_RacCtrl.ucRevPoint<gs_RacCtrl.ucRevLen)
            {
                gs_RacCtrl.ucBuf[gs_RacCtrl.ucRevPoint++]=SBUF4;
                if(gs_RacCtrl.ucRevPoint==gs_RacCtrl.ucRevLen)
                {
                    gs_RacCtrl.ucStatus=Rac_WaitPro;
                //    if( gs_RacPhy.uc_Sendstat == true)  
                  //  {
                  //     gs_sys_run.font_fg |= BIT0_FONT_FG_readpower; // ������ȡ�¼���ɱ�ʶ
                 //   }
                }
            }
        }*/

    }
////////////////////////////////////////////////////////////////////////
//  UART4  RX 	END
////////////////////////////////////////////////////////////////////////


    if (temp_ifg & BIT4)
    {// uart2timer0 interrupt
        ExInt2IFG &= ~BIT4;
        // user coding begin  
    }
    if (temp_ifg & BIT5)
    {// uart4timer0 interrupt
        ExInt2IFG &= ~BIT5;
        // user coding begin    
    }
    if (temp_ifg & BIT6)
    {// flash erase interrupt
        ExInt2IFG &= ~BIT6;
        // user coding begin  
    }

////////////////////////////////////////////////////////////////////////
//   ACTIVE ENERGY PLUSE accumulate 	START
////////////////////////////////////////////////////////////////////////
    if (temp_ifg & BIT7)
    {// CF impluse interrupt
        ExInt2IFG &= ~BIT7;
        // user coding begin
	  gs_energy_var.pluse[1]++;    //�й��ܵ����ۼ� //
   //  gs_mot_drv_var.u8_cnt ++;
    }
////////////////////////////////////////////////////////////////////////
//   ACTIVE ENERGY PLUSE accumulate 	END
////////////////////////////////////////////////////////////////////////


    EXIF &= ~BIT4;
}

//=======================================================
//��������Interrupt_ExInt3
//�������ã��ⲿ�ж�3���ж�����				��4BH��	
//ע�⣺������չ�жϱ�־λ�������ں��жϱ�־λ		
//=======================================================
void Interrupt_ExInt3 (void) interrupt 9 using 2
{
 unsigned char temp_ifg;
	
    temp_ifg = ExInt3IE & ExInt3IFG;
    if (temp_ifg & BIT4)
    {// uart3timer0 interrupt
       ExInt3IFG &= ~BIT4;
        // user coding begin   
    }
	if (temp_ifg & BIT5)//UART5 ����ͨ��ʱ������ж�
    {// uart3timer0 interrupt
       ExInt3IFG &= ~BIT5;
        // user coding begin   
    }			
    if (temp_ifg & BIT6)
    {// rtc interrupt
        ExInt3IFG &= ~BIT6;
        // user coding begin  
       gs_sys_run.back_fg |= BIT1_BACK_FG_RTC;   //������ʾ������ʶ��Ч //  //  RTC ÿ��������ж�  //
        gs_emu_adj_var.sec_flg=TRUE;           // Ӳ���̽�У���붨ʱ��ʶ //

    }


    if (temp_ifg & BIT7)	//  �޹�
    {// CF2 interrupt
        ExInt3IFG &= ~BIT7;
	    gs_energy_var.pluse[0]++;    //�й��ܵ����ۼ� //
		///////////////////////////////////////////////////
		//����ģʽ�½������������		 cxy 2019-04-02
		if(IsModeTest == TRUE)
		{
			UART4_FUNC_DIS();
			UART4_TX_LOW();
			UART4_TX_DIR_OUT() ;
//			//�����彫��������ø� 
//			P9DO &= ~BIT2;//�ø߹رձ�����
//			//����80ms��ʱ��
			TL1	 = LOBYTE(T1_80MS_CNT);	   //80ms  Timer1
			TH1	 = HIBYTE(T1_80MS_CNT);
			Start_Timer1();
		}
		else
		{	//�ر�80ms��ʱ��
			TR1 = 0;
			ET1 = 0;
		}
		//��ʾ��������ʾ��������coding	//���������
		gs_dis_pixel_var.dis_buff[10] &= ~(BIT7+BIT6+BIT5);
		if(MD_dis_delay == 0)
		{
			Write_LCD(&gs_dis_pixel_var.dis_buff[0]);
		}
		else
		{
			Full_SEG(0xFF);
		}
		Bar_No = 0;//����������0˵���������Ǳ���յ�
		//t_count = 0;
		TL2  = LOBYTE(T2_01MS_CNT); 
		TH2	 = HIBYTE(T2_01MS_CNT);
		TR2 = 1;
		ET2 = 1;
		//������ʱ��
      	//////////////////////////////////////////////////
    }
    EXIF &= ~BIT5;
}
//=======================================================
//��������Interrupt_ExInt4
//�������ã��ⲿ�ж�4���ж�����				��53H��
//ע�⣺������չ�жϱ�־λ�������ں��жϱ�־λ
//======================================================= 
void Interrupt_ExInt4 (void) interrupt 10 using 2
{

 unsigned char temp_ifg;
 
    temp_ifg = ExInt4IE & ExInt4IFG;
//    if (temp_ifg & BIT0)
//    {
//        ExInt4IFG &= ~BIT0;
//        // user coding begin   
//    }
//    if (temp_ifg & BIT1)
//    {
//        ExInt4IFG &= ~BIT1;
//        // user coding begin    
//    }
//    if (temp_ifg & BIT2)
//    {
//        ExInt4IFG &= ~BIT2;
//        // user coding begin    
//    }
    if (temp_ifg & BIT3)
    {
        

		 ExInt4IFG &= ~BIT3;
        // user coding begin   
    }

   
    EXIF &= ~BIT6;
}

//=======================================================
//��������Interrupt_PF
//�������ã�ϵͳ����Ԥ���жϣ��ж�����		��63H��
//======================================================= 
/*void Interrupt_PF (void) interrupt 12 using 2
{// powerdown interrupt
    EICON &= ~BIT3;
    // user coding begin
    Int27();    
}*/
	


/***************************************************************
*    END
****************************************************************/

