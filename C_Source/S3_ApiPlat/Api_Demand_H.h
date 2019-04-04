/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Demand_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: ��������Ӧ�ò���غ������������ļ�
**note			: MERTER FOR DL06A
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-20   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#ifndef _Api_Demand_H
#define _Api_Demand_H

/***************************************************************
*    1     ������Ԥ����   
****************************************************************/
//ÿ��ͨ������������󻺴�������//
#define MAX_MD_BUFF_NUM 30   //����������ڲ�����30����  //



/***************************************************************
*    2     ���ݽṹ����     
****************************************************************/
//������������ݵ�Ԫ�ṹ  //
typedef union
{
	uint8 buf[9];
	struct
	{
	  uint32 val_hex32;   // ��ǰ������������ݣ�32λ16�������ݣ�����0.0001kW//
	  uint8   date_rtc[5];   // ��ǰ�������������ʱ�䣬������ʱ��//
	}val;
}MMD_DATA_UNIT;

typedef union
{
	uint8 buf[9];
	struct
	{
		uint32 val_hex32;   // ��ǰ������������ݣ�32λ16�������ݣ�����0.0001kW//
		uint8   date_rtc[5];   // ��ǰ�������������ʱ�䣬������ʱ��//
	}val;
}NEW_MMD_DATA_UNIT;


typedef union
{
	uint8 buf[10];
	struct
	{
		uint32 val_hex32;   // ��ǰ������������ݣ�32λ16�������ݣ�����0.0001kW//
		uint8 val_count;//��¼�����������
		uint8   date_rtc[5];   // ��ǰ�������������ʱ�䣬������ʱ��//
	}val;
}SUM_MMD_DATA_UNIT;
//�ն���������������

typedef struct 
{
    SUM_MMD_DATA_UNIT   us_val[5];
//    INT16U  u16_csck;    //16λ�ۼӺ�У��ֵ//

}E2P_MDmax_SUM_DATA_ARRAY;
typedef struct 
{
    NEW_MMD_DATA_UNIT   us_val[5];
//    INT16U  u16_csck;    //16λ�ۼӺ�У��ֵ//

}E2P_MDmax_DATA_ARRAY;

//����ÿ��ͨ��ƽ������ʱʱ�������ݵı�������  //
typedef struct 
{
    uint8   cnt_sec;        //�ۼӺ�ʹ�õ����������С�ڵ���60�룬����1��  //
    uint32  sum_min;   //��ǰ�����ڵ��빦�������ۼӺͣ�32λ16�������ݣ�����0.0001kW   //
    uint32  val_min;   //ÿ����ƽ���������ݣ�32λ16�������ݣ�����0.0001kW  //
} MD_CH_MIN_VAR_UNIT;


//����ÿ��ͨ��ƽ������ʱʱ�������ݵı�������  //
typedef struct 
{
    uint32  val_slid;   //��ǰʱʱ����(ÿ���������һ��)��32λ16�������ݣ�����0.0001kW  //
    uint32  date_buff[MAX_MD_BUFF_NUM];   //����ƽ�������������飬32λ16�������ݣ�����0.0001kW  //
    uint8   cnt_min;        //�ۼӺ�ʹ�õķ��Ӽ�������С�����ڲ���������1����  //
    uint8   cal_flg;        //  ���������ʶ��=1��ʶ��ʼ������������    //
} MD_CH_SLID_VAR_UNIT;


//�����������  //
typedef struct 
{
    	uint8    u8_period;		//�������� //	
	uint8    u8_slid;			//��������  //	
} MD_PARAM;



//������������  //
typedef struct 
{
    MD_CH_MIN_VAR_UNIT   min_var[MD_CH_NUM];   
    MD_CH_SLID_VAR_UNIT  slid_var[MD_CH_NUM];
    uint32   power_val[MD_CH_NUM];
    uint8  min_remaind[2];     // ��һ��ʱʱ����ֵ����ʣ�������������  //
} MD_VAR;


//�����������  //
typedef struct 
{
    MMD_DATA_UNIT   us_val[MD_CH_NUM][1+TARIFF_MAX_NUM];
    INT16U  u16_csck;    //16λ�ۼӺ�У��ֵ//
} MD_USER_DATA;


//��ǰ�����������E2P�洢���ݽṹ  //
typedef struct 
{
    MMD_DATA_UNIT   us_val[MD_CH_NUM][1+TARIFF_MAX_NUM];
    INT16U  u16_csck;    //16λ�ۼӺ�У��ֵ//
} E2P_CURRENT_MD_DATA_MAP;

/***************************************************************
*    3    ��������       
****************************************************************/
extern MD_VAR gs_md_var;           // ��������������RAM������ //
extern MD_PARAM gs_md_param;  // ��������������RAM������ //

extern uint8 EOI_delay_1s;

/***************************************************************
*    4     ��������        
****************************************************************/
extern uint8 api_check_Demand_para(MD_PARAM *s_para);
extern void api_init_md_data_ram(void);
extern void api_cmp_save_md_data(uint8 ch, uint32 val_hex32);
extern void lnk_updata_md_var_power_per_second(void);
extern void api_updata_md_var_min_per_second(void);
extern void api_updata_md_var_slid_per_minute(void);
extern void api_clr_current_MD_data(void);



/***************************************************************
*    5     �������ݣ�����αָ���       
****************************************************************/
//������������ݵ�Ԫ���� //
#define LEN_MMD_UNIT			sizeof(MMD_DATA_UNIT)
#define LEN_NEW_MMD_UNIT		sizeof(NEW_MMD_DATA_UNIT)
#define LEN_SUM_MMD_UNIT		sizeof(SUM_MMD_DATA_UNIT)    
#define LEN_MMD_CURRENT		(sizeof( E2P_CURRENT_MD_DATA_MAP)-2)


//��ͨ������������ݳ���
#define LEN_MMD_NUM	     ((1+TARIFF_MAX_NUM)*LEN_MMD_UNIT)
/***************************************************************
*    END
****************************************************************/

#endif
