/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_User_Comm.c
**Author		: maji
**date			: 2016-04-21 
**description	: ͨѶ����Ӧ�ò���غ�������C�ļ�
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/



#include <MyIncludes_H.h>


/*******************************************************************************
* ȫ�ֱ���������
*******************************************************************************/
uint8 *P_IEC_BUFF;    //ͨѶ������ָ�� //
uint8 g_ch_comm;   //��ǰͨѶͨ���� //
UART_COMM_APP_VAR  gs_uart_iec_app_var[UART_CH_NUM];
COMM_DATA  comm_data;
uint8 clr_flag = 0;
uint8 IsClrEvent = 0;
/*******************************************************************************
* ��������code
*******************************************************************************/
const  INT8U Super_mima_iec[8] ={'4','5','6','1','2','3','0','0'};
const  INT8U meter_info[Meter_INFO_LENTH] = {'X','L','E','3','D','D','S','3','1','1','-','7','V','4','.','1',};	// ��ΪԶ���⣬��չΪ2400bps//
const  INT8U meter_info_IR[Meter_INFO_LENTH] = {'X','L','E','3','D','D','S','3','1','1','-','7','V','4','.','1',};	// ��ΪԶ���⣬��չΪ2400bps//
const  INT8U mima_info[MIMA_INFO_LENTH] = {'(','1','1','1','1','1','1','1','1',')',};

/*********************************************************************************************************
**  ͨѶID���
********************************************************************************************************/
 code IEC_COMDISTAB    IEC_ComDisTab[] =
{
	// �й����������//
	0x00010000, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF_T].buf[0],	EN_R+P_RAM, 4,8,	//��ǰ�ܵ���
#if (BILL_MAX_NUM>0)
	0x00010001, ADR_BLOCK101_BILL1_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL ,4,8,	//��1�µ���
	0x00010002, ADR_BLOCK102_BILL2_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//��2�µ���
	0x00010003, ADR_BLOCK103_BILL3_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//��3�µ���
	0x00010004, ADR_BLOCK104_BILL4_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//��4�µ���
	0x00010005, ADR_BLOCK105_BILL5_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//��5�µ���
	0x00010006, ADR_BLOCK106_BILL6_DATA_E2P+(OFFSET_bill_data_ec+TARIFF_T*LEN_EC_UNIT), EN_R+P_E2P1+CM_BILL,4,8,	//��6�µ���
#endif

#if (TARIFF_MAX_NUM>0)//�й��ķ���
	0x00010100, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF1].buf[0],	EN_R+P_RAM,	4,8,
	0x00010200, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF2].buf[0],	EN_R+P_RAM,	4,8,
	0x00010300, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF3].buf[0],	EN_R+P_RAM,	4,8,
	0x00010400, &gs_energy_user_data.us_val[EC_CH_AC_PT_TT][TARIFF4].buf[0],	EN_R+P_RAM,	4,8,
#endif	
	//�޹�
	0x00030000, &gs_energy_user_data.us_val[EC_CH_RE_PT_TT][TARIFF_T].buf[0],	EN_R+P_RAM,	4,8,  //��
#if (TARIFF_MAX_NUM>0)//�ķ���
	0x00030100, &gs_energy_user_data.us_val[EC_CH_RE_PT_TT][TARIFF1].buf[0],	EN_R+P_RAM,	4,8,
	0x00030200, &gs_energy_user_data.us_val[EC_CH_RE_PT_TT][TARIFF2].buf[0],	EN_R+P_RAM,	4,8,
	0x00030300, &gs_energy_user_data.us_val[EC_CH_RE_PT_TT][TARIFF3].buf[0],	EN_R+P_RAM,	4,8,
	0x00030400, &gs_energy_user_data.us_val[EC_CH_RE_PT_TT][TARIFF4].buf[0],	EN_R+P_RAM,	4,8,
#endif

	//���� ��
	0x01010000, ADR_BLOCK01_MMD_L0_E2P+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1,9,18,//��ǰ���������� 4+5 ����+ʱ��
	0x010100FF,	&sum_st_mmd_unit_array[0].buf[0],	EN_R+P_RAM,	5,10, //��ǰ�ۼ��������
#if (BILL_MAX_NUM>0)
	0x01010001, ADR_BLOCK101_BILL1_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��1��������
	0x01010002, ADR_BLOCK102_BILL2_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��2��������
	0x01010003, ADR_BLOCK103_BILL3_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��3��������
	0x01010004, ADR_BLOCK104_BILL4_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��4��������
	0x01010005, ADR_BLOCK105_BILL5_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��5��������
	0x01010006, ADR_BLOCK106_BILL6_DATA_E2P+OFFSET_bill_data_mmd+(TARIFF_T*LEN_MMD_UNIT), EN_R+P_E2P1+CM_BILL,9,18,//��6��������
#endif
	//��������	//
	0x02010000,&gs_measure_var_data.gs_really[0].dw_u_val.B08[0],	EN_R+P_RAM, 2,4,	//�����ѹ xxx.x		 YYY
	0x02010100,&gs_measure_var_data.gs_really[0].dw_u_val.B08[0],	EN_R+P_RAM, 2,4,	//A���ѹ xxx.x		 YYY
	0x02020000,&gs_measure_var_data.gs_really[0].dw_i_val.B08[0],	EN_R+P_RAM, 3,6,	//�������	 XXX.XXX
	0x02020100,&gs_measure_var_data.gs_really[0].dw_i_val.B08[0],	EN_R+P_RAM, 3,6,	//A�����   XXX.XXX
	0x02030000,&gs_measure_var_data.gs_really[0].dw_p_val.B08[0],	  EN_R+P_RAM,	3,6,	// TOTAL ACTIVE POWER	XX.XXXX 
	0x02030100,&gs_measure_var_data.gs_really[0].dw_p_val.B08[0],	  EN_R+P_RAM,	3,6,	// A�� ACTIVE POWER   XX.XXXX 
	0x02030200,&gs_measure_var_data.dw_p_n_val.B08[0],				 EN_R+P_RAM,	3,6,	// B�� ACTIVE POWER   XX.XXXX 
	0x02060000,&gs_measure_var_data.gs_really[0].dw_pf_val.B08[0],	 EN_R+P_RAM,2,4,	//���๦������  X.XXX
	0x02060100,&gs_measure_var_data.gs_really[0].dw_pf_val.B08[0],	 EN_R+P_RAM,2,4,	// A�๦������  X.XXX
	0x02800001,&gs_measure_var_data.dw_i_n_val.B08[0],		EN_R+P_RAM, 3,6,	//N�����   XXX.XXX
	0x02800002,&gs_measure_var_data.gs_really[0].w_freq_val.B08[0],    EN_R+P_RAM,2,4,	//Ƶ��  XX.XX
#if (TARIFF_MAX_NUM>1)
	0x02900001,&gs_current_triff_var.triff_No,	  EN_R+P_RAM,1,2,	//��ǰ���� 1~4//
	0x02900002,&gs_current_triff_var.dayTab_No,    EN_R+P_RAM,1,2,	//��ǰ��ʱ�α� 1~4//
	0x02900003,&gs_current_triff_var.weekTab_No,	EN_R+P_RAM,1,2, //��ǰ�ܱ� 1~4//
	0x02900004,&gs_current_triff_var.seasonTab_No,	  EN_R+P_RAM,1,2,	//��ǰ���ڱ� 1~4//
#endif

	//�����Ϣ///
	0x04000100,&gs_CurDateTime.Week,				EN_RW+P_RAM,7,14,	//ע�����ڣ�ͨѶ��д˳�� YYMMDDWWhhmmss
	0x04000103,ADR_METER_PARAM1_MD, 			   EN_RW+P_E2P1,	2,	4,	//�����������,����ʱ��//
	0x04000401,ADR_METER_PARAM1_METER_ID,		EN_RW+P_E2P1,16,32, //ͨѶ��ַ ���
	0x04000402,ADR_METER_PARAM1_RF_COMM_ID,		EN_RW+P_E2P1,6,12, //RFͨѶ��ַ yang 2017-12-21
	0x04000409,&sys_parm.meter_cons[0], 			EN_R+P_RAM,3,6, 	//����	 // 	
