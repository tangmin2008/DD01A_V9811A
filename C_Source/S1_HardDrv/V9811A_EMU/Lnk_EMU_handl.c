/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Lnk_EMU_handl.c
**Author		: maji
**date			: 2016-04-20 
**description	: V9811A��EMU����ģ����·�㴦��C����
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


EMU_RUN_VAR   gs_emu_run_var;

/*********************************************************************************************************
** ��������: Init_EMU
** ��������: ��ʼ��EMU
** ��ڲ���: ��
** ���ڲ���: ��
********************************************************************************************************/


void Init_EMU_RAM(void)
{
    Get_EMU_AdjParam();
}


/*********************************************************************************************************
** ��������: Init_EMU
** ��������: ��ʼ��EMU
** ��ڲ���: ��
** ���ڲ���: ��
********************************************************************************************************/
void Init_EMU(uint8 u8_mode)
{

#if meter_type == D_1P3W
    if(u8_mode==SLEEP_MODE)
    {
        CtrlADC5=0X80;     //�ر�Mͨ�� //
        CtrlADC6=0;         //�ر�����AD���� //  
        PMG=1;              //�رռ���ʱ��  //
        return;
    }

    PMG=0;              //�򿪼���ʱ��  //
    //step1  //
    SetMeterCfgACK(0x00,PMCtrl4);   //�ر������ۼӡ�CF ����Լ���/Ǳ���жϵ�· //
    SetMeterCfgACK(0x00,PMCtrl1);   // �رռ���ͨ������ֹͣ����/��Чֵ����  //
    // step2  //
    CtrlADC5=0X80;   
    CtrlADC6=0;   
    if(u8_mode==NORMAL_MODE)
    {
        Set_EmuClock(EMUCLK_3M2);          // 3.2M ������Ƶ //
        ADCOPEN(ADCAPDN+ADCBPDN+ADCUPDN);//OPEN ADC U  A  B
        SetMeterCfgACK(0X889374BC,PARABPF);    // �ֲ�Ҫ�� //
    }
    else if((u8_mode ==DLX_MODE)||(u8_mode ==CONST_MODE))
    {
        Set_EmuClock(EMUCLK_800k);       // 800K������Ƶ //
        ADCOPEN(ADCAPDN+ADCBPDN);//OPEN ADC  A B
        SetMeterCfgACK(0X911D3C9C,PARABPF);    // �ֲ�Ҫ�� // 
    }
    else
    {
        sys_err();          // ��������쳣����ѭ���˳� //
    }
    CtrlADC0=(ADCA4X+ADCB4X+ADCU2X); //ADC A 4X GAIN  ADC B 4x GAIN ADC U 2sX GAIN	

    // step3  //
 
    SetMeterCfgACK(INITGATEP,GATEP);//���ü�����ʼֵ
    SetMeterCfgACK(INITGATEQ,GATEQ);

    // step4 // 
//    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  ���� E1 �����ۼӼĴ���  //
//    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  ���� E1 �����ۼӼĴ��� //
//    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  ���� E1 �������������   //
//    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  ���� E1 �������������   //
//    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  ���� E2 �����ۼӼĴ���//
//    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  ���� E2 �����ۼӼĴ���   //
//    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   ���� E2 �������������  //
//    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   ���� E2 �������������  //
//    // step5// 
//    SetMeterCfgACK(0xC8,PMCtrl4);		//  ?????  //
//    SetMeterCfgACK(0x08,PMCtrl4);
//    SetMeterCfgACK(0x00,PMCtrl4);
    // step6 // 
//    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  ���� E1 �����ۼӼĴ���  //
//    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  ���� E1 �����ۼӼĴ��� //
//    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  ���� E1 �������������   //
//    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  ���� E1 �������������   //
//    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  ���� E2 �����ۼӼĴ���//
//    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  ���� E2 �����ۼӼĴ���   //
//    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   ���� E2 �������������  //
//    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   ���� E2 �������������  //
 
    // step7// 
    SetMeterCfgACK(0x01,GATECP);//Ǳ��1   ,=1���ֹ���巢��    //
    SetMeterCfgACK(0x01,GATECQ);//Ǳ��2   ,=1���ֹ���巢��  //
//    SetMeterCfgACK(0x00,DATACP);//�峣�����ʼĴ��� //

    // step8// 
    SetMeterCfgACK(0x57,PMCtrl1);//BIT7= 0  ����λȷ��Ϊ0  ; BIT6=1 ʹ�ܽǲ�У��  //
                                  //BIT5= 0  IA--I1,IB--I2 ; BIT4=1 ʹ��PM�����źŴ���  //
                                  //BIT3= 0  ����Mͨ�������ź�����; BIT2=1 ʹ��I2�����ź�����ʹ��  //
                                  //BIT1= 1   ʹ��I1�����ź�����ʹ��; BIT0=1 ʹ��U�����ź�����ʹ��  //
    SetMeterCfgACK(0x00,PMCtrl2);// ����������Ĳ����Ĵ�����Ҫ��Ϊ0  //

    SetMeterCfgACK(0x70,PMCtrl3);//������·�й�,��ͨ�͵�ͨ�˲�����Ч //
 //   SetMeterCfgACK(0xD2,PMCtrl4);//BIT7= 1 E2 ����ͨ����/Ǳ���ж�ʹ��  ; BIT6=1  E2 ����ͨ����/Ǳ���ж�ʹ��  //
                                 //BIT5= 1 E2 ���� CF �����������; BIT4=1 ʹ��E1 ���� CF ������� //
                                 //BIT3= 0 ��ֹ�����ۼӺ������������; BIT2=0  E1 ���� CF ��������� CF1 �����E2 ���� CF ��������� CF2 ���  //
                                 //BIT1:0= 10 �������� //
    SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  ���� E1 �����ۼӼĴ���  //
    SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  ���� E1 �����ۼӼĴ��� //
    SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  ���� E1 �������������   //
    SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  ���� E1 �������������   //
    SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  ���� E2 �����ۼӼĴ���//
    SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  ���� E2 �����ۼӼĴ���   //
    SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   ���� E2 �������������  //
    SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   ���� E2 �������������  //
    // step9 // 
    SetMeterCfgACK(0xDE,PMCtrl4);//BIT7= 1  E2 ����ͨ����/Ǳ���ж�ʹ��  ; BIT6=1  E2 ����ͨ����/Ǳ���ж�ʹ��  //
                                                  //BIT5= 0 E2 ���� CF ��������ر�; BIT4=1 ʹ��E1 ���� CF ������� //
                                                  //BIT3= 1 ʹ�������ۼӺ������������; BIT2=0  E1 ���� CF ��������� CF1 �����E2 ���� CF ��������� CF2 ���  //
                                                  //BIT1:0= 10 �������� //
     // step10 // 
    SetMeterCfgACK(0,PHCCtrl1);//��0
    SetMeterCfgACK(0,PHCCtrl2);//��0

    SetMeterCfgACK(0x01,CFCtrl);           //��E2��������ֵ�ۼ�����Ϊ���ݲ���CF2  ��E1��������Ϊ���ݲ���CF	//
 
     // step11// 
    SetMeterCfgACK(0x01,DATACP);//�峣�����ʼĴ��� //
    P9OE&=(~(BIT5));//��CF2 IO���//
    P9FC|=(BIT5);
//    P1OE &=(~(BIT3));//��CF 1 IO���//
//    P13FS |=(BIT2);
    EIE|=0x02;
    ExInt3IE|=BIT7;     // ����CF2�ж� //  �й��ж�

    gs_emu_run_var.delay_emu = 3;   // ��ʼ������ȶ�ʱ��Ϊ2�� //

#endif
#if meter_type == D_1P2W
	if(u8_mode==SLEEP_MODE)
	{
		CtrlADC5=0X80;     //�ر�Mͨ�� //
		CtrlADC6=0;         //�ر�����AD���� //  
		PMG=1;              //�رռ���ʱ��  //
		return;
	}

	PMG=0;              //�򿪼���ʱ��  //
	//step1  //
	SetMeterCfgACK(0x00,PMCtrl4);   //�ر������ۼӡ�CF ����Լ���/Ǳ���жϵ�· //
	SetMeterCfgACK(0x00,PMCtrl1);   // �رռ���ͨ������ֹͣ����/��Чֵ����  //
	// step2  //
	CtrlADC5=0x80;   
	CtrlADC6=0;   
	if(u8_mode==NORMAL_MODE)
	{
		Set_EmuClock(EMUCLK_3M2);          // 3.2M ������Ƶ //
		ADCOPEN(ADCBPDN+ADCAPDN+ADCUPDN);//OPEN ADC U  A 
		SetMeterCfgACK(0X889374BC,PARABPF);    // �ֲ�Ҫ�� //
	}
	else if((u8_mode ==DLX_MODE)||(u8_mode ==CONST_MODE))
	{
		Set_EmuClock(EMUCLK_800k);       // 800K������Ƶ //
		ADCOPEN(ADCBPDN+ADCAPDN);//OPEN ADC  A 
		SetMeterCfgACK(0X911D3C9C,PARABPF);    // �ֲ�Ҫ�� // 
	}
	else
	{
		sys_err();          // ��������쳣����ѭ���˳� //
	}
	CtrlADC0=(ADCA4X+ADCB4X+ADCU2X); //ADC A 4X GAIN  ADC B 4x GAIN ADC U 2sX GAIN	

	// step3  //   
	SetMeterCfgACK(INITGATEP,GATEP);//���ü�����ʼֵ
	SetMeterCfgACK(INITGATEQ,GATEQ);


	// step7// 
	SetMeterCfgACK(0x01,GATECP);//Ǳ��1   ,=1���ֹ���巢��    //
	SetMeterCfgACK(0x01,GATECQ);//Ǳ��2   ,=1���ֹ���巢��  //
	//    SetMeterCfgACK(0x00,DATACP);//�峣�����ʼĴ��� //

	// step8//
	SetMeterCfgACK(0x73,PMCtrl1); 
	//SetMeterCfgACK(0x57,PMCtrl1);//BIT7= 0  ����λȷ��Ϊ0  ; BIT6=1 ʹ�ܽǲ�У��  //
									//BIT5= 0  IA--I1,IB--I2 ; BIT4=1 ʹ��PM�����źŴ���  //
									//BIT3= 0  ����Mͨ�������ź�����; BIT2=1 ʹ��I2�����ź�����ʹ��  //
									//BIT1= 1   ʹ��I1�����ź�����ʹ��; BIT0=1 ʹ��U�����ź�����ʹ��  //
	SetMeterCfgACK(0x00,PMCtrl2);// ����������Ĳ����Ĵ�����Ҫ��Ϊ0  //
	
	SetMeterCfgACK(0x60,PMCtrl3);
	// SetMeterCfgACK(0x70,PMCtrl3);//������·�й�,��ͨ�͵�ͨ�˲�����Ч //

	SetMeterCfgACK(0,0x10f0);    //0x10f0  PPCNT  ���� E1 �����ۼӼĴ���  //
	SetMeterCfgACK(0,0x10f1);    //0x10f1  NPCNT  ���� E1 �����ۼӼĴ��� //
	SetMeterCfgACK(0,0x10f2);    //0x10f2  PPCFCNT  ���� E1 �������������   //
	SetMeterCfgACK(0,0x10f3);   //0x10f3  NPCFCNT  ���� E1 �������������   //
	SetMeterCfgACK(0,0x10f6);   //0x10f6  PQCNT  ���� E2 �����ۼӼĴ���//
	SetMeterCfgACK(0,0x10f7);   //0x10f7  NQCNT  ���� E2 �����ۼӼĴ���   //
	SetMeterCfgACK(0,0x10f8);   //0x10f8  PQCFCNT   ���� E2 �������������  //
	SetMeterCfgACK(0,0x10f9);   //0x10f9 NQCFCNT   ���� E2 �������������  //
	// step9 // 
	SetMeterCfgACK(0x7F,PMCtrl4);//BIT7= 1  E2 ����ͨ����/Ǳ���ж�ʹ��  ; BIT6=1  E2 ����ͨ����/Ǳ���ж�ʹ��  //
								//BIT5= 0 E2 ���� CF ��������ر�; BIT4=1 ʹ��E1 ���� CF ������� //
								//BIT3= 1 ʹ�������ۼӺ������������; BIT2=0  E1 ���� CF ��������� CF1 �����E2 ���� CF ��������� CF2 ���  //
								//BIT1:0= 10 �������� //
	// step10 // 
	SetMeterCfgACK(0,PHCCtrl1);//��0
	SetMeterCfgACK(0,PHCCtrl2);//��0

	SetMeterCfgACK(0x00,CFCtrl);           //��E2��������ֵ�ۼ�����Ϊ���ݲ���CF2  ��E1��������Ϊ���ݲ���CF	//

	 // step11// 
	SetMeterCfgACK(0x00,DATACP);//�峣�����ʼĴ��� //
	P9OE&=(~(BIT5));//��CF2 IO���//
	P9FC|=(BIT5);
	P9OE&=(~(BIT6));//��CF2 IO���//
	P9FC|=(BIT6);
	EIE|=(0x02+0x01);
	ExInt2IE|=BIT7;     // ����CF1�ж� //  �޹��ж�
	ExInt3IE|=BIT7;     // ����CF2�ж� //  �й��ж�

	gs_emu_run_var.delay_emu = 3;   // ��ʼ������ȶ�ʱ��Ϊ2�� //

#endif

}
/*******************************************************************************************
** ��������: Lnk_get_EMU_voltage
** ��������: ��ȡָ��ͨ����ѹ 
** �������: ch: ͨ�����0�������
** �������: uint32 ָ��ͨ����ѹֵ��HEX��,����Ϊ0.01V
** ˵��    : У��ϵ��Ϊ0.01V��/10תΪ0.1V
*******************************************************************************************/
uint32 Lnk_get_EMU_voltage(uint8 ch)
{
    uint32 val,tmpval;
    fp32   f32_tmp;
    ST_U32_U08 TempValue;
    if(ch==WIRE_L1) 
    {    
        val= ReadMeterParaACK(RMSIU);   
        if(val>=0x80000000)
        {
            val=~val+1;
        }       
        f32_tmp = (gs_adj_emu_param.vrms_xs*(fp32)val);
        tmpval = (uint32)f32_tmp;
    }
    else  if(ch==WIRE_L2)  
    {
	if(ReadRaccoon(RegAARTU,1,BPhy)==TRUE)     // ��� 200ms��ȡ�ȴ�    //  ����ʱ��15ms  RegAARTU   
	{
           memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
           Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
           val = TempValue.u32;
	}
	else // ��ȡʧ�ܣ�ˢ�´���// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}

        if(val>=0x80000000)
        {
            val=~val+1;  
        }
        f32_tmp = gs_adj_emu_param.vrms_xs*(fp32)val;
        tmpval = (uint32)f32_tmp;
    }
    else
    {
        tmpval = 0;
    }


    if( tmpval <50)   tmpval =0;

    return(tmpval);


}
/*******************************************************************************************
** ��������: Lnk_get_EMU_current
** ��������: ��ȡָ��ͨ������
** �������: ch: ͨ�����0�������
** �������: uint32 ָ��ͨ������ֵ��HEX��,����Ϊ0.001A
** ˵��    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_current(uint8 ch)
{
   uint32 val,tmpval;
   fp32   f32_tmp;
//   ST_U32_U08 TempValue;

    if(ch==WIRE_L1) 
    {
        val = ReadMeterParaACK(RMSII2);  
        if(val>=0x80000000)
        {
            val=~val+1;  
        } 
        f32_tmp = gs_adj_emu_param.i2rms_xs*val;
        tmpval = (uint32)f32_tmp;
    }
    else  if(ch==WIRE_L2) 
    {
		val = ReadMeterParaACK(RMSII1);
		if(val>=0x80000000)
		{
			val=~val+1;  
		} 
		f32_tmp = gs_adj_emu_param.i2rms_xs*val;
		tmpval = (uint32)f32_tmp;

    }
    else
    {
        tmpval = 0;
    }

    if( tmpval <20)   tmpval =0;

    return (tmpval);

}
/*******************************************************************************************
** ��������: Lnk_get_EMU_activePower
** ��������: ��ȡָ��ͨ������
** �������: ch: ͨ�����
** �������: uint32 ָ��ͨ������ֵ��HEX��,����Ϊ0.1W
** ˵��    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_activePower(uint8 ch)
{
   uint32 val,val2,tmpval;
   fp32   f32_tmp;
   ST_U32_U08 TempValue;

    if(ch==PHASE_TT) 
    {
      val=ReadMeterParaACK(DATAIP);	 
	  if(ReadRaccoon(RegAAP,1,BPhy)==TRUE)     // ��� 200ms��ȡ�ȴ�    //  ����ʱ��15ms
	  {
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
       val2 = TempValue.u32;
	  }
	   else // ��ȡʧ�ܣ�ˢ�´���// 
	  {
	   gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	  }	
	   
	  tmpval = val+val2;
    }
    if(ch==WIRE_L1) 
    {
//       gs_emu_run_var.net_flg &= ~REV_L_ACPOWER;
       val=ReadMeterParaACK(DATAIP);	 
       if(val>=0x80000000)
       {
		val=~val+1;  
//		gs_emu_run_var.net_flg |= REV_L_ACPOWER;
       }
				
    }
   else  if(ch==WIRE_L2) 
    {
//    gs_emu_run_var.net_flg &= ~REV_N_ACPOWER;
	if(ReadRaccoon(RegAAP,1,BPhy)==TRUE)     // ��� 200ms��ȡ�ȴ�    //  ����ʱ��15ms
	{
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
       val = TempValue.u32;
	}
	else // ��ȡʧ�ܣ�ˢ�´���// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}

     if(val>=0x80000000)
     {
	  val=~val+1; 
//	  gs_emu_run_var.net_flg |= REV_N_ACPOWER;  
      }	
    }
    else
    {
        val = 0;
    }

    if(val<REF_START_Pn)
    {
        tmpval =0;    
//        if(ch==WIRE_L1) gs_emu_run_var.net_flg &= ~REV_L_ACPOWER;
//       if(ch==WIRE_L2) gs_emu_run_var.net_flg &= ~REV_N_ACPOWER;
    }
    else
    {
        if(ch==WIRE_L1) f32_tmp = cst_adj_const.Kim_Prms*val;
        if(ch==WIRE_L2) f32_tmp = cst_adj_const.Kim_Prms*val;
        tmpval = (uint32)f32_tmp;  	        
    }


   if((gs_emu_run_var.net_flg&REV_L_ACPOWER)||(gs_emu_run_var.net_flg&REV_N_ACPOWER))
    {
//      gs_measure_status_judge_var.u8_status |= REV_ACPOWER_L ;
     }
   else 
    {
//     gs_measure_status_judge_var.u8_status &=~ REV_ACPOWER_L ;
    }
   

    return (tmpval);
}
/*******************************************************************************************
** ��������: Lnk_get_EMU_freq
** ��������: ��ȡָ��ͨ������Ƶ��
** �������: ch: ͨ�����0�������
** �������: uint32 ָ��ͨ��Ƶ��ֵ��HEX��,����Ϊ0.01Hz
** ˵��    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_freq(uint8 ch)
{
	uint32 val,tmpval;
	ST_U32_U08 TempValue;
    if(ch==WIRE_L1) 
    {    
        val= ReadMeterParaACK(DATAFREQ);          
        tmpval= (819200 *100)/val;			// 819200ΪADC Ƶ��  //
    }
   else  if(ch==WIRE_L2) 
	{
	if(ReadRaccoon(RegFREQ,1,BPhy)==TRUE)     // Ƶ��˲ʱֵ
	{
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
		val = TempValue.u32;
		tmpval= (819200 *100*4)/val;			// 819200ΪADC Ƶ��  //
	}
	else // ��ȡʧ�ܣ�ˢ�´���// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}
    }
    else
    {
        tmpval = 0;
    }
#if (C_Fq == 60)
		tmpval = tmpval*1.2;
#else
		tmpval = tmpval; 
#endif           
    return(tmpval);
}

/*******************************************************************************************
** ��������: Lnk_CAL_EMU_constSum
** ��������: ��������ÿ���������
** �������: 
** �������: 
** ˵��    : 
*******************************************************************************************/
void lnk_CAL_EMU_constSum_per_second(void)
{
	ST_U32_U08 sumval0;
	ST_U32_U08 val1;
//	ST_U32_U08 val2;
//	ST_U32_U08 TempValue;

	//gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);

	//��ȡ���޹�����ģʽ��
	//  mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
	//  mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	//9260F�޹�����д��//
	//Lnk_get_EMU_ractive_const();

	//�й�����д��//
    val1.u32 = 0;

	//���ʻ�ȡ //
#if meter_type == D_1P3W
	val1.u32+=ReadMeterParaACK(DATAIP);
	val1.u32+=ReadMeterParaACK(DATAIQ);	

	if(val1.u32>=0x80000000)
	{
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);
		val1.u32=~val1.u32+1;  
	//	gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// ���๦�ʷ���
	}
	else
		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// ���๦�ʷ���
//	sumval0.u32=REF_START_Pn;
	sumval0.u32=val1.u32;
	if(val1.u32<REF_START_Pn) 
	{
		sumval0.u32=0;
		val1.u32 = 0;
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);//���η���ָʾ
	}
   	sumval0.u32 /= 2;
	SetMeterCfgACK(sumval0.u32,DATACP); 
	gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
#endif

#if meter_type == D_1P2W
//�й�����
	val1.u32=ReadMeterParaACK(DATAIP);
	if(val1.u32>=0x80000000)
	{
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);
		val1.u32=~val1.u32+1;  
	//	gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// ���๦�ʷ���
	}
	else
		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// ���๦�ʷ���
//	sumval0.u32=REF_START_Pn;
	sumval0.u32=val1.u32;
	if(val1.u32<REF_START_Pn) 
	{
		sumval0.u32=0;
		val1.u32 = 0;
		gs_emu_run_var.net_flg &= (~REV_TT_ACPOWER);//���η���ָʾ
	}
	SetMeterCfgACK(sumval0.u32,DATACP); 
	gs_measure_var_data.gs_really[PHASE_TT].dw_p_val.u32 = cst_adj_const.Kim_Prms*val1.u32;
//�޹�����
	val1.u32=ReadMeterParaACK(DATAIQ);
	if(val1.u32>=0x80000000)
	{
		val1.u32=~val1.u32+1;  
	}
