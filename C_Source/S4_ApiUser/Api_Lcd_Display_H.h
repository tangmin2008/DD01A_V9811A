/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Api_Lcd_Display_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: Һ����ʾ����Ӧ�ò���غ�������H�ļ�
**note			: V9811A ��MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/


#ifndef _Api_Lcd_Display_H
#define _Api_Lcd_Display_H


/***************************************************************
*    1     ������Ԥ����   
****************************************************************/
//DIS_PIXEL_VAR ����BUFF�±궨��//
#define DS_UNIT			12 	//��10 ����ʾ��Ԫ//
#define DS_DATANum		6  //�����ָ���//
#define DS_OBISNum		3  //OBIS����//
#define DS_CHARNum		3  //�����ַ�����//

//  //
#define DIS_RESET 	  0   //ȫ���Լ���ʾ  //
#define DIS_ITEM_V    1   //��ѹ�� //

#define DIS_MAX_ITEM   6    // �����ʾ���6//

#define DIS_DEFAULT_ITEM 	  3   //Ĭ����ʾ��ǰ���й�������Ŀ  //
#define RESEAT_DIS_KEEP_3S   2	//���ϵĸ�λȫ����ʾ����ʱ�� //
//#define DIS_ERR_START  7      // ��������ʾ��ʼ������Ŀǰ���Կ��Ǵ���


// Ӳ��У����ʾ��Ŀ //
#define DIS_ADJ_CH1_0 	  0XA0   //L ��·У�������ʼ�� //
#define DIS_ADJ_CH1_1 	  0XA1  //L ��·1.0Ib �Ȳ�У׼ //
#define DIS_ADJ_CH1_2 	  0XA2   //L ��·1.0Ib 0.5L �ǲ�У׼ //
#define DIS_ADJ_CH1_3 	  0XA3   //L ��·0.05Ib С�źŲ���У׼ //
#define DIS_ADJ_CH2_0 	  0XA4   //N ��·У�������ʼ�� //
#define DIS_ADJ_CH2_1 	  0XA5  //N ��·1.0Ib �Ȳ�У׼ //
#define DIS_ADJ_CH2_2 	  0XA6   //N��·1.0Ib 0.5L �ǲ�У׼ //
#define DIS_ADJ_CH2_3 	  0XA7   //N��·0.05Ib С�źŲ���У׼ //
#define DIS_DATA_CLR 	  0XA8   //������� //
#define DIS_DATA_BROADCAST 	  0XA9 //�㲥������ʾ //
//DIS_CTL_VAR.mode define //
#define DIS_MODE_RESET	  0x01   // ��λ
#define DIS_MODE_NOMAL  0x02  	//����

//LCDDISTAB ����//
//LCDDISTAB.chartype define //
#define CHAR_AEC		0x01		// ������
#define CHAR_MD			0x02		// ������
#define CHAR_MD_TIME	0x03		// ��������ʱ��
#define CHAR_V			0x04		// ��ѹ��
#define CHAR_A			0x05		// ������
#define CHAR_KW			0x06		// ������
#define CHAR_HZ			0x07		// Ƶ����
#define CHAR_TIME		0x08		// ʱ����
#define CHAR_DATE		0x09		// ������
#define CHAR_PF			0x0A		// ����������
#define CHAR_SETTL		0x0B		// ���������
#define CHAR_METERID     0x0C                  //�����
//LCDDISTAB.obis_digit define //
#define  OBIS_DIGIT_TYPE0  0  // xxxx
#define  OBIS_DIGIT_TYPE1  1  // xxx.x
#define  OBIS_DIGIT_TYPE2  2  //xx.xx
#define  OBIS_DIGIT_TYPE3  3  // x.xxx
#define  OBIS_DIGIT_TYPE4  4  // x.xx.x