#if (TARIFF_MAX_NUM>1)//2019-01-07 �����޸ķ����������cxy
	0x04000801,ADR_BLOCK83_WEEK_TAB_E2P	,			EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//�ܱ���ʼ��ַ(4���ܱ�)	
	0x04000802,ADR_BLOCK83_WEEK_TAB_E2P+(1*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//�ܱ���ʼ��ַ(4���ܱ�)
	0x04000803,ADR_BLOCK83_WEEK_TAB_E2P+(2*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//�ܱ���ʼ��ַ(4���ܱ�)
	0x04000804,ADR_BLOCK83_WEEK_TAB_E2P+(3*LEN_WEEK_TABLE_ONE),		EN_RW+P_E2P1,LEN_WEEK_TABLE_ONE,(2*LEN_WEEK_TABLE_ONE),	//�ܱ���ʼ��ַ(4���ܱ�) 

	0x04010000,ADR_BLOCK82_SEASON_TAB_E2P,	             EN_RW+P_E2P1,LEN_SEASON_TABLE, (2*LEN_SEASON_TABLE),    // ���ڱ� 8*3  ����+�ܱ��
	0x04010001,ADR_BLOCK84_DAY_TAB_E2P,				EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//ʱ�α�1  ʱ�α���ʼ��ַ(8��ʱ�α�10��ʱ����)  288      BCD  hhnnNN          
	0x04010002,ADR_BLOCK84_DAY_TAB_E2P+(1*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//ʱ�α�2  ʱ�α���ʼ��ַ(8��ʱ�α�10��ʱ����)  288      BCD  hhnnNN          
	0x04010003,ADR_BLOCK84_DAY_TAB_E2P+(2*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//ʱ�α�3  ʱ�α���ʼ��ַ(8��ʱ�α�10��ʱ����)  288      BCD  hhnnNN          
	0x04010004,ADR_BLOCK84_DAY_TAB_E2P+(3*LEN_DAY_TABLE_ONE),		EN_RW+P_E2P1,LEN_DAY_TABLE_ONE, (2*LEN_DAY_TABLE_ONE),//ʱ�α�4  ʱ�α���ʼ��ַ(8��ʱ�α�10��ʱ����)  288      BCD  hhnnNN
#endif

	0x04000B01,ADR_METER_PARAM1_SETL_DDHH,			EN_RW+P_E2P1,2,4,	//������
	0x04000B02,ADR_BLOCK20_METER_PARAM1_E2P+ST_MB_OFFSET(E2P_METER_PARAM1_MAP,BILL_FLAG), EN_RW+P_E2P1,1,2,	 //���㷽ʽ
	0x04000C01,ADR_METER_PARAM1_PW0,					EN_W+P_E2P1,4,8,	//TTL 2������
	0x04000C02,ADR_METER_PARAM1_RF_PW0,					EN_W+P_E2P1,4,8,	//RF 2������
	0x04800003,ADR_METER_PARAM1_RELAY_CMD, 			   EN_RW+P_E2P1,	2,	4,//�̵������״̬��//
    0x04000A01,	 ADR_METER_PARAM1_load_lptime_inv_var,  	EN_RW+P_E2P1,2,4,//��¼���
    0x04000AA1,	 ADR_METER_PARAM1_load_egcode_var,            EN_RW+P_E2P1,10,20, //���ɱ����
    0x06010000,	 &LPRunPa.Num[0],				    EN_R+P_RAM,2,4, //�Ѽ�¼����
			//��ʾ�����//
	0x04000302,ADR_DIS_PARAM_AUTO_SEC,			EN_RW+P_E2P1,1,2,	 //�Զ�ѭ��ʱ�� 
	0x04000303,ADR_DIS_PARAM_EC_FORM,		 EN_RW+P_E2P1,1,2,	  // ������ʾλ��//
	0x04040100,ADR_DIS_PARAM_AUTO_TAB,		 EN_RW+P_E2P1,(DIS_MAX_ITEM+1),(2*(DIS_MAX_ITEM+1)), //�Զ�ѭ�Ա�//
	0x04800001,&sys_parm.video[0],			EN_R+P_RAM,12,12,  //����汾��(ASCII)


	// ������չ������//
	0x80805001, 0x0000, 					EN_R+P_E2P1,12,24,  // ��չ���� ����+�̵���״̬+����ʱ�� yang 2017-12-22// 
	0x08020100,0x0000,						EN_W+P_RAM,1,2, 		   //��������/�ֶ���λ//
	0x08020200,0x0000,						EN_W+P_RAM,1,2, 		   //=FF �¼���¼����  // �忪��
	0x08020300,0x0000,						EN_W+P_RAM,1,2, 		   //=00 ���ɼ�¼����  //
	0x08020400,0x0000,						EN_W+P_RAM,4,8, 		  //xxxxxxxx = 00000000ʱ�����ǰ����ʷ������������//
	0x08020500,ADR_METER_PARAM1_FC_CFG, 	EN_RW+P_E2P1,4,8,		  //��������״̬�֣�4�ֽ�  // 5A:����	//
	0x04000107,ADR_METER_PARAM1_BAT_LEVEL,	EN_RW+P_E2P1,	2,	4,	// ��ص�ѹ��ֵ  1 �ֽڣ�BCD  X.X 
	//������ʼ�� //
	0x0000FEFF,0x0000,							EN_W+P_RAM,2,4, 	// ��ʼ��
	0xEE000109,ADR_BLOCK70_EMU_PARAM_E2P,			EN_RW+P_E2P1+W_FCT,32,64,//!У�������(U,I1,I2,P1,P2,Fre)
	0xEE000118,ADR_METER_PARAM1_EEPROM_Flag,	EN_RW+P_E2P1+W_FCT,3,6,//EEPROM�״��ϵ��־λ �״��ϵ� �ñ�־λ������0x653412��������Ϊ�״��ϵ�///
	//�¼���¼ID ��բ//
	
	// һ���¼�  //
	//������λ   //
	0x03500000, ADR_BLOCK151_EVENI_resetMD_E2P,			    EN_R+P_E2P1, 2,4,	//�ܴ���
	0x03500101, ADR_BLOCK151_EVENI_resetMD_E2P+2,		    EN_R+P_E2P1, 6,12, //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03500102, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500103, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500104, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500105, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500106, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500107, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500108, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03500109, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0350010A, (ADR_BLOCK151_EVENI_resetMD_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0350FFFF, ADR_BLOCK151_EVENI_resetMD_E2P+2,			EN_R+P_E2P1, 60,120,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

	//������֤����
	0x03560000,  ADR_BLOCK153_EVENI_MMERR_E2P,		    EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03560101,  ADR_BLOCK153_EVENI_MMERR_E2P+2,		EN_R+P_E2P1, 6,12, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03560102,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560103,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560104,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560105,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560106,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560107,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560108,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 7*6),	EN_R+P_E2P1 , 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03560109,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 8*6),	EN_R+P_E2P1 , 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0356010A,  (ADR_BLOCK153_EVENI_MMERR_E2P+2+ 9*6),	EN_R+P_E2P1 , 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0356FFFF,  ADR_BLOCK153_EVENI_MMERR_E2P+2,		EN_R+P_E2P1 , 60,120, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

	//ʱ��У׼
	0x03570000,  ADR_BLOCK152_EVENI_setRtc_E2P,			    EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03570101,  ADR_BLOCK152_EVENI_setRtc_E2P+2,	   	    EN_R+P_E2P1, 6,12, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03570102,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570103,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570104,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570105,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570106,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570107,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570108,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03570109,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0357010A,  (ADR_BLOCK152_EVENI_setRtc_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0357FFFF,  ADR_BLOCK152_EVENI_setRtc_E2P+2,		    	EN_R+P_E2P1, 60,120, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��

	//E2P����
	0x03580000,  ADR_BLOCK156_EVENII_EERR_E2P,		    EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03580101,  ADR_BLOCK156_EVENII_EERR_E2P+2,	 	EN_R+P_E2P1, 6,12, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03580102,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 1*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580103,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 2*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580104,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 3*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580105,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 4*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580106,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 5*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580107,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 6*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580108,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 7*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03580109,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 8*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0358010A,  (ADR_BLOCK156_EVENII_EERR_E2P+2+ 9*6),	EN_R+P_E2P1, 6,12, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0358FFFF,   ADR_BLOCK156_EVENII_EERR_E2P+2,		    	EN_R+P_E2P1, 60,120, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��

//  �����¼�	//
	//���Ƿѹ ���⴦��  ֻ��������ʱ��//	
	0x03590000,   ADR_BLOCK162_EVENII_lowbattery_E2P,            EN_R+P_E2P1, 2,4,	//�ܴ���
	0x03590001,   ADR_BLOCK162_EVENII_lowbattery_E2P+2,          EN_R+P_E2P1, 4,8,	//�ۼ�ʱ��
	0x03590101,   ADR_BLOCK162_EVENII_lowbattery_E2P+6,		     EN_R+P_E2P1+CM_EBIL, 12,24,	 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03590102,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590103,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590104,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590105,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 4*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590106,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 5*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590107,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 6*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590108,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 7*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03590109,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 8*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0359010A,  (ADR_BLOCK162_EVENII_lowbattery_E2P+6+ 9*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,//������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0359FFFF,  ADR_BLOCK162_EVENII_lowbattery_E2P+6,	         EN_R+P_E2P1, 60,120,//��ʼ��¼  6*10��
	
	// ��Դ����  //	
	0x03380000,  ADR_BLOCK161_EVENII_PWN_E2P,                      EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03380001,  ADR_BLOCK161_EVENII_PWN_E2P+2,                  EN_R+P_E2P1, 	4,8,	//�ۼ�ʱ��
	0x03380101,  ADR_BLOCK161_EVENII_PWN_E2P+6,		        EN_R+P_E2P1+CM_EBIL, 12,24, 		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03380102,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380103,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380104,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380105,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 4*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380106,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 5*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380107,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 6*6),	 EN_R+P_E2P1+CM_EBIL, 12,24, //������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380108,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 7*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03380109,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 8*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0338010A,  (ADR_BLOCK161_EVENII_PWN_E2P+6+ 9*6),	  EN_R+P_E2P1+CM_EBIL, 12,24, ///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0338FFFF,   ADR_BLOCK161_EVENII_PWN_E2P+6,		       EN_R+P_E2P1+CM_GETHER_EBIL, 120,240,		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��

	//COVER_//	
	0x03450000,  ADR_BLOCK163_EVENII_COVER_E2P,           EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03450001,  ADR_BLOCK163_EVENII_COVER_E2P+2,         EN_R+P_E2P1, 	4,8,	//�ܴ���
	0x03450101,  ADR_BLOCK163_EVENII_COVER_E2P+6,		      EN_R+P_E2P1+CM_EBIL, 12,24,		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	0x03450102,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 1*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450103,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 2*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450104,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 3*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450105,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 4*6),  EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450106,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 5*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450107,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 6*6),  EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450108,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 7*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x03450109,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 8*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
	0x0345010A,  (ADR_BLOCK163_EVENII_COVER_E2P+6+ 9*6),	 EN_R+P_E2P1+CM_EBIL, 12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0341FFFF,  ADR_BLOCK163_EVENII_NOL1_E2P+6,		       EN_R+P_E2P1+CM_GETHER_EBIL, 120,240,		 //���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	//L2 ���� //
//	0x03420000,  ADR_BLOCK164_EVENII_NOL2_E2P,                      EN_R+P_E2P1, 	2,4,	//�ܴ���
//	0x03420001, ( ADR_BLOCK164_EVENII_NOL2_E2P+2),               EN_R+P_E2P1, 	4,8,	//���ۼ�ʱ��//
////	0x03420101,  ADR_BLOCK164_EVENII_NOL2_E2P+6,		         EN_R+P_E2P1+CM_EBIL, 	12,24,//���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
//	0x03420102,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 1*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420103,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 2*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420104,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 3*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420105,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 4*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420106,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 5*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420107,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 6*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420108,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 7*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x03420109,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 8*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��
//	0x0342010A,  (ADR_BLOCK164_EVENII_NOL2_E2P+6+ 9*6),	  EN_R+P_E2P1+CM_EBIL, 	12,24,///������ε�����¼  �¼�����  ��ʼʱ�� ����ʱ��

//	0x0342FFFF,  ADR_BLOCK164_EVENII_NOL2_E2P+6,		   EN_R+P_E2P1+CM_GETHER_EBIL, 	120,240,//���һ�ε�����¼  �¼����� ��ʼʱ�� ����ʱ��
	// ���ʷ����¼�  //	
	0x03360000,  ADR_BLOCK164_EVENII_L1reverse_E2P,                       EN_R+P_E2P1, 	2,4,	//�ܴ���
	0x03360001, ( ADR_BLOCK164_EVENII_L1reverse_E2P+2),                EN_R+P_E2P1, 	4,8,	//���ۼ�ʱ��//
	0x03360101,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6),                EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360102, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+1*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360103,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+2*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360104, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+3*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360105, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+4*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360106, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+5*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360107,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+6*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360108, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+7*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x03360109, ( ADR_BLOCK164_EVENII_L1reverse_E2P+6+8*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//
	0x0336010A,  (ADR_BLOCK164_EVENII_L1reverse_E2P+6+9*6),         EN_R+P_E2P1+CM_EBIL, 	12,24,	//��ʼʱ��+����ʱ��//

//	0x0336FFFF,  (ADR_BLOCK165_EVENII_L1reverse_E2P+6),                EN_R+P_E2P1+CM_GETHER_EBIL, 	120,240,	//��ʼʱ��+����ʱ��//

	// ��һ�ν���ʱ��
	0x03700101 ,   ADR_BLOCK101_BILL1_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12,  // ������ʱ����
	0x03700102 ,   ADR_BLOCK102_BILL2_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
	0x03700103 ,   ADR_BLOCK103_BILL3_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
	0x03700104 ,   ADR_BLOCK104_BILL4_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
	0x03700105 ,   ADR_BLOCK105_BILL5_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
	0x03700106 ,   ADR_BLOCK106_BILL6_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ���� 
//	0x03700107 ,   ADR_BLOCK107_BILL7_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
//	0x03700108 ,   ADR_BLOCK108_BILL8_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
//	0x03700109 ,   ADR_BLOCK109_BILL9_DATA_E2P ,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
//	0x0370010A ,   ADR_BLOCK110_BILL10_DATA_E2P ,     EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����
//	0x0370010B ,   ADR_BLOCK111_BILL11_DATA_E2P,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ���� 
//	0x0370010C ,   ADR_BLOCK112_BILL12_DATA_E2P,      EN_R+P_E2P1+CM_BILL, 6,12, // ������ʱ����

//�ն���
	0x0506FF01,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF02,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF03,    0x0000,	  EN_R+P_E2P1, 18,36,
	0x0506FF04,    0x0000,	  EN_R+P_E2P1, 18,36,

	//ͨѶ����У�� 6 //
	0x0000FEBB,0x0000,						EN_W+P_E2P1+W_FCT,2,4,//����EMU ��Ƶ //
	0x80800803, 0x0000, 					EN_W+P_E2P1+W_FCT,2,4,  // EMU����������ʼ�� ����Ĭ�ϲ�����ֵ // 
	0x0000FEB0, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //����· 1.0L IB ������ЧֵУ׼// 
	0x0000FEB1, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //����· 0.5L IB ���У׼// 
	0x0000FEB2, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //�λ�·1.0L IB ���У׼// 
	0x0000FEB3, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,   //�λ�·0.5L IB ���У׼// 
	0x0000FEB4, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,  //�λ�·0.5L IB ���У׼// 
	0x0000FEB5, 0x0000, 					EN_W+P_E2P1+W_FCT,12,24,  //�λ�·1.0L 5%IB ���У׼// 
};
//---------------------------------------------------------------------//
//����:   ��������ʼ��ַ��ʼ��F��ֹ�ĳ���//
//     0000F     return 4
//	*Addr ������ʼ��ַ��num���ݳ���
//---------------------------------------------------------------------//
INT8U get_meter_id_asclen(INT8U *Addr,INT8U numbyte)
{
	INT8U i;
	INT8U asclen=0;
	for(i=0;i<numbyte;i++)
	{
		if((((*(Addr+i))>>4)&0x0F)==0x0F)
		{
			return asclen;
		}
		asclen++;
		if(((*(Addr+i))&0x0F)==0x0F)
		{
			return asclen;
		}
		asclen++;
	}
	return asclen;
}


