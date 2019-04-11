/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_Tou.c
**Author		: maji
**date			: 2016-04-20
**description	: ϵͳʱ�괥����Ĵ������C����
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



RTC_TYPE gs_CurDateTime;
TOU_VAR  gs_tou_var;
AUTO_ClrMeter_VAR  gs_auto_clrMeter_var;
uint8 button_flag = 0;
uint8 button_delay = 0;
uint8 MD_flag = 0;
uint8 MD_delay = 1;
uint8 MAGN_flag = 0;
uint8 MAGN_delay = 0;
uint8 MD_dis_delay = 0;
INT8U Bill_Data;
uint8 KEY_READ_FLAG = 0;
uint8 KEY_READ_DELAY = 0;
uint8 IsModeTest = FALSE;
uint8 TestDelay = 0;
uint8 TestDelaySecond = 0;
uint8 Bar_No = 0;
float timdelay = 0;
float t_count =0;

	volatile float p_val;
	volatile float t_val;
	float c;
	float e;
/*****************************************************************************
** Function name    :Proc_get_clock
**
** Description         :��ȡ��ǰRTC ʱ��ȫ�ֱ�������  
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void Proc_get_clock(void)
{
 RTC_TYPE rtc_data_tmp;

    //��ȡ��ǰʱ����������   //
    Get_RTCTime(&rtc_data_tmp);  
    if(Lib_Check_RTC_data_FORM(&rtc_data_tmp.Week) ==FALSE)
    {
        mem_read(&rtc_data_tmp.Week, ADR_METER_VAR_RTC, 7, MEM_E2P1);
        if(Lib_Check_RTC_data_FORM(&rtc_data_tmp.Week) ==FALSE)
       {
            // Ĭ�� 2016-4-20  12:30:30   WEEKDAY=3(��3) //
            rtc_data_tmp.Week= 3;       
            rtc_data_tmp.Year = 0x16;
            rtc_data_tmp.Month = 0x04;
            rtc_data_tmp.Day= 0x20;
            rtc_data_tmp.Hour= 0x12;
            rtc_data_tmp.Minute= 0x30;
            rtc_data_tmp.Second= 0x30;
            mem_db_write(ADR_METER_VAR_RTC, &rtc_data_tmp.Week, 7, MEM_E2P1);
        }
        Set_RTCTime(&rtc_data_tmp);   
    }
    Lib_Copy_Str_TwoArry(&gs_CurDateTime.Week, &rtc_data_tmp.Week, 7);    //���ݸ�ֵ //
}


/*****************************************************************************
** Function name    :Proc_init_tou_var
**
** Description         :��ȡ��ǰRTC ʱ��ȫ�ֱ�������  
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void Proc_init_tou_var(void)
{
    Proc_get_clock();
      
}
void api_handl_button_pre_10ms(void)
{
	if(Judge_ERR_COVER_key() == FALSE && KEY_READ_FLAG == 0)
	{
		KEY_READ_FLAG = 1;
		KEY_READ_DELAY = 3;		 //����3�����testģʽ
	}
	if(Judge_ERR_COVER_key() == TRUE)
	{
		KEY_READ_FLAG = 0;
		KEY_READ_DELAY = 0;
	}
	if(Judge_PG_DN_key() == TRUE && button_delay == 0)
	{
		button_flag = 1;
		button_delay = 1;
	//	api_updated_LCDDisplayItem_per_second();
	}
	if(Judge_PG_DN_key() == FALSE)
	{
		button_delay = 0;
	}
	if(Judge_MD_CLEAR_key() == TRUE && MD_delay == 0)
	{
		MD_flag = 1;
		MD_delay = 1;
	}
	if(Judge_MD_CLEAR_key() == FALSE)
	{
		MD_delay = 0;
	}
	if(Judge_MAGN_CHK_key() == TRUE && MAGN_delay == 0)
	{
		MAGN_flag = 1;
		MAGN_delay = 1;
//			api_updated_LCDDisplayItem_per_second();
	} 
	if(Judge_MAGN_CHK_key() == FALSE)
	{
		MAGN_delay = 0;
	}
}

void api_handl_bar_display_1ms(void)  //0.8ms
{
//	volatile float p_val;
//	volatile float t_val;
//	float c;
//	float e;

	//��ȡ���� //0.72kW -> 5S    7.2kW -> 0.5s	   240*200=48kW
//	pow_val = 72147;

	if(p_val > 0)
	{
		c= (t_count);
//		if(gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32 > 10000)
//		{
//			e = (t_val*200);
			e=	(t_val*42);	 //5ms
//		}
//		else
//		{
////			TL2  = LOBYTE(T0_10MS_CNT);         //10ms
////	    	TH2  = HIBYTE(T0_10MS_CNT);
//			e=	(t_val*25);	//10ms
//		}
	//	
		if(c >= e)
		{	
			Bar_No++;
			t_count = 0;
			switch(Bar_No)
			{
				case 1:
	//				gs_dis_pixel_var.dis_buff[10] &= ~(BIT7+BIT6+BIT5);
					gs_dis_pixel_var.dis_buff[10] |= BIT7;
					break;
				case 2:
//					gs_dis_pixel_var.dis_buff[10] &= ~(BIT7+BIT6+BIT5);
					gs_dis_pixel_var.dis_buff[10] |= BIT6+BIT7;
					break;
				case 3:
//					gs_dis_pixel_var.dis_buff[10] &= ~(BIT7+BIT6+BIT5);
					gs_dis_pixel_var.dis_buff[10] |= BIT7+BIT6+BIT5;
					TR2 = 0;
					ET2 = 0;
					break;
				default:
	//				gs_dis_pixel_var.dis_buff[10] &= ~(BIT7+BIT6+BIT5);
					break;
			}
			if(MD_dis_delay == 0)
			{
				Write_LCD(&gs_dis_pixel_var.dis_buff[0]);
			}
			else
			{
				Full_SEG(0xFF);
			}
		}
		else
		{
			t_count++;
		}
	}
}

/*****************************************************************************
** Function name    :Proc_handl_tou_10ms
**
** Description         :10ms�жϲ�����ĳ�����        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void Proc_handl_tou_10ms(void)
{
	uint8 i;

	//ͨѶ����10ms ���� //
	for(i=0;i<UART_CH_NUM;i++)
	{
	    Hdl_UART_10ms_run(i);
	    api_handl_COMM_pre_10ms(i);
	}
	api_handl_button_pre_10ms();
	
//	api_handl_bar_display_10ms();
//	//48kW -> 0.075S
}



/*****************************************************************************
** Function name    :Proc_handl_tou_10ms
**
** Description         :10ms�жϲ�����ĳ�����        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void Proc_handl_tou_1s(void)
{
//	INT8U Bill_Data;

//	NEW_MMD_DATA_UNIT st_mmd_unit;
//	SUM_MMD_DATA_UNIT sum_st_mmd_unit;
	Proc_get_clock();
//	mem_db_check();
	CLRWDT();	// �忴�Ź�//
	if(gs_emu_run_var.delay_emu>0)
	{
		 gs_emu_run_var.delay_emu--;//emu�ȶ�ʱ��Ϊ3��
		 api_Measure_ram_INIT();
	}
	//�����ȶ���ɨ���¼�
	if(gs_emu_run_var.delay_emu==0)
	{
		// ˲ʱ����ȡ//
		// OpenCF();
		api_measure_deal_pre_second();
		CLRWDT();	// �忴�Ź�//
		//���������봦�� //
		api_accum_current_energy_data_pre_second();
		CLRWDT();   // �忴�Ź�//
		api_even_Detect_sec();
		//��������㴦�� //
		api_updata_md_var_min_per_second(); 
		CLRWDT();	// �忴�Ź�//
		//Ӳ���̽��������봦��//   
		Proc_Check_PROKEY_Pre_sec();
		//LED��״̬ˢ��//
		api_updated_LED_per_second();
		api_measure_VBAT_pre_min();   
	}

	if(MD_dis_delay == 0)//�������������Ժ�3��ȫ��
	{
	 	// ��ʾ��Ŀÿ��ı䴦��//        
    	api_updated_LCDDisplayItem_per_second();    
     	// ��ʾ����ÿ����´���//                
    	api_updated_LCDDisplayPixel_per_second();
	}
	else
	{
		MD_dis_delay -- ;
	}
     //���Ӽ�����  //
#if meter_type == D_1P2W

	 //�޹�����
	 TimeEventProc();
#endif
    if(gs_CurDateTime.Second == 0)
    {
		Check_EMU_AdjParam();
		CLRWDT();	// �忴�Ź�//
		api_updata_md_var_slid_per_minute();   
		CLRWDT();	// �忴�Ź�//
		//�ն���
		api_freeze_energy_Ram_pre_day();
		api_freeze_energy_pre_day(); 
//		mem_read(&Bill_Data, ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), 1, MEM_E2P1);
		#if (BILL_MAX_NUM>0)
		if(Bill_Data == 0x30)
		{
			CLRWDT();
			api_handl_settle_per_minute();
		}
		CLRWDT();	// �忴�Ź�//
		#endif
		LoadPro_Rec();         
		CLRWDT();	// �忴�Ź�//
		#if (TARIFF_MAX_NUM>1)
		api_update_triffNo_pre_minute();
		#endif	
			 
    }
	///////////////////////////////////////////////////////////////////////////////////////////////
	//�����л�ģʽ
	if(Judge_ERR_COVER_key() == FALSE)
	{
		if(KEY_READ_DELAY > 0)
		{
			KEY_READ_DELAY--;	
		}else{
			if(IsModeTest == FALSE)
			{
				IsModeTest = TRUE;
				TestDelay = 10;	  //min
				TestDelaySecond = gs_CurDateTime.Second;
				//Һ����ʾtestģʽ  code
			}
			else
			{
				IsModeTest = FALSE;
			}
		}
	}else{
		if(TestDelay > 0)
		{
			if(gs_CurDateTime.Second == TestDelaySecond)
				TestDelay --;
		}else{
			IsModeTest = FALSE;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
     if(gs_CurDateTime.Second == 0x25)
     {
        Handl_RTC_Adj_per_minu();   
		CLRWDT();	// �忴�Ź�//
     }  

  
    if(gs_CurDateTime.Second == 0x31)
     {
       api_measure_temperature_pre_min();   
	   CLRWDT();   // �忴�Ź�//
     } 
	
}


//-------------------------------------------------------------------------------//
//  Function:      void get_default_mem_data(INT8U index)     
//  Description:  
//  Parameters:   ��  
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
void  Proc_Auto_ClrMeter_Pre_sec(void)
{

   ;
}

//-------------------------------------------------------------------------------//
//  Function:      void get_default_mem_data(INT8U index)     
//  Description:  
//  Parameters:   ��  
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//


void  Initlize_Auto_ClrMeter_Var(void)
{

	gs_auto_clrMeter_var.u8_sec_cnt=0;
	gs_auto_clrMeter_var.u8_flg = 0;

}





/***************************************************************
*    END
****************************************************************/

