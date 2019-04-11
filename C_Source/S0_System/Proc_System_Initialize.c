/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_System_Initialize.c
**Author		: maji
**date			: 2016-04-20
**description	: ϵͳ��λ��ʼ���������C����
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>



/****************************************************************
Function:           Proc_Initialize_systerm_normal_mode 
Description:            ϵͳ�ϵ�/��λ��ʼ������ 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_handle_when_systerm_reset(void)
{
   #if(MCU_TYPE == MCU_G80F92X)   // ��ӱоƬ //
	IEN0=0x00;
	IEN1=0x00;
	gs_sys_globaL_var.reset_status = RSTSTAT;
	Switch_Clk_4P096M();
   #endif

   #if(MCU_TYPE == MCU_V98XX)   //��оƬ //
   gs_sys_globaL_var.reset_status = Systate;
         #ifndef _RTC_CAL    //����Ҫ�²�����ʱִ���������� //
            if((RTCCH != 0) | (RTCCL != 0))
            {
                RTCPEN = 0x96;
                RTCPWD = 0x57;
                RTCCH = 0x00;
                RTCCL = 0x00;
                RTCPEN = 0x96;
                RTCPWD = 0x56;
            }
            NOP();
          #endif

    SPCFNC=1;  		//����FLASH����
    XBYTE[0x0402]=0x86;
    SPCFNC=0;
    CBANK=0X01;
    Set_McuClock(FSYSCLK);   //SetMCUPLL();  6Mhz
   #endif


    Init_GPIO_sleep();
    E2p_IIC_Initail();
    E2p_Stop_i2c();

}


/****************************************************************
Function:   		Proc_Initialize_INRam
Description: 		�ϵ��ʼ��RAM���
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_INRam(void)
{
//	uint8 Bill_Data;
	uint8 i,j;
//	ST_U16_U08 u16_month_A,u16_month_B;
//	ST_U16_U08 u16_day_A,u16_day_B;
	uint8 last_rec_dateTime[6];//���һ�ζ���ʱ��
	uint8 param_settel[2];
//	uint8 max_dateTime;


	INT8U eeprom_flg[3];
	//////////dubge   ��E2P��ʼ��////////////
	//mem_db_clr(0XA5);
	////////////////////////
	////��ȡEEPROM�Ƿ��״��ϵ��־λ����flg=0x653412Ϊ�Ѿ��Ϲ���ı�ƣ�///////
	////��֮���ñ�ƴ�δ�Ϲ��磬�������������� ˫У����У��λ���� yang 2017-12-29////
	mem_read(&eeprom_flg[0], ADR_METER_PARAM1_EEPROM_Flag, 3, MEM_E2P1);
	if((eeprom_flg[0]!=0x65)||(eeprom_flg[1]!=0x34)||(eeprom_flg[2]!=0x12))
	{
		d_flag = 1;
		api_freeze_energy_pre_day(); //�ϵ�ʱ���ն���EEP������������
		mem_fill(ADR_BLOCK101_BILL1_DATA_E2P, 0x1000, 0x00, MEM_E2P1);//
		mem_db_clr(0XA5);//��˫����ȥУ������
		CLRWDT();
	}

//	d_flag = 1;
//	api_freeze_energy_pre_day(); //�ϵ�ʱ���ն���EEP������������

	if(gs_sys_globaL_var.work_mode == NORMAL_MODE)
	{
		mem_db_check();  //����У���� //
		CLRWDT();
	}
	////////////////////////////////////////////////
	//  S0_System �����RAM��ʼ��
	////////////////////////////////////////////////
	Proc_sys_data_init();
	Proc_init_Check_var();
	CLRWDT();
	////////////////////////////////////////////////
	//  S1_HardDrv �����RAM��ʼ��
	////////////////////////////////////////////////
	mem_read((uint8 *)&gs_adj_emu_param, ADR_BLOCK70_EMU_PARAM_E2P, LEN_BLOCK70_EMU_PARAM_E2P, MEM_E2P1);
	Init_EMU_RAM();    
	Proc_init_tou_var();

	////////////////////////////////////////////////
	//  S3_ApiPlat �����RAM��ʼ��
	////////////////////////////////////////////////
	Init_LoadPro_param();
	CLRWDT();
	api_init_md_data_ram();
	api_init_current_energy_data_ram();
	api_Measure_ram_INIT();
	api_measure_VBAT_pre_min();
	api_measure_temperature_pre_min();   
	CLRWDT();
#ifdef _RTC_CAL    //����Ҫ�²�����ʱִ������d���� //
	Handl_RTC_Adj_per_minu();
#endif

#if (TARIFF_MAX_NUM>1)
	api_update_triffNo_pre_minute();  
	CLRWDT();
#endif

#if (BILL_MAX_NUM>1)
if (Bill_Data == 0x30)
{
	CLRWDT();
	i = 0;

	mem_read(&last_rec_dateTime[0],ADR_BLOCK161_EVENII_PWN_E2P+6,5,MEM_E2P1);
	mem_read(&param_settel[0], ADR_METER_PARAM1_SETL_DDHH, 2, MEM_E2P1);
	if(last_rec_dateTime[2]<param_settel[0])
	{	
		i = 1;	
	}else{
		i = 0;
	}
	i = i + gs_CurDateTime.Month - last_rec_dateTime[1];
	if (gs_CurDateTime.Month < param_settel[0])
	{
		i = i - 1;
	}else{
		i = i + 0;
	}
	if (i > 6)
	{	i = 6;	}
	if (i > 0)
	{
		for(j=0;j<i;j++)
		{
			api_write_bill_data(BILL_ID0_MOTTH_SETTL);
			CLRWDT();
		}
		CLRWDT();
		api_clr_current_MD_data();
	}
}
#endif

	////////////////////////////////////////////////
	//  S4_ApiUser �����RAM��ʼ��
	////////////////////////////////////////////////
	api_init_display();
}


/****************************************************************
Function:   		void SysInitialize(void)  
Description: 		ϵͳ�ϵ�/��λ��ʼ������ 
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_sleep_when_reset(void)
{
   uint8 volatile  check_cnt;
   uint8 volatile  check_cnt2;
   RTC_TYPE rtc_data_tmp;
  
    check_cnt=0;
    check_cnt2 = 0;

   CLRWDT(); 
#if(MCU_TYPE == MCU_G80F92X)   // ��ӱоƬ //
 
   EMUIE=0x00;  
   EMUSR=0x00;  
   LDOCON=0x00;
   CLRWDT();     
   Init_GPIO_sleep();
  // Config_CpuReg_Sleep();  
   IEN0=0x00;
   IEN1=0x00;                       
   TPCON=0x00;
   Close_SecOut(); 
/////////////////////////////////////
    gs_sys_globaL_var.work_mode = SLEEP_MODE;
    api_Measure_ram_INIT();          //���˲ʱ�� //
    api_check_current_energy_data();		//  ��ʼ������//
    //��ʼ������״̬�͹���״̬��ȫ�ֱ�������//
    mem_read(&gs_sys_globaL_var.open_cover_status, ADR_METER_VAR_OPENED, 1, MEM_E2P1);
    mem_read(&gs_sys_globaL_var.meter_factory_status, ADR_METER_PARAM1_FC_CFG, 1, MEM_E2P1);   //����0=0x5A ������״̬ // 
    Init_LCD();
    Init_RTC();
    gs_dis_ctl_var.item=0x03;
    Link_Get_DisTab_Var(gs_dis_ctl_var.item) ;    		// ��ȡ��ʾ��Ŀ��Ϣ  //
    api_updated_LCDDisplayPixel_per_second();
///////////////////////////////////////////////// 
   CLRWDT();

   CLKCON=Bin(00000000);       //���ٵ�32K  
   IT21=0;
   IT20=1;                    //�ⲿ�ж�2�½��ش���
   IEN1|=Bin(10000000);       //����LPD�ж�  
   IEN1|=Bin(00000010);       //�����ⲿ�ж�2
   IEN1|=Bin(00001000);       //����RTC�ж�
   EA=1;


    while( 1 )
    {		
       CLRWDT();                
        Enter_PwrDwnMode();         // ��������״̬ //
        CLRWDT();                          //���Ѻ�Ĵ��� //

        if(gs_sys_run.back_fg&BIT1_BACK_FG_RTC)
        {
            gs_sys_run.back_fg &= ~BIT1_BACK_FG_RTC;    
            //�µ�״̬�µĿ����ϸǼ�� //         
            api_handl_dis_sleep();
        }   

        if(CheckPOWERUP()== TRUE)		//�е��ж�  //
        {
            Feed_WDT(0x02);        //���ÿ��Ź���λ����֤�ɿ����� //
            while(1);   
        }

        if(Check_LX_mode()== TRUE)		//PT��Դ�ж�  //
        {
            Feed_WDT(0x02);        //���ÿ��Ź���λ����֤�ɿ����� //
            while(1);   
        }

   } 

#endif


#if(MCU_TYPE == MCU_V98XX)   //��оƬ //
//	CLRWDT();     
//	api_Measure_ram_INIT();          //���˲ʱ�� //
//	api_init_current_energy_data_ram();//  ��ʼ������//
//	//api_check_current_energy_data();		//  ��ʼ������//
//	Init_LCD();
//	api_handl_dis_sleep();
//	Init_GPIO_sleep();
//	SetExtRTC(0x01,0x02);// Сʱ����
//	//SetExtRTC(0x01,0x01);// ���ӻ���//
//	IOWK|=BIT0;
//	Set_McuSleep();
//	if((Systate&BIT0)==0x01)  //�����Ժ��˹��ϵ�ȫ���Լ�
//	{   
//		sys_err();	  //����ϵ������ߴ��� //
//	}

	CLRWDT();     
	api_Measure_ram_INIT();          //���˲ʱ�� //
//	Init_GPIO_run();
//	E2p_IIC_Initail();
//	E2p_Start_i2c();
	api_init_current_energy_data_ram();//  ��ʼ������//
	//api_check_current_energy_data();		//  ��ʼ������//

	Init_LCD();
	api_handl_dis_sleep();
	Init_GPIO_sleep();
	//�·�����	2019-01-18
	P14FS = 0x00;
	P1IE |= BIT4;//����ʹ��
	while(1)
	{
		SetExtRTC(0x01,0x02);// Сʱ����
		//SetExtRTC(0x01,0x01);// ���ӻ���//
		IOWK|=BIT0;
		Set_McuSleep();
//		if((Systate&BIT0)==0x01)  //�����Ժ��˹��ϵ�ȫ���Լ�
		CLRWDT();
		Set_McuClock(FSYSCLK_3M2); 
	
		//��ȡ��ǰʱ����������   //
	    Get_RTCTime(&rtc_data_tmp);
		Lib_Copy_Str_TwoArry(&gs_CurDateTime.Week, &rtc_data_tmp.Week, 7);
	
		if((Systate&BIT0)==0x01)//�����Ժ��˹��ϵ�ȫ���Լ�  
		{
			Init_GPIO_run();
	    	E2p_IIC_Initail();
	    	E2p_Start_i2c();
			////
			mem_write(ADR_BLOCK_4Energy_L0_E2P, &energy_data_array[0].buf[0], 5*LEN_EC_UNIT,MEM_E2P1);
			mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit_array[0].buf[0],5*LEN_NEW_MMD_UNIT,MEM_E2P1);
			mem_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit_array[0].buf[0],5*LEN_SUM_MMD_UNIT,MEM_E2P1);		
			/////
			api_freeze_energy_pre_day();  
			sys_err();	  //����ϵ� //  
		}
		api_freeze_energy_Ram_pre_day();
		Proc_display_Power_down_process();
	}
#endif
}
	





/****************************************************************
Function:           Proc_Initialize_systerm_normal_mode 
Description:            ϵͳ�ϵ�/��λ��ʼ������ 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_systerm_normal_mode(void)
{
	Set_McuClock(FSYSCLK_3M2);   //SetMCUPLL();  6Mhz
	Init_GPIO_run();
	Init_LCD();
	Full_SEG(0xFF);
	CLRWDT();
	Proc_Initialize_INRam();  //��ʼ��RAM�� //
//	Drv_relay_powerup_check();	 //�̵�����ʼ����鴦��  //
//	Lnk_SX2176_Intilize();	//LORA//
	Init_Timer();	//��ʱ��
	api_handl_COMM_FRAME_ERROR(UART_CH0_IR);	//TTL ͨѶӲ����ʼ��
	Init_RTC();
	Init_EMU(NORMAL_MODE);
	Check_EMU_AdjParam();
	Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;	
	gs_even_fun_var.into_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // �¼�����ʱ�� �����¼����
	gs_even_fun_var.esc_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // �¼�����ʱ��
	api_deal_even_class_recode(ID_CLASS_II_PWN,END);	
	Lib_Set_String(&gs_even_fun_var.esc_cnt,0,sizeof(EVEN_FUN_VAR));
	KEY_ERR_COVER_INITLIZE() ;
}



/****************************************************************
Function:          Proc_Initialize_systerm_reset
Description:        ϵͳ�ϵ�/��λ��ʼ������ 
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_Initialize_systerm_reset(void)
{

	Proc_handle_when_systerm_reset();   // ϵͳ��λ�Ĵ�������ע��MCU��ͬʱ��Ҫ�����޸ĺ������� //
	if(CheckPOWERUP()== FALSE)		//
	{
		Lib_Delay_ms(300);
	}
	if(CheckPOWERUP()== FALSE)		//
	{
		gs_sys_globaL_var.work_mode = SLEEP_MODE;  
	}
	else
	{
		gs_sys_globaL_var.work_mode = NORMAL_MODE;
	}

	if(gs_sys_globaL_var.work_mode == SLEEP_MODE)
	{
		Proc_sleep_when_reset();                                   // ���߳�ʼ��//
	}
	else
	{
		Proc_Initialize_systerm_normal_mode();    //  �е繩Ӧ����µĳ�ʼ��//
	}
}

void Proc_display_Power_down_process(void)
{
	 //�µ���ʾ

		//��ⰴ��


		if(Judge_PG_DN_key() == TRUE )//&& button_delay == 0)
		{
			button_flag = 1;
//			button_delay = 1;
		//	api_updated_LCDDisplayItem_per_second();
		}
//		if(Judge_PG_DN_key() == FALSE)
//		{
//			button_delay = 0;
//		}
///////////////////////////

		if(button_flag  == 1)
		{
			button_flag = 0;
			if(dis_num_char == 5)
			{
				dis_num_char = 0;
			}
			else
			{
				dis_num_char ++;
			}
			api_updated_LCDDisplayPixel_per_second();
			gs_dis_ctl_var.keep_sec[0] = 6; //���Գ���ʱ��//
		}
		else
		{
			if(gs_dis_ctl_var.keep_sec[0] > 0)
			 	gs_dis_ctl_var.keep_sec[0]--;
		}
		if(gs_dis_ctl_var.keep_sec[0] == 0)
		{
			dis_num_char = 0;
			Bar_No = 0;//�����������ʾ
			IsModeTest = FALSE;
			api_updated_LCDDisplayPixel_per_second();
		}

}

/****************************************************************
Function:           Proc_judge_Power_down_process
Description:       ���й����еĵ����麯��
Input:			no
Parameters:         	no
Returns:			no                        
Others:         
****************************************************************/
void Proc_judge_Power_down_process(void)
{
   	RTC_TYPE rtc_data_tmp;

	if(CheckPOWERUP()==FALSE)
	{
		PMG=1;                  //�رռ���ʱ��
		DisableINT();    				// disable interrupt function  //
		api_accum_current_energy_data_pre_second();
		mem_db_write(ADR_BLOCK00_EC_L0_E2P,&gs_energy_user_data.us_val[0][0].buf[0],LEN_EC_CURRENT,MEM_E2P1);
		
//		d_flag = 1;
//		api_freeze_energy_pre_day(); //�ϵ�ʱ���ն���EEP��������д��EEP
		gs_sys_globaL_var.work_mode = SLEEP_MODE;
//		mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (INT8U*)&LPRunPa.Ptr[0], LP_RunPara_Len,MEM_E2P1); //���ɼ�¼���в��� 
		mem_db_write(ADR_METER_VAR_RTC, &gs_CurDateTime.Week,7,MEM_E2P1);	
		
		gs_even_fun_var.into_cnt[ID_CLASS_II_PWN-ID_CLASS_II_min]=4;  // �¼�����ʱ�䴫��ID - 0-6
		api_deal_even_class_recode(ID_CLASS_II_PWN,START);	
	//�������¼�����
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_NOL1-ID_CLASS_II_min]=4;  // �¼�����ʱ�� �����¼����
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_NOL1-ID_CLASS_II_min]=4;  // �¼�����ʱ��
		//	api_deal_even_class_recode(ID_CLASS_II_NOL1,END);
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_NOL2-ID_CLASS_II_min]=4;  // �¼�����ʱ�� �����¼����
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_NOL2-ID_CLASS_II_min]=4;  // �¼�����ʱ��
		//	api_deal_even_class_recode(ID_CLASS_II_NOL2,END);
		gs_even_fun_var.into_cnt[ID_CLASS_II_L1reverse-ID_CLASS_II_min]=4;  // �¼�����ʱ�� �����¼����
		gs_even_fun_var.esc_cnt[ID_CLASS_II_L1reverse-ID_CLASS_II_min]=4;  // �¼�����ʱ��
		api_deal_even_class_recode(ID_CLASS_II_L1reverse,END);
		//	gs_even_fun_var.into_cnt[ID_CLASS_II_L2reverse-ID_CLASS_II_min]=4;  // �¼�����ʱ�� �����¼����
		//	gs_even_fun_var.esc_cnt[ID_CLASS_II_L2reverse-ID_CLASS_II_min]=4;  // �¼�����ʱ��
		//	api_deal_even_class_recode(ID_CLASS_II_L2reverse,END);

		mem_read(&gs_even_fun_var.even_fun_doingflg[ID_CLASS_II_COVER-ID_CLASS_II_min],ADR_CLASS2FLG_START_E2P+ (ID_CLASS_II_COVER-ID_CLASS_II_min)* LEN_CLASSII_flg,LEN_CLASSII_flg,MEM_E2P1);	