/*****************************************************************
 name:          INT8U  get_comm_var( INT16U id )
 function:      get command information precess  
 input:         no            
 return:        GetComVar()
                =0    succeed  precess 
                =1    fail precess 
*****************************************************************/
INT8U  get_IEC_comm_var( INT32U id )
{
	INT16U i;  
    //���ڱ�������� ��������//
    if((id>=0x06010001)&&(id<=0x060105A0))//������������¼900��   //SXL 2019-01-6
    {
		comm_data.addr =0;
		comm_data.Info.u16 = EN_R;		//���ܻ���
		comm_data.len = 0;
		comm_data.len_asc = 0;
		return TRUE;
    }
		 if((id>=0x060A0001)&&(id<=0x060A0120))//�������ݿ�����¼90��   //SXL 2019-01-6
    {
		comm_data.addr =0;
		comm_data.Info.u16 = EN_R;		//���ܻ���
		comm_data.len = 0;
		comm_data.len_asc = 0;
		return TRUE;
    }
   for( i = 0 ; i <dim(IEC_ComDisTab) ; i++ )
	{
	    if( id == IEC_ComDisTab[i].ComID )
	    {
	        comm_data.addr = IEC_ComDisTab[i].Addr;
	        comm_data.Info.u16 = IEC_ComDisTab[i].ComInfo;
	        comm_data.len = IEC_ComDisTab[i].Len;
	        comm_data.len_asc = IEC_ComDisTab[i].Len_Asc;
	        return TRUE;
	    }
	}
	return FALSE;	

	

/*   INT16U i;  

    //���ڱ��������//
    if((id>=0x06010001)&&(id<=0x060116A9))
    {
	comm_data.addr =0;
	comm_data.Info.u16 = EN_R;		//���ܻ���
	comm_data.len = 0;
	comm_data.len_asc = 0;
	return 1;
    }

    if((id>=0x06012001)&&(id<=0x06012245))
    {
	comm_data.addr =0;
	comm_data.Info.u16 = EN_R;		//���ܻ���
	comm_data.len = 0;
	comm_data.len_asc = 0;
	return 1;
    }

    //��������ݷ�������//
    
	if(id<0x01000000) //��������
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab) ; i++ )
	    {
	        if( id == IEC_ComDisTab[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab[i].ComInfo;
	            comm_data.len = IEC_ComDisTab[i].Len;
	            comm_data.len_asc = IEC_ComDisTab[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x02000000)//������
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab1) ; i++ )
	    {
	        if( id == IEC_ComDisTab1[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab1[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab1[i].ComInfo;
	            comm_data.len = IEC_ComDisTab1[i].Len;
	            comm_data.len_asc = IEC_ComDisTab1[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x03000000)//ϵͳ������
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab2) ; i++ )
	    {
	        if( id == IEC_ComDisTab2[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab2[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab2[i].ComInfo;
	            comm_data.len = IEC_ComDisTab2[i].Len;
	            comm_data.len_asc = IEC_ComDisTab2[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x04000000)//�¼���¼��
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab3) ; i++ )
	    {
	        if( id == IEC_ComDisTab3[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab3[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab3[i].ComInfo;
	            comm_data.len = IEC_ComDisTab3[i].Len;
	            comm_data.len_asc = IEC_ComDisTab3[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x05000000)//ϵͳ������
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab4) ; i++ )
	    {
	        if( id == IEC_ComDisTab4[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab4[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab4[i].ComInfo;
	            comm_data.len = IEC_ComDisTab4[i].Len;
	            comm_data.len_asc = IEC_ComDisTab4[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}
	else if(id<0x09000000)//ϵͳ������
	{
	    for( i = 0 ; i <dim(IEC_ComDisTab5) ; i++ )
	    {
	        if( id == IEC_ComDisTab5[i].ComID )
	        {
	            comm_data.addr = IEC_ComDisTab5[i].Addr;
	            comm_data.Info.u16 = IEC_ComDisTab5[i].ComInfo;
	            comm_data.len = IEC_ComDisTab5[i].Len;
	            comm_data.len_asc = IEC_ComDisTab5[i].Len_Asc;
	            return TRUE;
	        }
	    }
	}

    return FALSE;*/
}

