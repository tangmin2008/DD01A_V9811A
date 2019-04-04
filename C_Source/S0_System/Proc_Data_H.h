/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Prco_Data_H.H
**Author		: maji
**date			: 2016-04-26
**description	: ϵͳ������ݴ���C�ļ�
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _PROC_DATA_H
#define _PROC_DATA_H

/***************************************************************
*    1     ������Ԥ����   
****************************************************************/


/***************************************************************
*    2     ���ݽṹ����     
****************************************************************/
typedef struct 
{	
	INT8U	back_fg;                     // �ж��γɱ�ʶ //
	INT8U	font_fg;                     // ����������в�����ʶ //
} SYS_RUN_STATUS;


typedef struct 
{	
    INT8U  reset_status;                     // ϵͳ��λ��ʶ�� //
    INT8U  work_mode;             // ϵͳ��Դ��ʶ //
    INT8U  delay_emu;                    // EMU �ȶ�ʱ�䣬��ʼ��ֵ=2�� //
    INT8U  gs_check_err_status;   //У��״̬��  //
    INT8U  gs_hard_err_status;   //Ӳ��״̬��//
    INT8U  gs_net_status;             //����״̬��//
    INT8U  open_cover_status;     //�����ϸ�״̬��=0x5A���������ϸ�//
    INT8U  meter_factory_status;     //�����״̬��=0x5A�����ڹ���״̬//
    INT8U  fac_energy[2]; 			//�й�����ģʽ��
    INT8U  Rac_energy[2]; 			//�޹�����ģʽ��
    ST_U32_U08  sys_e2_err; 	    //����E2����//
    INT8U   sys_e2_err_flg; 		//����E2��������ʶ//
    ST_U32_U08 sleep_sec;   			//�µ���ʾά��ʱ��//
    INT8U  qf_time;     			//�޹������//
} SYS_GLOBAL_VAR;

typedef struct  
{
     INT8U      meter_cons[3];           //�����//
     INT8U      video[12];               //����汾��12���ֽ�ASCII��//
}SYS_PARM;





typedef struct 
{
    MD_PARAM                                md_param; // 2
   #if (BILL_MAX_NUM>0)
    BILL_MONTH_SETTLED_PARAM     bill_ms_param;  // 2
   #endif
    uint8   BILL_FLAG;//�����־,0x01Ϊ�ս���,0x30Ϊ�½���,����Ϊû�н���
    INT8U   EMU_fast_flg;  //EMU �ӱ���ʶ=0x3c Ϊ�ӱ�������Ϊ���� // 1
    INT8U   meter_id[16];                                //16
    INT8U   RF_comm_id[6];        //RFͨѶ��ַ
    INT8U   password[2][4];   //  2 ������ //   8
    INT8U   RF_password[2][4];   // RF 2 ������ //   8
    INT8U   fac_cfg[4];          //  ������Ϣ������//4
    INT8U   bat_level[2];          //  ��ص�ѹ��ֵ//2
    INT8U    fac_energy[2];  // �й����ģʽ��
    INT8U    Rac_energy[2];  // �޹����ģʽ��
   //�����������в���//
   INT16U   load_Ptr_var;
   INT16U   load_Num_var;
   INT16U   load_lptime_inv_var;     //��ͨ����¼ʱ������Min��  	2       BCD  XXXX     
   INT8U    load_egcode_var[10];  // ���Ĵ�������(1+9)   			10      BCD  XXXX         ��һ��λ���� 
   ST_U32_U08    sys_e2_err; 			 //���ڼƶ��������������� //
   INT8U   relay_cmd;			//�̵����������� =0x00 ��բ =0xA3 ��բ
   INT8U   relay_status;		//�̵���״̬ =0x00��բ =0xA3  ��բ
   INT8U   const_fast_flg;		//���������ʶ =0x3C ��������10����������������
   INT8U   EEPROM_Flag[3];       // //  �״��ϵ�EEPROM���״̬�� !=0x653412��־��ǰΪ�״��ϵ� 
   INT16U   param1_csck;    //16λ�ۼӺ�У��ֵ//
} E2P_METER_PARAM1_MAP;

typedef struct 
{
    RTC_TYPE  save_rtc;  //�µ籣���ʱ�� //
    INT16U  var_csck;    //16λ�ۼӺ�У��ֵ//
} E2P_METER_VAR_MAP;


/***************************************************************
*    3    ��������       
****************************************************************/
extern SYS_RUN_STATUS  gs_sys_run;
extern SYS_GLOBAL_VAR gs_sys_globaL_var;

//������ //
extern const  E2P_METER_PARAM1_MAP   code  default_meter_parm1_tab;
extern const SYS_PARM code sys_parm;

/***************************************************************
*    4     ��������        
****************************************************************/
extern void Proc_sys_data_init(void);

/***************************************************************
*    5     �������ݣ�����αָ���       
****************************************************************/
//  gs_sys_run.font_fg define 

//  gs_sys_globaL_var.open_cover_status define 
#define  OPENED_COVER_MARK      0x5A     // =0X5A �������ϸ��¼�����//

//  gs_sys_globaL_var.meter_factory_status define 
#define  METER_FACTORY_MARK      0x5A     // =0X5A �����ڹ���״̬//

//  gs_sys_run.back_fg define 
#define  BIT0_FONT_FG_10MS   BIT0   // 10ms �жϲ�����ʶ //
#define  BIT1_BACK_FG_RTC   BIT1   // RTC������1S�жϱ�ʶ //
#define  BIT2_BACK_DIS_KEY   BIT2   // ��ʾ�����������жϱ�ʶ //
#define  BIT3_FONT_FG_1MS   BIT3 

#define BIT3_FONT_FG_readpower    BIT3// ��ȡ�������ɱ�ʶ//
#define BIT4_FONT_FG_EnyBottom    BIT4// // ����ˢ��
/***************************************************************
*    END
****************************************************************/

#endif