//EEP�����ڹر�IIC֮ǰ
		api_check_current_energy_data();		//  ��ʼ������//
		api_handl_dis_sleep();
			
		Init_GPIO_sleep();

		//�·�����	2019-01-18
		P14FS = 0x00;
		P1IE |= BIT4;//����ʹ��
	  
		E2p_IIC_Initail();
		E2p_Stop_i2c();
		api_Measure_ram_INIT();          //���˲ʱ�� //
		
//		KEY_ERR_COVER_INITLIZE() ;		
//		gs_even_fun_var.esc_cnt[ID_CLASS_II_COVER-ID_CLASS_II_min] =0; 
//		gs_even_fun_var.into_cnt[ID_CLASS_II_COVER-ID_CLASS_II_min] =0;

		gs_dis_ctl_var.keep_sec[0] = 0;//2019-01-18		cxy
		d_flag = 0;
		while(1)
		{
			CLRWDT();
			SetExtRTC(0x01,0x00);//�뻽��
			IOWK|=BIT0;
			Init_RTC();  
			Set_McuSleep();
			
			//		
			CLRWDT();
			Set_McuClock(FSYSCLK_3M2); 
//			Proc_get_clock();
			
			//��ȡ��ǰʱ����������   //
    		Get_RTCTime(&rtc_data_tmp);
			Lib_Copy_Str_TwoArry(&gs_CurDateTime.Week, &rtc_data_tmp.Week, 7);
//			if(Judge_ERR_COVER_key()==TRUE)
//				api_pwl_cover_even_class_recode(ID_CLASS_II_COVER-ID_CLASS_II_min,START);
//			else 
//				api_pwl_cover_even_class_recode(ID_CLASS_II_COVER-ID_CLASS_II_min,END);
			
			if((Systate&BIT0)==0x01)  
			{
				Init_GPIO_run();
    				E2p_IIC_Initail();
    				E2p_Start_i2c();
				////
				mem_write(ADR_BLOCK_4Energy_L0_E2P, &energy_data_array[0].buf[0], 5*LEN_EC_UNIT,MEM_E2P1);
				mem_write(ADR_BLOCK_4MMD_L0_E2P,&st_mmd_unit_array[0].buf[0],5*LEN_NEW_MMD_UNIT,MEM_E2P1);
				mem_write(ADR_BLOCK_4MMD_sum_L0_E2P,&sum_st_mmd_unit_array[0].buf[0],5*LEN_SUM_MMD_UNIT,MEM_E2P1);		
			
//				mem_read(&Bill_Data, ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), 1, MEM_E2P1);
				/////
			//	api_freeze_energy_pre_day();  
				sys_err();	  //����ϵ� //  
			}
			api_freeze_energy_Ram_pre_day();
			
			Proc_display_Power_down_process();		   
    	}
	}
	else
	{
		return;
	}
}


/****************************************************************
Function:   		void sys_err(void)
Description: 		ϵͳ�����Ŀ��Ź���λ����
Input:			no
Parameters:     	no
Returns:			no                        
Others:         
****************************************************************/
void sys_err(void)
{  
    CLRWDT();

    while(1)
    {
		NOP();
		NOP();
		NOP();
    }  
}



/***************************************************************
*    END
****************************************************************/