/*****************************************************************
 name:          uchar  GetComVar( uchar* Addr, uint* Info )
 function:      get command information precess  
 input:         no            
 return:        GetComVar()
                =0    succeed  precess 
                =1    fail precess 
*****************************************************************/
INT8U SecurityCK(void)
{
	INT8U secType;
	secType = comm_data.Info.B08[0]&0x07; //Ȩ�޼��//
	/*
	if(secType ==(W_FCT>>8))
	{
		if(ADJA_KEY)	// 0 �̽� 1 δ�̽�
		{
			return (mmover);
		}
	}
    */
	return (mmok);
}

//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Password_Deal(INT8U* sptr,INT8U len) 
//  Description:   iec ͨѶ����ָ������
//  Parameters:    
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Password_Deal(INT8U* sptr)
{
  INT8U tmp_8u_01[4];
  INT8U tmp_8u_02[8];

    if((gs_uart_iec_app_var[g_ch_comm].iec_status & (COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC)) != 0x03)   return(FALSE);

     mem_read(&tmp_8u_01[0], ADR_METER_PARAM1_PW0, 4, MEM_E2P1);
     Lib_BCD_AscA(&tmp_8u_02[0], &tmp_8u_01[0], 4);
	 

    if(Lib_Cmp_TwoArry((sptr+5), &tmp_8u_02[0], 8) == 0x00)  
    {
         gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_MIMA_OK_IEC;  // ������������ɹ���ʶ //
         Get_ACK_Answer(sptr);
         gs_uart_iec_app_var[g_ch_comm].len = 1;
         return TRUE;
    }
    else
    {
        if(Lib_Cmp_TwoArry((sptr+5), &Super_mima_iec[0], 8) == 0x00) 
        {
            gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_MIMA_OK_IEC;  // ������������ɹ���ʶ //
            Get_ACK_Answer(sptr);
            gs_uart_iec_app_var[g_ch_comm].len = 1;
            return TRUE;
        }
    }
    api_deal_even_class_recode(ID_CLASS_I_MMERR,START); //  
    return FALSE;
}