/***************************************************************
*    2     ���ݽṹ����     
****************************************************************/
//��ʾ�������� //
typedef struct 
{
    uint8  auto_sec;    // �Զ�ѭ����ʾ����ʱ��  //
    uint8  key_sec;     // ������ʾ����ʱ��  //
    uint32  sleep_sec;     //�µ���ʾά��ʱ��//
    uint8  ec_form;     // ������ʽ  //
    uint8  auto_item[DIS_MAX_ITEM+1];    //0Ϊ��ʾ������1~6Ϊ��ʾ��Ŀ����������ʾ6����Ŀ  //
    INT16U  u16_csck;    //16λ�ۼӺ�У��ֵ//
} DIS_PARAM_DATA;

//��ʾ���Ʊ����� //
typedef struct 
{
    uint8  mode;    // ��ʾģʽ //
    uint8  item;      // ��ǰ��ʾ��Ŀ  //
    uint8  ptr[4];    //������ʾģʽ����Ŀָ�룬0��ų�����ʾģʽ //
    uint8  keep_sec[4];  // ��ʾ�����ʱ��, 0���Ϊ�Զ�ѭ����ʾ,1���Ϊ������ʾ����ʱ��2Ϊ����ά��ʱ��3Ϊ�ϵ�ȫ������ʱ��//
} DIS_CTL_VAR;

//��ʾԪ�ر����� //
typedef struct 
{
    uint8  dis_buff[DS_UNIT];    // ��ʾ���� //
    uint8  dis_data_buff[DS_DATANum];				// data��8 λ������ʾת������ //
    uint8  dis_obis_buff[DS_OBISNum];				// OBIS �� 6 λ������ʾת������ //
    uint8  dis_char_buff[DS_CHARNum];				// ���CHAR��14��������ʾת������ //
} DIS_PIXEL_VAR;


//��ʾ���ر�ṹ����
typedef struct  
{
    INT8U      DisID;               //��ʾID//
    INT16U     ObisNum;         // OBIS����,��Ϊ4������//
    INT16U     Addr;                //ram�� E2 ��ַ//
    INT16U     DisInfo ;            // �洢����(H)+ ��ʾλ��(L) // 
    INT8U      chartype;            // �������� //        
    INT8U      obis_digit;           //OBISС����ʽ����//
}LCDDISTAB;

/***************************************************************
*    3    ��������       
****************************************************************/
extern DIS_PARAM_DATA   gs_dis_param;   // ������ʾ��ز���  //
extern DIS_CTL_VAR  gs_dis_ctl_var;
extern DIS_PIXEL_VAR  gs_dis_pixel_var;
extern LCDDISTAB   gs_LCDDISTAB_var;

extern const DIS_PARAM_DATA  code  default_dis_param_tab;

extern INT8U dis_num_char;

/***************************************************************
*    4     ��������        
****************************************************************/
extern void api_init_display(void);
extern void api_handl_dis_sleep(void);  
extern void api_updated_LCDDisplayPixel_per_second(void);
extern void api_updated_LED_per_second(void);
extern void api_updated_LCDDisplayItem_per_second(void);
extern INT8U api_get_next_dis_item(void);
extern INT8U Link_Get_DisTab_Var(INT8U u8_dis_itm);
extern void Link_Get_Dis_OBIS_Num_buff(void);
extern void Link_Get_Dis_Data_Num_buff(void);
extern void Link_Get_Dis_Char_buff(void);
extern void Link_Get_Dis_RealChar_buff(void);
extern void Link_Get_Dis_Drv_buff(void);
extern void api_get_energy_LCD(uint8*  temp, uint8 type, uint8*  result);
extern void api_chg_LCDDisplay_adj_item(uint8 u8_item);
extern void api_LCDDisplay_adj_item(uint8 u8_item);
extern void LED_display(void);
/***************************************************************
*    5     �������ݣ�����αָ���       
****************************************************************/
//********************************************************************
// LCD character bitmap
//          3                a
//       -----            -----
//       |     |          |     |
//      2|     |7        f|     |b
//       |  6  |          |  g  |
//        -----            -----
//       |     |          |     |
//      1|     |5        e|     |c
//       |  0  |          |  d  |
//        -----  *4        -----  *dpc
//******************************************************************* 
//********************************************************************
// ����������
//*******************************************************************                   								
//******************************************************************* 
//********************************************************************
// ����������					    / E G F	D C B A
//*******************************************************************                   								
#define DS_Chr_0     0x5F   // 0: 
#define DS_Chr_1     0x06   // 1:
#define DS_Chr_2     0x6B   // 2: 
#define DS_Chr_3     0x2F   // 3: 
#define DS_Chr_4     0x36   // 4: 
#define DS_Chr_5     0x3D   // 5: 
#define DS_Chr_6     0x7D   // 6: 
#define DS_Chr_7     0x07   // 7: 
#define DS_Chr_8     0x7F   // 8: 
#define DS_Chr_9     0x3F   // 9: 
#define DS_Chr_A     0x77   // A: 
#define DS_Chr_b     0x7C   // B: 
#define DS_Chr_C     0x59   // C: 
#define DS_Chr_d     0x6E   // D: 
#define DS_Chr_E     0x79   // E: 
#define DS_Chr_F     0x76   // F: 

