/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Proc_Data.c
**Author		: maji
**date			: 2016-04-20
**description	: ϵͳ������ݴ���C�ļ�
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#include <MyIncludes_H.h>



SYS_RUN_STATUS  gs_sys_run;
SYS_GLOBAL_VAR gs_sys_globaL_var;




// 0 �汾�� ���� ���� //
/*********************************************************************************************************
**  �汾�� ���� ����
********************************************************************************************************/

#if(meter_type == D_1P3W)
const SYS_PARM code sys_parm={
      0x00,METER_CONST_10WH_BCD,0x00,        //����й�����//
      'D','D','0','1','A','-','V','3','0','.','0','9',   //����汾��  //  �����޶��ڶ���
};
#endif
#if(meter_type == D_1P2W)
const SYS_PARM code sys_parm={
      0x00,METER_CONST_10WH_BCD,0x00,        //����й�����//
      'D','D','0','1','A','-','V','2','0','.','0','9',   //����汾��  //
};
#endif
/*********************************************************************************************************
**  ���ܱ����1 ��ʼ��Ĭ������
********************************************************************************************************/
const  E2P_METER_PARAM1_MAP   code  default_meter_parm1_tab = 
{
    30,30,       //  MD_PARAM                                md_param;
#if (BILL_MAX_NUM>0)
    0x26,0x00,        //BILL_MONTH_SETTLED_PARAM     bill_ms_param; 
    0x20, //Ϊ������
#endif
    0,           //INT8U   EMU_fast_flg;    = 0X//EMU �ӱ���ʶ //
    0X11,0X11,0X11,0X11,0X11,0X11,0XFF,0XFF,//INT8U   meter_id[16];
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0x11,0x11,0x11,0x11,0x11,0x11,				// Ĭ��RFͨѶ��ַ12��1,����6�ֽ� //	
    0,0,0,0,//INT8U   password[2][4];   //  2 ������ //
    0,0,0,0,
    0,0,0,0,//INT8U   RF_password[2][4];   // RF 2 ������ //
    0,0,0,0,
    METER_FACTORY_MARK,0,0,0,//INT8U   fac_cfg[4];          //  ������Ϣ������//
    0,28,   //INT8U   bat_level[2];          //  ��ص�ѹ��ֵ//  29=2.90v
    0,0, // �й���Ϸ�ʽ��//
    0,0, // �޹���Ϸ�ʽ��//
    0,	   //��ͨ�����ɼ�¼ָ��(1��ͨ��)  	2       BIN  XXXX    
    0,	    //��ͨ�����ɼ�¼����		   	2       BIN  XXXX    
    15,		   //��ͨ����¼ʱ������min��  	2       BCD  XXXX     
    1,1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,   // ���Ĵ�������(1+9)   			10      BCD  XXXX         ��һ��λ����   
    0x123456,
    0,	//INT8U	relay_cmd;			 //�̵����������� =0x00 ��բ =0x3A ��բ
    0,	//INT8U	relay_status;		 //�̵���״̬ =0x00��բ =0x3A  ��բ
    0,	//INT8U	const_fast_flg; 	 //���������ʶ =0x3C ��������10����������������
    0x65,0x34,0x12,// INT8U	EEPROM_Flag[3]; 	  // //  �״��ϵ�EEPROM���״̬�� !=0x653412��־��ǰΪ�״��ϵ� 
    0,          //INT16U  param1_csck;    //16λ�ۼӺ�У��ֵ//
};



void Proc_sys_data_init(void)
{
    gs_sys_run.back_fg = 0;
    gs_sys_run.font_fg = 0;
    gs_sys_globaL_var.delay_emu =3;   // EMU �¶�ʱ��ӳ�� //
    gs_sys_globaL_var.qf_time =0;     //�޹��������//
    mem_read(&gs_sys_globaL_var.meter_factory_status, ADR_METER_PARAM1_FC_CFG, 1, MEM_E2P1);   //����0=0X5A ������״̬ // 
    mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
    mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	//�µ���ʾά��ʱ��ָ�
	mem_read(&gs_sys_globaL_var.sleep_sec.B08[0], ADR_DIS_PARAM_SLEEP_SEC, 4, MEM_E2P1);    

}



/***************************************************************
*    END
****************************************************************/