//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Read_Deal(INT8U* sptr,INT8U len)
//  Description:   iec ͨѶ��ָ������
//  Parameters:    
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Read_Deal(INT8U* sptr)
{
 INT8U i,j;
 INT8U  mem_type;
 INT8U  data_buff[128];
 INT8U  *ptr_adr;
 INT16U ctmp1;
 ST_U32_U08  long_tmp;
 ST_U16_U08	 LPRecStart,LPRecEnd;

SUM_MMD_DATA_UNIT t;
NEW_MMD_DATA_UNIT te;
    if((gs_uart_iec_app_var[g_ch_comm].iec_status &(COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC+COMM_MIMA_OK_IEC)) != 0x07)   return(FALSE);	
    //get comm DI1 DI0
     Lib_Asc_BCDA(&comm_data.di1_di0.B08[0], sptr+4,8);

    //get other comm fram data 	
    if(get_IEC_comm_var( comm_data.di1_di0.u32) == 0 )   return(FALSE);
    if( ( comm_data.Info.u16& EN_R) == 0 )  return(FALSE);
    //  ��ȡ���ݵĴ洢���� //
    mem_type = (comm_data.Info.B08[0]>>3)&0x07;   	
    //���������㲢���÷��ͳ���
    Lib_Set_String((sptr+2), '0', comm_data.len_asc);  

    ptr_adr = comm_data.addr;

      //    // 
	//���ڱ����ID���� ��������ID 
	if((comm_data.di1_di0.u32>=0x06010001)&&(comm_data.di1_di0.u32<=0x060105A0))
	{
		ctmp1 = (INT16U)(comm_data.di1_di0.u32 - 0x06010001);
		if(ctmp1 < LPRunPa.Num[0])	// ��ȡID С�����Ѽ�¼��
		{
			comm_data.len = LPRec_Read_1(sptr+2, ctmp1);
			comm_data.len_asc = 2*comm_data.len;	
		}
		else
		{
//			comm_data.len = LPRec_Read_1(sptr+2, ctmp1);
//			comm_data.len_asc = 2*comm_data.len;	
//			Lib_Set_String((sptr+2), 'F', comm_data.len_asc); 
			comm_data.len_asc = 0;	
		}


		// �ɹ���������֡ //
		//api_deal_even_class_recode(ID_CLASS_I_COMM,START);
		gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
		*(sptr+0) = STX;
		*(sptr+1) = '(';
		CLRWDT(); *(sptr+2+comm_data.len_asc) = ')';
		*(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
	CLRWDT(); 	*(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);

		return(TRUE); 
	}
	
	
	if((comm_data.di1_di0.u32>=0x060A0001)&&(comm_data.di1_di0.u32<=0x060A0120))//   //SXL 2019-01-6
  {
		LPRecStart.u16 =(INT16U)((comm_data.di1_di0.u32 - 0x060A0001)*BLOCK_MAX_LEN);
		LPRecEnd.u16 =LPRecStart.u16+BLOCK_MAX_LEN;
		
		if(LPReadPara.ReadLoraFlag == FALSE )
		{			
			LPReadPara.Original_FrameNum = LPRunPa.Num[0] ;
			LPReadPara.Original_Ptr = LPRunPa.Ptr[0] ;
		} 		
		
		comm_data.len_asc  = LPRec_Read_Block(sptr+2,LPRecStart.u16, LPRecEnd.u16);
	  	 
		
		if(LPReadPara.ReadLoraFlag == FALSE )	LPReadPara.ReadLoraFlag = TRUE; 
			
		gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
		CLRWDT(); 
		*(sptr+0) = STX;
		*(sptr+1) = '(';CLRWDT(); 
		*(sptr+2+comm_data.len_asc) = ')';
		CLRWDT(); *(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
		*(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);
CLRWDT(); 
		return(TRUE); 
		
		}
	
    // ���ݶ�ȡ���ݵ�ID��Ŀ����֧���� //
    switch(comm_data.di1_di0.u32)
    {	
		case 0x02010000:    //��ѹ Դ����ΪHEX32��0.1V���٣���Ҫת��ΪBCD���ʽ,XXX.XV //
		case 0x02010100:    //��ѹ Դ����ΪHEX32��0.1V���٣���Ҫת��ΪBCD���ʽ,XXX.XV //
			Lib_long_bcd4(&long_tmp.B08[0],gs_measure_var_data.gs_really[0].dw_u_val.u32);
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[2], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02020000:    //L���� Դ����ΪHEX32��0.001A���٣���Ҫת��ΪBCD���ʽ,XXX.XXXA //
		case 0x02020100:    //L���� Դ����ΪHEX32��0.001A���٣���Ҫת��ΪBCD���ʽ,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_i_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02800001:    //N���� Դ����ΪHEX32��0.001A���٣���Ҫת��ΪBCD���ʽ,XXX.XXXA //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_i_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02030000:    //L   ����Դ����ΪHEX32��0.0001KW���٣���Ҫת��ΪBCD���ʽ,XX.XXXXkW //
		case 0x02030100:	//����Դ����ΪHEX32��0.0001KW���٣���Ҫת��ΪBCD���ʽ,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_p_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;
		
		case 0x02030200:    //N����Դ����ΪHEX32��0.0001KW���٣���Ҫת��ΪBCD���ʽ,XX.XXXXkW //
			Lib_long_bcd4(&long_tmp.B08[0],(gs_measure_var_data.dw_p_n_val.u32));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[1], 3);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x02060000:	//������������ΪHEX16��0.001����Ҫת��ΪBCD���ʽ0.001 //
		case 0x02060100:    //������������ΪHEX16��0.001����Ҫת��ΪBCD���ʽ0.001 //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].dw_pf_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x02800002:    //����Ƶ������ΪHEX16��0.01Hz����Ҫת��ΪBCD���ʽxx.xx //
			Lib_word_bcd2(&long_tmp.B08[0],(gs_measure_var_data.gs_really[0].w_freq_val.u16));
			Lib_Copy_Str_TwoArry(&data_buff[0], &long_tmp.B08[0], 2);
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x04000100://���ڼ�����(����0����������)YYMMDDWWhhmmss
			Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
			data_buff[7] = data_buff[0];	//ww
			data_buff[0] = data_buff[1];	//YY
			data_buff[1] = data_buff[2];	//MM
			data_buff[2] = data_buff[3];	//DD
			data_buff[3] = data_buff[7];	//WW
			data_buff[4] = data_buff[4];	//hh
			data_buff[5] = data_buff[5];	//mm
			data_buff[6] = data_buff[6];	//ss
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
		break;

		case 0x04000401://���ַ ��� NNNNNNNNNNNN
			mem_read(&data_buff[0], (INT16U)comm_data.addr,  comm_data.len,  mem_type );
			comm_data.len_asc = get_meter_id_asclen(&data_buff[0], 16);
			comm_data.len = comm_data.len_asc>>1;
			Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len);
		break;
		
		case 0x04800001: //����汾�� ����ASC��
			Lib_Copy_Str_TwoArry(sptr+2, ptr_adr, comm_data.len);
		break;		
		
		case 0x80805001:	//��ǰ���ܵ���+�̵���״̬+�������ʱ�� //
			//��ǰ���ܵ���
			mem_read(&data_buff[10], (INT16U)&gs_energy_user_data.us_val[0][0].buf[0], LEN_EC_UNIT, MEM_RAM);	  //��ȡ��ǰ���� //
			api_get_energy_LCD(&data_buff[10], 0x62,&data_buff[0]);
			Lib_BCD_AscA(sptr+2, &data_buff[0], 4); 
			//�̵���״̬
			mem_read(&data_buff[0], (INT16U)ADR_METER_PARAM1_RELAY_CMD, 2, MEM_E2P1);	  //��ȡ��ǰ���� //
			Lib_BCD_AscA(sptr+2+8, &data_buff[0], 2); 
			Lib_Copy_Str_TwoArry(&data_buff[0], &gs_CurDateTime.Year, 6);
			Lib_BCD_AscA(sptr+2+8+4, &data_buff[0], 6); 
		break;

		case 0x0506FF01: //����1
		case 0x0506FF02: //����2
		case 0x0506FF03: //����3
		case 0x0506FF04: //����4
			  i = comm_data.di1_di0.B08[3];
			  mem_read(&data_buff[10], &energy_data_array[i].buf[0], LEN_BLOCK_4Energy_E2P/4,MEM_RAM);//����
			  api_get_energy_LCD(&data_buff[10], 0x62,&data_buff[0]);
			  Lib_BCD_AscA(sptr+2+10, &data_buff[0], 4);	
				
			  mem_read(&te, &st_mmd_unit_array[i].buf[0], LEN_NEW_MMD_UNIT, MEM_RAM);//����
			  te.val.val_hex32 /=100;
			  data_buff[0] = te.val.val_hex32/1000000;
			  data_buff[1] = te.val.val_hex32%1000000/10000;
			  data_buff[2] = te.val.val_hex32%10000/100 ;
			  data_buff[3] = te.val.val_hex32%100  ;
			  for(j=0;j<4;j++)
			  {
			  	if(data_buff[j]>0x09)
				{
					data_buff[j]+= 0x06*(data_buff[j]/10);
				}
			  }
			  Lib_BCD_AscA(sptr+2+10+8,&data_buff[0],4);
			  //Lib_BCD_AscA(sptr+2+10+8,&te.buf[0],4);
			 
			  mem_read(&t, &sum_st_mmd_unit_array[i].buf[0], LEN_SUM_MMD_UNIT, MEM_RAM);//�ۼ�����
			  t.val.val_hex32 /=100;
			  data_buff[0] = t.val.val_hex32/1000000;
			  data_buff[1] = t.val.val_hex32%1000000/10000;
			  data_buff[2] = t.val.val_hex32%10000/100; 
			  data_buff[3] = t.val.val_hex32%100;
			  for(j=0;j<4;j++)
			  {
			  	if(data_buff[j]>0x09)
				{
					data_buff[j]+=0x06*(data_buff[j]/10);
				}
			  }
			  Lib_BCD_AscA(sptr+2+10+8+8,&data_buff[0],4);
			 // Lib_BCD_AscA(sptr+2+10+8+8,&t.buf[0],4);
			  data_buff[0] = te.val.date_rtc[0];	//YY
			  data_buff[1] = te.val.date_rtc[1];	//MM
			  data_buff[2] = te.val.date_rtc[2];	//DD
			  data_buff[3] = te.val.date_rtc[3];	//hh
			  data_buff[4] = te.val.date_rtc[4];	//mm
			  Lib_BCD_AscA(sptr+2, &data_buff[0], 5);
			  if(t.buf[4]>0x09)
				{
					t.buf[4]+=0x06*(t.buf[4]/10);
				}
			  Lib_BCD_AscA(sptr+2+10+8+8+8,&t.buf[4],1);//����
		break;
		
        default:
			////////////////���������⴦��/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x00)
			{
				comm_data.len = LEN_EC_UNIT;   // �޸Ļ�ȡ����//
			}
			///////////////////////////////////////////////////////////////////

			if((comm_data.Info.u16&CM_ALL) == CM_BILL)
			{
				mem_read(&data_buff[0],api_get_bill_record_addr((INT16U)comm_data.addr), comm_data.len,  mem_type );
			}
			else if((comm_data.Info.u16&CM_ALL) == CM_EBIL)//�¼���¼������
	  	{
        mem_read(&data_buff[0], comm_data.addr,     (comm_data.len>>1),  mem_type );
        mem_read(&data_buff[6], comm_data.addr+60,  (comm_data.len>>1),  mem_type );
	    }
			
			else 
			{
				if(mem_type==MEM_RAM)
				{
					Lib_Copy_Str_TwoArry(&data_buff[0], ptr_adr, comm_data.len);
				}
				else
				{
					mem_read(&data_buff[0],(INT16U)comm_data.addr,comm_data.len,mem_type );
				}
			}
			////////////////���������⴦��/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x00)
			{
				Lib_Copy_Str_TwoArry(&data_buff[10], &data_buff[0], LEN_EC_UNIT);
				api_get_energy_LCD(&data_buff[10], 0x62, &data_buff[0]) ;
				comm_data.len=4;
			}
			///////////////////////////////////////////////////////////////////

			////////////////����������⴦��/////////////////////////////
			if(comm_data.di1_di0.B08[0]==0x01)
			{  
				Lib_Copy_Str_TwoArry(&long_tmp.B08[0], &data_buff[0], 4);
				Lib_long_bcd4(&data_buff[0],long_tmp.u32);
			}
			
			
			
			
			
			/////////////////////////////////////////////////////////////////// 		   
			if(comm_data.len_asc == 5)
			{
				*(sptr+2) = Lib_BtoA(data_buff[0]&0x0f);
				Lib_BCD_AscA(sptr+3, &data_buff[1], 2);   
			}
			else
			{
				Lib_BCD_AscA(sptr+2, &data_buff[0], comm_data.len); 
			}
            break;	
    }

    // �ɹ���������֡ //
    gs_uart_iec_app_var[g_ch_comm].len= (comm_data.len_asc+5);
    *(sptr+0) = STX;
    *(sptr+1) = '(';
    *(sptr+2+comm_data.len_asc) = ')';
    *(sptr+gs_uart_iec_app_var[g_ch_comm].len-2) =ETX;
    *(sptr+gs_uart_iec_app_var[g_ch_comm].len-1) = Get_Bcc_Chk(sptr, 1, gs_uart_iec_app_var[g_ch_comm].len-2);
	
    return(TRUE); 
}