//********************************************************************
// ����OBIS   ǰ3 λ���� 			F G E / A B C D 
//*******************************************************************                 								
#define DS_OBIS_Chr_0     0xAF   // 0: 
#define DS_OBIS_Chr_1     0x06   // 1:
#define DS_OBIS_Chr_2     0x6D   // 2: 
#define DS_OBIS_Chr_3     0x4F   // 3: 
#define DS_OBIS_Chr_4     0xC6   // 4: 
#define DS_OBIS_Chr_5     0xCB   // 5: 
#define DS_OBIS_Chr_6     0xEB   // 6: 
#define DS_OBIS_Chr_7     0x0E   // 7: 
#define DS_OBIS_Chr_8     0xEF   // 8: 
#define DS_OBIS_Chr_9     0xCF   // 9: 
#define DS_OBIS_Chr_A     0xEE   // A: 
#define DS_OBIS_Chr_b     0xE3   // B: 
#define DS_OBIS_Chr_C     0xA9   // C: 
#define DS_OBIS_Chr_d     0x67   // D: 
#define DS_OBIS_Chr_E     0xE9   // E: 
#define DS_OBIS_Chr_F     0xE8   // F: 



#define DS_Chr_All_NUM 16  
//�ַ�����//

//dis_var.dis_char_buff[0]  bit6:bit0      //
#define CHAR_DATA_P1P2 	BIT0   
#define CHAR_DATA_P3 	BIT1    
#define CHAR_DATA_P4P5 	BIT2    
#define CHAR_DATA_P6	BIT3   
#define CHAR_DATA_P7 	BIT4   
#define CHAR_DATA_P8 	BIT5  
//#define CHAR_DATA_P9 	BIT6
#define CHAR_DATA_EOI	BIT6
#define CHAR_DATA_TEST	BIT7   


//dis_var.dis_char_buff[1]  bit7:bit0      //
#define CHAR_DATA_kWh 	BIT0   
#define CHAR_DATA_kW 	BIT1   
#define CHAR_DATA_S1 	BIT2  
#define CHAR_DATA_S2 	BIT3  
#define CHAR_DATA_S3 	BIT4  
#define CHAR_DATA_S4 	BIT5  
#define CHAR_DATA_Received 	BIT6
#define CHAR_DATA_Delivered 	BIT7  



//dis_var.dis_char_buff[2]  bit7:bit0      //
//#define CHAR_DATA_k 	BIT0   
//#define CHAR_DATA_W 	BIT1   
//#define CHAR_DATA_V 	BIT2  
//#define CHAR_DATA_A 	BIT3  
//#define CHAR_DATA_Hz 	BIT4  
//#define CHAR_DATA_PF 	BIT5  
//#define CHAR_DATA_MD 	BIT6  
//#define CHAR_DATA_S9 	BIT7 
#define CHAR_DATA_V   	BIT0
#define CHAR_DATA_A		BIT1
#define CHAR_DATA_Hz	BIT2
#define CHAR_DATA_MD	BIT3
#define CHAR_DATA_S9	BIT4
#define CHAR_DATA_S12	BIT5
#define CHAR_DATA_S13	BIT6
#define CHAR_DATA_S14	BIT7


/***************************************************************
*    END
****************************************************************/

#endif