//	else
//		gs_emu_run_var.net_flg |= REV_TT_ACPOWER;// ���๦�ʷ���
		
	sumval0.u32=val1.u32;
	if(val1.u32<REF_START_Pn) 
	{
		sumval0.u32=0;
		val1.u32 = 0;
	}
	gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val1.u32/1.568;
#endif
   	if(gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 == 0)
	{
		Bar_No = 0;
	}
}


/*******************************************************************************************
** ��������: 
** ��������: ��ȡ�޹�����
** �������: 
** �������: 
** ˵��    : 
*******************************************************************************************/
void Lnk_get_EMU_ractive_const(void)
{
	ST_U32_U08 Temp_paraqc;
	ST_U32_U08 val1,sumval1;
	ST_U32_U08 val2,sumval2;
	ST_U32_U08 TempValue;
	float err_ec;
	
	val1.u32 = 0;
	Temp_paraqc.u32 = 0;
	val2.u32 = 0;
	//��ȡA·�޹�
	val1.u32=ReadMeterParaACK(DATAIQ);	    
	// ��ȡ���β����Ĵ���
	Temp_paraqc.u32=ReadMeterParaACK(PARAQC);
	
	//B·�����Լ���ѹ������ȡ��ȡ9260F  //
	if(ReadRaccoon(RegAAQ,1,BPhy)==TRUE)     // ��� 200ms��ȡ�ȴ�    //  ����ʱ��76ms  RegIAP
	{
		memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
		Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//'
		val2.u32= TempValue.u32*1.669f; //0.01%����
	}
	else 
	{
		gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
		return;
	}

	if((sumval1.u32 <= REF_START_Pn)||(sumval1.u32 >= REF_START_REV_Pn))// �޹�����
	{
		sumval1.u32 =0; 
	}
	
	if((sumval2.u32 <= REF_START_Pn)||(sumval2.u32 >= REF_START_REV_Pn))// �޹�����
	{
		sumval2.u32 =0; 
	}


	//��ȡ�����Լ����෽��//
	if(val1.u32>=0x80000000)
	{
	//       gs_emu_run_var.net_flg |= REV_TT_RACPOWER; // ���෴��
	}
	else
	{
	//      gs_emu_run_var.net_flg &= (~REV_TT_RACPOWER); // ���෴��
	}

	if(val2.u32>=0x80000000)
	{
	//		gs_emu_run_var.net_flg |= REV_N_RACPOWER; // B·���� //
	}
	else
	{
	//		gs_emu_run_var.net_flg &= (~REV_N_RACPOWER); // B·����//
	}

	  
	  //���ݺ����Լ�B·��С���ж�A·����//
//	  if(Temp_paraqc.u32==0) // ���β����Ĵ���
//	  {
//        if((gs_emu_run_var.net_flg&REV_TT_RACPOWER)!=REV_TT_RACPOWER) // A·����
//        {
//	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //�޹� B·����ֱ�Ӷ��� 
//	       {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //������
//	         {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//	         }
//			 else
//			 {
//			  val2.u32 = val2.u32; //ֱ�Ӷ���
//			 }
//		   }
//		   else 
//		   {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //������
//	         {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//	         }
//			 else
//			 {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//			 }
//		   }
//        }
//		else  //A·��
//		{
//	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //�޹� B·����ֱ�Ӷ��� 
//	       {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //������
//	         {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//	         }
//			 else
//			 {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//			 }
//		   }
//		   else 
//		   {
//	         if(gs_sys_globaL_var.Rac_energy[0]==0x55) //������
//	         {
// 			  val2.u32=~val2.u32+1;  // ȡ������
//	         }
//			 else
//			 {
// 			  val2.u32=val2.u32;  // ֱ�Ӷ���
//			 }
//		   }
//        }
//	     // 9260F �޹�д����β����Ĵ��� 2^31/(2^31*gs_adj_emu_param.w1gain*sumval2.u32*(2.175/1.2/1.086))//
//	     err_ec = (float)((float)(0x80000000/(float)(0x80000000+gs_adj_emu_param.w1gain)));
//		 val2.u32 =  val2.u32*err_ec ;
//		 SetMeterCfgACK( val2.u32,PARAQC);
//	     gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 = cst_adj_const.Kim_Prms2*sumval0.u32;
//	     gs_measure_var_data.gs_really[WIRE_L1].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val1.u32;
//	     gs_measure_var_data.gs_really[WIRE_L2].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val2.u32;
//	  }
//	  else
//	  {
        if((gs_emu_run_var.net_flg&REV_TT_RACPOWER)!=REV_TT_RACPOWER) // A·����
        {
	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //�޹� B·����ֱ�Ӷ��� 
	       {
 			  val2.u32=~val2.u32+1;  //
 			 //{
			   //if(val1.u32 >= val2.u32)
//			   gs_emu_run_var.net_flg|REV_L_RACPOWER ;  //A����
			   TempValue.u32 = val2.u32+ val1.u32;     //A·����ֵ
			   //ȫ��ȡ������
			 //}
		   
		   }
		   else //�޹�B·��
		   {
			   if(val1.u32 >= val2.u32)
			   {
			     TempValue.u32 = val1.u32 - val2.u32 ;        //A·����ֵ
//			     gs_emu_run_var.net_flg&=(~REV_L_RACPOWER);   //A����
	 			 val2.u32=~val2.u32+1;  // ȡ������
			   }
			   else
			   {
//			     gs_emu_run_var.net_flg|REV_L_RACPOWER;   //A����
			     TempValue.u32 = val2.u32 - val1.u32 ;   //A·����ֵ
				 if(gs_sys_globaL_var.Rac_energy[0]==0x55) //������
				 {
				   val2.u32=~val2.u32+1;  // ȡ������
				 }
			   }
		   }
        }
		else  //A·��
		{
	       if(gs_emu_run_var.net_flg&REV_N_RACPOWER) //�޹� B·����ֱ�Ӷ��� 
	       {
			   if(val1.u32 >= val2.u32)
			   {
			     TempValue.u32 = val1.u32 - val2.u32 ;        //A·����ֵ
//			     gs_emu_run_var.net_flg|=REV_L_RACPOWER;   //A����
				 if(gs_sys_globaL_var.Rac_energy[0]!=0x55)   //������
				 {
  				   val2.u32= val2.u32;    //ֱ�Ӷ���
				 }
				 else
				 {
				   val2.u32=~val2.u32+1;  // ȡ������
				 }
				}
			   else
			   {
//			     gs_emu_run_var.net_flg&=(~REV_L_RACPOWER);   //A����
			     TempValue.u32 = val2.u32 - val1.u32 ;       //A·����ֵ
				 val2.u32=~val2.u32+1;  // ȡ������
			   }
		   }
		   else //�޹�B·��
		   {
			     TempValue.u32 = val1.u32 + val2.u32 ;        //A·����ֵ
//			     gs_emu_run_var.net_flg|=REV_L_RACPOWER;   //A����
				 if(gs_sys_globaL_var.Rac_energy[0]!=0x55)   //������
				 {
  				   val2.u32= val2.u32;    //ֱ�Ӷ���
				 }
				 else
				 {
				   val2.u32=~val2.u32+1;  // ȡ������
				 }
		   }
        }
		
	     // 9260F �޹�д����β����Ĵ��� 2^31/(2^31*gs_adj_emu_param.w1gain*sumval2.u32*(2.175/1.2/1.086))//
	     err_ec = (float)((float)(0x80000000/(float)(0x80000000+gs_adj_emu_param.w1gain)));

		 if(val2.u32>0x80000000)
		 {
	      val2.u32=~val2.u32+1;  // ȡ������
		  val2.u32 =  val2.u32*err_ec ;
	      val2.u32=~val2.u32+1;  // ȡ������
		 }
		 else  val2.u32 =  val2.u32*err_ec ;

		 SetMeterCfgACK(val2.u32,PARAQC);
//	  }

	    //��Чֵ��ȡ//
		if(val2.u32>0x80000000)
		{
	      val2.u32=~val2.u32+1;  // ȡ������
		}
		
	    if(val1.u32>0x80000000)
		{
	      val1.u32=~val1.u32+1;  // ȡ������
		}	
	    if(TempValue.u32>0x80000000)
		{
	      TempValue.u32=~TempValue.u32+1;  // ȡ������
		}		
		gs_measure_var_data.gs_really[PHASE_TT].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val1.u32;
		gs_measure_var_data.gs_really[WIRE_L1].dw_q_val.u32 = cst_adj_const.Kim_Prms2*TempValue.u32;
		gs_measure_var_data.gs_really[WIRE_L2].dw_q_val.u32 = cst_adj_const.Kim_Prms2*val2.u32;
		gs_measure_var_data.gs_really[WIRE_L3].dw_q_val.u32 = 0;

}