//-------------------------------------------------------------------------------//
//  Function:      INT8U IEC_Cmd_Write_Deal(INT8U* sptr,INT8U len)
//  Description:   iec ͨѶдָ������
//  Parameters:   lenΪ�������ݵ�ʵ�ʳ���
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Write_Deal(INT8U* sptr,INT8U len)
{
// INT8U tmp_8u_01[16];
 INT8U  mem_type;
 INT8U  data_buff[70];
 INT8U	sectemp;
 INT8U i,MeterID_LenTmp; //���  ��Ч���� asc
 INT8U  *ptr_adr;
 
    if((gs_uart_iec_app_var[g_ch_comm].iec_status &(COMM_REQUIST_OK_IEC+COMM_INFO_OK_IEC+COMM_MIMA_OK_IEC)) != 0x07)   return(FALSE);
    //get comm DI1 DI0
    Lib_Asc_BCDA(&comm_data.di1_di0.B08[0], sptr+4,8);
    //get other comm fram data 
    if(get_IEC_comm_var( comm_data.di1_di0.u32) == 0 )   return(FALSE);

    if( ( comm_data.Info.u16& EN_W) == 0 )  return(FALSE);
    //  ��ȡ���ݵĴ洢���� //
    mem_type = (comm_data.Info.B08[0]>>3)&0x07;   	
    sectemp = SecurityCK();					//Ȩ���ж�
    if(sectemp == mmover)   return(FALSE);

    //���������㲢�����յ����ݳ��ȺϷ��� //
    Lib_Set_String(&data_buff[0], 0x00, comm_data.len); 
    if(comm_data.di1_di0.u32== 0x04000401) //�����ñ����.����Ҫ�Ա����ݳ���
    {
        if((len > comm_data.len_asc+16)||(len < 16))
        return(FALSE);
    }
    else
    {
        if(len != (comm_data.len_asc+16))	   
        return(FALSE);
    }

    ptr_adr = comm_data.addr;

    // �����������ݵ�ID��Ŀ����֧���� //
    switch(comm_data.di1_di0.u32)
    {

        case 0x04000100://���ڼ�����(����0����������)YYMMDDWWhhmmss  
            Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
            gs_CurDateTime.Year= data_buff[0];
            gs_CurDateTime.Month= data_buff[1];
            gs_CurDateTime.Day= data_buff[2];
            gs_CurDateTime.Week= data_buff[3];
            gs_CurDateTime.Hour= data_buff[4];
            gs_CurDateTime.Minute= data_buff[5];
            gs_CurDateTime.Second= data_buff[6];
            EA=0;
            Set_RTCTime(&gs_CurDateTime);
            EA=1;
            mem_db_write(ADR_METER_VAR_RTC, &gs_CurDateTime.Week, 7, MEM_E2P1);
			CLRWDT();  
				    api_deal_even_class_recode(ID_CLASS_I_setRtc,START);
            api_init_md_data_ram();
		#if (TARIFF_MAX_NUM>0)
            api_update_triffNo_pre_minute();
		#endif
            break;
			

            //������⴦��
            case 0x04000401://���ַ ��� 16���ֽ� ����Ӧ,������������ǰ��0
                MeterID_LenTmp = (len-16);	//��Ч���  ����
                if(((len-16)%2)!=0)	//������ǰ�油һ��0
                {
	                for(i=(len-16);i>0;i--)
	                {
		                *(sptr+13+i) = *(sptr+13+i-1) ;	//���Ų1λ
	                }
	                *(sptr+13)= '0';					//��λ��0
	                MeterID_LenTmp = (len-16) +1;
                }
				comm_data.len_asc = MeterID_LenTmp;
				Lib_Set_String(&data_buff[0], 0xFF, 16);
				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)			
				mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);		//д��Ӧ����16�ֽڣ����������õĳ���
                break;

			
		case 0x04000B01://������DDHH 4
                Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
                if((data_buff[0]<0x01)||(data_buff[0]>0x28)||(data_buff[1]>0x23))		//�����ж�
                {
                    return(FALSE);
                }
                mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
                break;
//		case 0x04800003://��բ��բ����//)
//				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
//				if(Drv_relay_COMM_CMD_operating(&data_buff[0])==FALSE)	
////				{
//					return(FALSE);
//				}
//				break;	

		case 0x04000103://�������� PERIOD : SLID   4  //�޸ĺ��������¼��� //
                Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
                if(((data_buff[0]%data_buff[1]) !=0)||(data_buff[0]>30)||(data_buff[0]==0)||(data_buff[1]==0))		//�����ж�//
                {
                    return(FALSE);
                }
                mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
//				CLRWDT();
                api_init_md_data_ram();
				CLRWDT();
                break;


///////////���У��//////////////////
		case 0x0000FEB0: 
		case 0x0000FEB1:
		case 0x0000FEB2:
		case 0x0000FEB3:
		case 0x0000FEB4:
		case 0x0000FEB5:
			Measure_Electric_Err_Gain(sptr);//������У׼����ʱ1������У׼�������׼//	
		break;
		//У���ʼ��(��������)
		case 0x80800803:		
			if((*(sptr+13)=='3')&&(*(sptr+14)=='3')&&(*(sptr+15)=='3'))  
			{
				if((*(sptr+16)=='3'))					// clear all
				{
					gs_adj_emu_param.w1gain=0;
					gs_adj_emu_param.p1cal=0;
					gs_adj_emu_param.watt1os=0;
					gs_adj_emu_param.irms1os=0;
					gs_adj_emu_param.w2gain=0;
					gs_adj_emu_param.p2cal=0;
					gs_adj_emu_param.watt2os=0;
					gs_adj_emu_param.irms2os=0;
					gs_adj_emu_param.urmsos=0;
					CLRWDT();
				}
				if((*(sptr+16)=='2'))					// ��ڶ�·�������
				{
					gs_adj_emu_param.w1gain=0;
					gs_adj_emu_param.p1cal=0;
					gs_adj_emu_param.watt1os=0;
					gs_adj_emu_param.irms1os=0;
					gs_adj_emu_param.urmsos=0;
					CLRWDT();
				}
				Save_EMU_AdjParam();
				Check_EMU_AdjParam();//  �ָ�Ĭ������//
			}
			else
			{
				return FALSE;
			}
			break;	

		case 0x0000FEBB://������Ƶ10���� //	
			if((*(sptr+13)=='0')&&(*(sptr+14)=='0')&&(*(sptr+15)=='1')&&(*(sptr+16)=='6')) 
			{
				//��Ƶ10��������Ϊ10000 //
				data_buff[0] = 0x3C;	 // ����Ƶ10�� //
				mem_db_write(ADR_METER_PARAM1_CONST_FAST_FLG, &data_buff[0], 1, MEM_E2P1);
				Check_EMU_AdjParam();//  �ָ�Ĭ������//
			}
			else if((*(sptr+13)=='0')&&(*(sptr+14)=='1')&&(*(sptr+15)=='B')&&(*(sptr+16)=='8')) 
			{
				//��Ƶ�ָ�������Ϊ1000 //
				 data_buff[0] = 0;	   // ����Ƶ10�� //
				mem_db_write(ADR_METER_PARAM1_CONST_FAST_FLG, &data_buff[0], 1, MEM_E2P1);
				Check_EMU_AdjParam();//  �ָ�Ĭ������//
			}
			else
			{
				return FALSE;
			}
			break;
			
		case 0x08020200://NN=FF���������ͨ����//
			if((*(sptr+13)=='F')&&(*(sptr+14)=='F'))	//YYY
			{
//				api_clr_even_by_comm();	
				IsClrEvent = 1;
				CLRWDT();
			}
			else
			{
				return(FALSE);
			}
			break;
		
			case 0x08020100://ֻ�嵱ǰ��������//
				Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
				if((data_buff[0]!=0))  return(FALSE);
				CLRWDT();
				api_clr_current_MD_data();
				api_deal_even_class_recode(ID_CLASS_I_resetMD,START);
			CLRWDT();
			break;

	    case 0x08020300:
			// Asc_BCDA( &data_buff[0],(sptr+13), 1*2);
            // һ��ͨ�����ɼ�¼ָ�롢��¼���� 
            if((*(sptr+13)=='0')&&(*(sptr+14)=='0'))	//YYY
			{
				LoadProRst();
				CLRWDT();
			}
			else
			{
				return(FALSE);
			}
			break;
			
			
		case 0x08020400://��������(�������(����+����)) //
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
			if((data_buff[0]!=0)||(data_buff[1]!=0)||(data_buff[2]!=0)||(data_buff[3]!=0))  return(FALSE);
			CLRWDT();
			clr_flag = 1;
			break;

		case 0x08020500:// ����ģʽ//	
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
			gs_sys_globaL_var.meter_factory_status = data_buff[0] ; //����״̬��
			mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
			break;
			
		case 0xEE000118://EEPROM��־//
			Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);	 //AtoB(INT8U Asc)
			CLRWDT();
			mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
			CLRWDT();
			while(1)
			{
				NOP();			// ��λ���ȴ����ݼ��ظ����ݣ�10������ //
			}	
			CLRWDT();
			break;
			
		 //У�������������������//
		case 0x0000FEFF://NN=FF���������ͨ����//
			 if((*(sptr+13)=='5')&&(*(sptr+14)=='5')&&(*(sptr+15)=='5')&&(*(sptr+16)=='5'))  
			{
				mem_db_clr(0);   //������У���ȫ������ //
			   	//2019-01-24  cxy
				api_clr_freeze_energy();//�����������

                while(1)
                {
                    NOP();          // ��λ���ȴ����ݼ��ظ����ݣ�10������ //
                }	
				CLRWDT();
			}
			else if((*(sptr+13)=='3')&&(*(sptr+14)=='3')&&(*(sptr+15)=='3')&&(*(sptr+16)=='3'))  
			 {
				 mem_db_clr(0xA5);   //����У׼����ȫ������ //
				 while(1)
				 {
					 NOP(); 		 // ��λ���ȴ����ݼ��ظ����ݣ�10������ //
				 }	 
				 CLRWDT();
			 }
			else
			{
				return(FALSE);
			}
			break;

          default:
                 Lib_Asc_BCDA(&data_buff[0], (sptr+13), comm_data.len_asc);   //AtoB(INT8U Asc)
		    
                 if(mem_type == MEM_RAM)
                 {
                       mem_write((INT16U)comm_data.addr,&data_buff[0], comm_data.len, mem_type);
                 }
                 else
                 {
                       mem_db_write((INT16U)comm_data.addr, &data_buff[0], comm_data.len, mem_type);
                 }
				 
	          	break;	
		   
     	}
	 	
// ͨѶ���� ϵͳ����
    #if (TARIFF_MAX_NUM>1)
	if(((comm_data.di1_di0.u32 >= 0x04000801)&&(comm_data.di1_di0.u32 <= 0x04000807))
		||((comm_data.di1_di0.u32 >= 0x04010000)&&(comm_data.di1_di0.u32 <= 0x04010008)))
	{
	
		api_update_triffNo_pre_minute();
	}
    #endif

// �й���Ϸ�ʽ�ָ��´���//
	if((comm_data.di1_di0.u32 == 0x04000601))
	{
      mem_read(&gs_sys_globaL_var.fac_energy[0], ADR_METER_PARAM1_FAC_ENERGY, 2, MEM_E2P1);   
	}

	if((comm_data.di1_di0.u32 == 0x04000602))
	{
      mem_read(&gs_sys_globaL_var.Rac_energy[0], ADR_METER_PARAM1_REC_ENERGY, 2, MEM_E2P1);    
	}
/////////////////������ʾ��������Ҫ����///////
	if((comm_data.di1_di0.u32 == 0x04000302)||(comm_data.di1_di0.u32 == 0x04000303)||(comm_data.di1_di0.u32 == 0x04040100))
	{
		mem_read(&gs_dis_param.auto_sec, ADR_BLOCK21_DIS_PARAM_E2P, LEN_BLOCK21_DIS_PARAM_E2P, MEM_E2P1);
	}
//	CLRWDT();  //2019-04-11���ӿ��Ź���ֹ��λ	
    // �������óɹ������֡���� //
//	gs_uart_iec_app_var[g_ch_comm].tx_ready_10ms  += 4;
    api_deal_even_class_recode(ID_CLASS_I_PROG,START);   //���ɱ���¼���¼ //
//	IsProg = 1;
    gs_uart_iec_app_var[g_ch_comm].len= 1;
    Get_ACK_Answer(sptr);
//	gs_uart_iec_app_var[g_ch_comm].delay_10ms = 0;	 
    return(TRUE); 
}


//-------------------------------------------------------------------------------//
//  Function:     INT8U IEC_Cmd_Break_Deal(INT8U* sptr,INT8U len)
//  Description:   iec ͨѶ��ָֹ������
//  Parameters:   
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
INT8U IEC_Cmd_Break_Deal(INT8U* sptr)
{

       gs_uart_iec_app_var[g_ch_comm].iec_status |= COMM_BREAK_OK_IEC;
	 gs_uart_iec_app_var[g_ch_comm].len= 1;
       Get_ACK_Answer(sptr);	 
       return(TRUE);  
}

/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :��ʼ����ǰ��������RAM�����ݣ�����λ��ʼ������        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void lnk_tx_comm_ready( uint8 ch,uint8 start_index)
{
    uint16 u16_badu;
    //comm_IEC_Tx_Ready();   
    switch(gs_uart_iec_app_var[ch].baud_rate)
    {
        case CBaud_3:
            u16_badu =300;
        break;
        case CBaud_6:
            u16_badu =600;
        break;
        case CBaud_12:
            u16_badu =1200;
        break;
        case CBaud_24:
            u16_badu =2400;
        break;
        case CBaud_48:
            u16_badu =4800;
        break;
        case CBaud_96:
            u16_badu =9600;
        break;

        default:
            u16_badu =300;
        break;
    }

    P_IEC_BUFF = &gs_uart_drv_var[ch].buff[0] ;
   // Tx_UART_frist(&P_IEC_BUFF[0], (gs_uart_iec_app_var[ch].len+start_index),u16_badu,ch); // ���ݷ��� //     
    Tx_UART_frist(&P_IEC_BUFF[start_index], (gs_uart_iec_app_var[ch].len),u16_badu,ch);        // ���ݷ��� //     
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :��ʼ����ǰ��������RAM�����ݣ�����λ��ʼ������        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_handl_COMM_FRAME_ERROR( uint8 ch)
{
    if(ch==UART_CH0_IR)           
    {
        Lib_Clr_String(&gs_uart_iec_app_var[ch].len, sizeof(UART_COMM_APP_VAR));   
        Init_UART4_hard(BPS300);  //reset_uart(UART_RESET_ALL); 
        Init_UART_soft(ch); 
		    Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;		
    }
    
    if(ch==CH_RF)
    {
     Lib_Clr_String(&gs_uart_iec_app_var[ch].len, sizeof(UART_COMM_APP_VAR));   
     Init_UART_soft(ch); 
	 SX127x_init();
     Rx_mode(); 
    }
    
    return;
}

//-------------------------------------------------------------------------------//
//  Function:      void init_comm_link(INT8U mode)         
//  Description:   ��ʼ��������·���������
//  Parameters:   mode: ͨѶͨ�����  
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
void process_iec62056_21 (INT8U ch)
{
 INT8U buff_size;
 INT8U start_index;
 INT8U meterID_len;

 INT8U tmp_8u_01[16];
 INT8U tmp_8u_02[32];
// int i;
// INT16U  u16_badu;

    
    if(ch > UART_CH_INDEX_MAX)  return ;
    
    if(gs_uart_iec_app_var[ch].tx_delay_flg ==TRUE ) 
    {
        return ;    //���͵ȴ�״̬ʱ�˳� //
    }

    buff_size = Get_UART_rx_buff(&P_IEC_BUFF,ch);
    if(buff_size==0)   return;    // ÿ��ѭ����ȡһ�ν������ݳ��ȣ�=0���˳� //

/////////////////////////////////////////////////////////////////////////
    if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) !=0)
    {
        CLRWDT();
        CLRWDT();
    }