/*******************************************************************************************
** ��������: Lnk_get_EMU_activePower
** ��������: ��ȡ�޹�����
** �������: ch: ͨ�����
** �������: uint32 ָ��ͨ������ֵ��HEX��,����Ϊ0.1W
** ˵��    : 
*******************************************************************************************/
uint32 Lnk_get_EMU_ractivePower(uint8 ch)
{
   uint32 val,tmpval;
   fp32   f32_tmp;
   ST_U32_U08 TempValue;

    if(ch==WIRE_L1) 
    {
//       gs_emu_run_var.net_flg &= ~REV_L_RACPOWER;
	   val=ReadMeterParaACK(DATAIQ);	 
       if(val>=0x80000000)
       {
		val=~val+1; 
//		gs_emu_run_var.net_flg |= REV_L_RACPOWER;
       }
    }
	
    else  if(ch==WIRE_L2) 
    {
	if(ReadRaccoon(RegAAQ,1,BPhy)==TRUE)     // ��� 200ms��ȡ�ȴ�    //  ����ʱ��15ms
	{
       memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
       Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
       val = TempValue.u32;
	}
	else // ��ȡʧ�ܣ�ˢ�´���// 
	{
	    gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;
	}
//      gs_emu_run_var.net_flg &= ~REV_N_RACPOWER;

       if(val>=0x80000000)
       {
		val=~val+1; 
//		gs_emu_run_var.net_flg |= REV_N_RACPOWER;
       }

	   val = val*1.669;//2.175f/1.2f*1.086f;
    }
    else
    {
        val = 0;
    }

    if(val<REF_START_Pn)
    {
        tmpval =0;    
//        if(ch==WIRE_L1) gs_emu_run_var.net_flg &= ~REV_L_RACPOWER;
//        if(ch==WIRE_L2) gs_emu_run_var.net_flg &= ~REV_N_RACPOWER;
    }
    else
    {
        
        if(ch==WIRE_L1) f32_tmp = cst_adj_const.Kim_Prms2*val;
        if(ch==WIRE_L2) f32_tmp = cst_adj_const.Kim_Prms2*val;
        tmpval = (uint32)f32_tmp;  	        
    }
	
    return (tmpval);
}

/*******************************************************************************************
**    END
*******************************************************************************************/