////////////////////////////////////////////////////////////////////////

    // ǰ���ֽڴ��� //
    for(start_index=0;start_index<5;start_index++)
    {
        if((P_IEC_BUFF[start_index]=='/')||(P_IEC_BUFF[start_index]==ACKCOM)||(P_IEC_BUFF[start_index]==SOH))  break;
    }

    if(start_index>3)
    {
        //���յ�����֡���󣬲��䴮�ڸ�λ����      //
        api_handl_COMM_FRAME_ERROR(ch);
        return;
    }

    gs_uart_iec_app_var[ch].tx_delay_flg =FALSE;   // �����ݽ���ʱ���������ʱ��ʶ //
    gs_uart_iec_app_var[ch].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
    gs_uart_iec_app_var[ch].err_flg = 0;
    g_ch_comm = ch;
    
    if(((buff_size >2) &&((P_IEC_BUFF[buff_size-2]) == CR ||(P_IEC_BUFF[buff_size-2]) == ETX )) )
    {
        buff_size =buff_size-start_index;
        // ����֡���� //
        switch(P_IEC_BUFF[start_index])
        {
            case IEC_COMM_REQUEST:	
                if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) !=0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
                mem_read (&tmp_8u_01[0],ADR_METER_PARAM1_METER_ID, 16,MEM_E2P1);
                meterID_len = get_meter_id_asclen(&tmp_8u_01[0], 16);	//��ȡ�����Чasc����
                Lib_BCD_AscA(&tmp_8u_02[0], &tmp_8u_01[0], 16);
                if(IEC_Comm_Request_Cmd_Judge(&P_IEC_BUFF[start_index], buff_size, &tmp_8u_02[0],meterID_len)==TRUE)
                {
                    if(ch==UART_CH0_IR)   // TTL//
                   {
                         Get_IEC_Comm_Request_Cmd_Answer(&P_IEC_BUFF[start_index], Meter_INFO_LENTH,(uint8*)&meter_info_IR[0]);//start_index
                    }
                     if(ch==CH_RF)  // RF//
                   {
                         Get_IEC_Comm_Request_Cmd_Answer(&P_IEC_BUFF[start_index], Meter_INFO_LENTH,(uint8*)&meter_info[0]);
                    }           
                    gs_uart_iec_app_var[ch].len = Meter_INFO_LENTH+3;
                    gs_uart_iec_app_var[ch].iec_status |= COMM_INFO_OK_IEC;    // ��������ɹ���ʶ //
                }
                else
                {
                    //���ǵ�����ʽͨѶ��ʧ��ʱ���������� //
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
            break;

            case IEC_COMM_ACKNOWLEDGEMENT:
                if((gs_uart_iec_app_var[ch].iec_status & COMM_INFO_OK_IEC) ==0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                }
                if(IEC_Comm_Acknowledgement_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size)==TRUE)
                {
                    gs_uart_iec_app_var[ch].baud_rate = P_IEC_BUFF[start_index+2];      //                                     //�����ʸ�ֵ  //
                    Get_IEC_Comm_Operand_Answer(&P_IEC_BUFF[start_index], MIMA_INFO_LENTH,(uint8*)&mima_info[0]); //   // ��ȡ������֡ //
                    gs_uart_iec_app_var[ch].len = (MIMA_INFO_LENTH+6);
                    gs_uart_iec_app_var[ch].iec_status |= COMM_REQUIST_OK_IEC;  // ������������ɹ���ʶ //
                }
                else
                {
                    gs_uart_iec_app_var[ch].iec_status &= ~COMM_REQUIST_OK_IEC;  // clr ��������ɹ���ʶ //
                    Get_Error_Answer(&P_IEC_BUFF[start_index]);//
                    gs_uart_iec_app_var[ch].len = 1;
                }

            break;

            case IEC_COMM_COMMAND:
                if((gs_uart_iec_app_var[ch].iec_status & COMM_REQUIST_OK_IEC) ==0)
                {
                    api_handl_COMM_FRAME_ERROR(ch);
                    return;
                } 
	//			i = IEC_Comm_Command_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size);             
                if(IEC_Comm_Command_Cmd_Judge(&P_IEC_BUFF[start_index],buff_size) !=TRUE)//
                {
                     // �������
			// ���ݽ���ʧ�� //
                    Get_Error_Answer(&P_IEC_BUFF[start_index]);
                    gs_uart_iec_app_var[ch].len = 1;   		           
                }
            break;

            default:
                gs_uart_iec_app_var[ch].err_flg |= FRAME_ERROR;   //֡������Ҫ��λ  //	
            break;
        }

    }
    else
    {
        gs_uart_iec_app_var[ch].err_flg |= FRAME_ERROR;   //֡������Ҫ��λ  //	
    }

    if((gs_uart_iec_app_var[ch].err_flg & FRAME_ERROR) )
    {
        //���յ�����֡���󣬲��䴮�ڸ�λ����      //
        api_handl_COMM_FRAME_ERROR(ch);
        return;
    }
    else
    {
        gs_uart_iec_app_var[ch].tx_delay_flg =TRUE;
        gs_uart_iec_app_var[ch].tx_ready_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
        if(gs_uart_iec_app_var[ch].iec_status ==(COMM_INFO_OK_IEC+COMM_REQUIST_OK_IEC))
        {
            gs_uart_iec_app_var[ch].tx_ready_10ms  += 4;   // �л�������ʱ��ʱ����10ms //
        }
        gs_uart_iec_app_var[ch].start_index = start_index;
    }
}


/*****************************************************************************
** Function name    :api_init_md_data_ram
**
** Description         :��ʼ����ǰ��������RAM�����ݣ�����λ��ʼ������        
**
** Parameters         :NONE          
**
** Returned value   :NONE
**
**----------------------------------------------------------------------------
** V01.01  MJ  2016-04-23
******************************************************************************/
void api_handl_COMM_pre_10ms(uint8 ch)
{

        process_iec62056_21(ch);    

    if(gs_uart_iec_app_var[ch].delay_10ms >COMM_DELAY_3500MS)
    {   
        //��ʱ�˳����֣����½�����еȴ�״̬ //
        api_handl_COMM_FRAME_ERROR(ch);    
    }

    if((gs_uart_iec_app_var[ch].tx_ready_10ms ==0)&&(gs_uart_iec_app_var[ch].tx_delay_flg ==TRUE))
    {   
        //����׼������   //
        gs_uart_iec_app_var[ch].tx_delay_flg =FALSE;
        lnk_tx_comm_ready(ch,gs_uart_iec_app_var[ch].start_index);     
    }

	if(clr_flag == 1)
	{
		clr_flag = 0;
		CLRWDT();
		api_clr_current_energy_data();
		CLRWDT();
		api_clr_current_MD_data();
		CLRWDT();
		#if (BILL_MAX_NUM>0)
		api_clr_bill_data();
		#endif
		gs_uart_iec_app_var[g_ch_comm].delay_10ms = 0; // 
		LoadProRst();
		CLRWDT();
		api_clr_even_by_comm(); 
		CLRWDT();
		//2019-01-24  cxy
		api_clr_freeze_energy();//�����������
		CLRWDT();
		api_chg_LCDDisplay_adj_item(DIS_DATA_CLR);   // Һ����ʾ //
	}
		
	if(IsClrEvent == 1)
	{
		IsClrEvent = 0;
		api_clr_even_by_comm();
	}
//	if(IsProg == 1)
//	{
//		IsProg = 0;
//		api_deal_even_class_recode(ID_CLASS_I_PROG,START);
//	}

}
///////////////////////////////////////////////////////////////////
//  RF ͨѶ����
///////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//  Function:      void uart_comm_deal(void)    ����ͨѶ������        
//  Description:   ʵ�ִ��ڹ��ܵĹ���Ͳ���
//  Parameters:   ��  
//  Returns:        ��                          
//  Others:         
//-------------------------------------------------------------------------------//
void RF_comm_deal(void)
{  
    // RF ͨ���Ƿ���յ����� //
    if(LSD4RF_READ_DIO0() ==0) return;
    Lib_Delay_Nop(20);
    if(LSD4RF_READ_DIO0() ==0) return;   
    
    // ���յ����ݺ�Ĵ��� //
	LSD_RF_RxPacket(&gs_uart_drv_var[CH_RF].buff[0]);
    CLRWDT();   // �忴�Ź�//
    gs_uart_iec_app_var[CH_RF].delay_10ms = 0;   // �����ݴ���ʱ�����ʱ������ //
    gs_uart_drv_var[CH_RF].rx_over_ticks =5;
	////////////////////////
	gs_uart_drv_var[CH_RF].buff_index = G_LoRaConfig.PayloadLength;
     ///////////////////////////////
}
/***************************************************************
*    END
****************************************************************/

