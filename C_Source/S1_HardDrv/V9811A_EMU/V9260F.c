#define  RACCOONEXT
#include <MyIncludes_H.h>


/*=========================================================================================\n
* @function_name: Init_RacCtrl
* @function_file: ComRaccoon.c
* @����: 
* 
* @����: 
* @����: 
* @����:   (2013-04-15)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Init_RacCtrl(void)
{
    uint8 i;
    gs_RacCtrl.ucSendLen=0;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacCtrl.ucStatus=Rac_Idle;
    for(i=0;i<100;i++)
    {
        gs_RacCtrl.ucBuf[i] =0;
    }
}
/*=========================================================================================\n
* @function_name: RxReset_V9281
* @function_file: Raccoon.c
* @����: �͵�ƽ��Ч P2.5����͵�ƽ ����63ms
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void RxReset_V9260(void)
{
//    P21FS=0; 
//    P20FS=0; 
//    P2OE&=(~BIT1);       //TXD �������
//    P2IE&=(~BIT1);       
//    
//    P2OD |=(BIT1);
//
//    P2OD&=(~BIT1);  
//	// ȥ���ж� ��ʹ����ʱ//
//	//guc_CommDelayTime = 10;
//	// while(guc_CommDelayTime >0);  //100ms
//	CLRWDT(); 	
//	Lib_Delay_ms(100);
//	CLRWDT(); 	
//     P2OD |=(BIT1);
//
//    P21FS=2;  
//    P20FS=2; 
}
/*=========================================================================================\n
* @function_name: WriteRaccoon
* @function_file: Raccoon.h
* @����: ��raccoon��������
* 
* 
* @����: 
* @param:Data  д������
*               �ֽ�1��֡ͷ0xFE    �ֽ�2����4λ��ַ��4bit����4λ����
*               �ֽ�3����ַ��8bit  �ֽ�4~7������   �ֽ�8��У��
* @param:addr  д���ַ
* ����
* @����: 
* @return: uint8 
* @����:   lwb (2013-07-04)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  hjj
* @�޸�����: ���Raccoon�����ַ�������һ��ʹ��3��Raccoon
* @param:Phyadd Raccoon��ַ

===========================================================================================*/
uint8  WriteRaccoon(uint32 Data,uint16 Addr,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    switch(Phyadd)
    {
    case APhy:
      {
        return SetMeterCfgACK( Data, Addr);
      }
      break;
    case BPhy:
      {   
        gs_RacCtrl.ucBuf[0] = 0xfe;
        
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+RacWrite+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = (uint8)(Data&0x000000ff);
        gs_RacCtrl.ucBuf[4] = (uint8)((Data&0x0000ff00)>>8);
        gs_RacCtrl.ucBuf[5] = (uint8)((Data&0x00ff0000)>>16);
        gs_RacCtrl.ucBuf[6] = (uint8)((Data&0xff000000)>>24);
        //����У���
        gs_RacCtrl.ucBuf[7] =  gs_RacCtrl.ucBuf[0];
        for(i=1;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        
        gs_RacCtrl.ucStatus=Rac_Send;       //���뷢��״̬
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=4;
        gs_RacCtrl.ucRevPoint=0;
        //gs_RacPhy.uc_Uartstat |= flgEtwrite;
         	 Uart4_Transmit();
        guc_CommDelayTime=9;
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
        {
          CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            //gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
            return FALSE;                   //�����ʱ 
          }
        }  
        //gs_RacPhy.uc_Uartstat &= ~flgEtwrite;
        ucSum=0;
        for(i=0;i<3;i++)
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;
        Lib_Delay_ms(5);
        if(gs_RacCtrl.ucBuf[3]==ucSum)
        {
          return TRUE;
        }else
        {
          return FALSE;
        }
      }
      break;
    default:
      return FALSE;
      break;
    }
  //  return TRUE;
}

/*=========================================================================================\n
* @function_name: ReadRaccoon
* @function_file: Raccoon.h
* @����: ��raccoon��ȡ����
* �ֽ�0��֡ͷ0xFE    �ֽ�1����4λ��ַ��4bit����4λ����
* �ֽ�2����ַ��8bit  �ֽ�3��word���� �ֽ�4~6�������� �ֽ�7��У��
* @����: 
* @param:Addr  ��ȡ��ʼ��ַ
* @param:num  ��ȡword������32bit
* 
* @����: 
*        
* @return: uint8 
* @����:   lwb (2013-07-04)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: ���Raccoon�����ַ�������һ��ʹ��3��Raccoon
* @param:Phyadd Raccoon��ַ

==========================================================================================*/
uint8 ReadRaccoon(uint16 Addr,uint8 num,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
    ST_U32_U08 TempValue;  
	
    switch(Phyadd)
    {
    case APhy:               //��9811A
      {
        gs_RacCtrl.ucBuf[0] = 0xFE;      //��raccoon�н��յ����ݱ���һ��
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ APhy;
        gs_RacCtrl.ucBuf[2] = num;
        
        for( i=0; i<num; i++)               //��ȡ������100���ֽ�
        {
          TempValue.u32= ReadMeterParaACK( Addr+i);
          memcpy(gs_RacCtrl.ucBuf+3+4*i,(uint8*)TempValue.B08,4); 
        }
        ucSum=0;;
        for(i=0;i<num*4+3;i++)
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        ucSum = ~ucSum;
        gs_RacCtrl.ucBuf[num*4+3] = ucSum+0x33;
      }
      break;
      
    case BPhy:
      {
        gs_RacCtrl.ucBuf[0] = 0xFE;
        
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = num;                   //num����Ҫ��ȡ�����ֵ�����
        gs_RacCtrl.ucBuf[4] = 0;
        gs_RacCtrl.ucBuf[5] = 0;
        gs_RacCtrl.ucBuf[6] = 0;
        gs_RacCtrl.ucBuf[7] = 0;                    //sum ����
        for(i=0;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        gs_RacCtrl.ucStatus=Rac_Send;       //���뷢��״̬


		
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=(num*4)+4;           
        gs_RacCtrl.ucRevPoint=0;
        guc_CommDelayTime=17; 
        //gs_RacPhy.uc_Uartstat |= flgEtread;   
        Uart4_Transmit();           //��������
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
        {

           CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            //gs_RacPhy.uc_Uartstat &= ~flgEtread;
            return FALSE;           //�����ʱ 
            
          }
        }
        //gs_RacPhy.uc_Uartstat &= ~flgEtread;
        ucSum=0;
        for(i=0;i<(num*4)+3;i++)               //��ȡ������255���ֽ�
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;

        if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
        {
          return TRUE;
        }else
        {
          return FALSE;
        } 
      }
    default:
      return FALSE;
      break;
    }
    return TRUE;
}

/*=========================================================================================\n
* @function_name: ReadRaccoonInt
* @function_file: Raccoon.h
* @����: ��raccoon��ȡ��������
* �ֽ�0��֡ͷ0xFE    �ֽ�1����4λ��ַ��4bit����4λ����
* �ֽ�2����ַ��8bit  �ֽ�3��word���� �ֽ�4~6�������� �ֽ�7��У��
* @����: 
* @param:Addr  ��ȡ��ʼ��ַ
* @param:num  ��ȡword������32bit
* 
* @����: 
*        
* @return: uint8 
* @����:  
* @��ע: 
*-------------------------------------------------------------------------------------------

==========================================================================================*/
void ReadRaccoonInt(uint16 Addr,uint8 num,uint8 Phyadd)
{
    uint8 i;
    volatile uint8 ucSum;
 //   ST_U32_U08 TempValue;
    switch(Phyadd)
    {
    case BPhy:
      {     
        gs_RacCtrl.ucBuf[0] = 0xfe;   
        gs_RacCtrl.ucBuf[1] = (uint8)((Addr&0x0f00)>>4)+ RacRead+ BPhy;
        gs_RacCtrl.ucBuf[2] = (uint8)(Addr&0x00ff);
        
        gs_RacCtrl.ucBuf[3] = num;                   //num����Ҫ��ȡ�����ֵ�����
        gs_RacCtrl.ucBuf[4] = 0;
        gs_RacCtrl.ucBuf[5] = 0;
        gs_RacCtrl.ucBuf[6] = 0;
        gs_RacCtrl.ucBuf[7] = 0;                    //sum ����
        for(i=0;i<7;i++)
        {
          gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
        }
        
        gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
        gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;
        
        gs_RacCtrl.ucStatus=Rac_Send;       //���뷢��״̬
        gs_RacCtrl.ucSendLen=8;
        gs_RacCtrl.ucSendPoint=0;
        gs_RacCtrl.ucRevLen=(num*4)+4;           
        gs_RacCtrl.ucRevPoint=0;
        gs_RacPhy.uc_Physendtim= guc_Dy9260FOver; //�˿ڳ�ʱ����
        gs_RacPhy.uc_Phystat = BPhy;
        gs_RacPhy.uc_Sendstat = TRUE;


    // ��������  200ms ������ʱ//
        guc_CommDelayTime=0x20;     
         Uart4_Transmit();           //��������
        while(gs_RacCtrl.ucStatus!=Rac_WaitPro)   // �ȴ��жϽ������//
        {
          CLRWDT(); 
          if(guc_CommDelayTime==0)
          {
            goto ENDERR;
          }
        }

        ucSum=0;
        for(i=0;i<(num*4)+3;i++)               //��ȡ������255���ֽ�
        {
          ucSum += gs_RacCtrl.ucBuf[i]; 
        }
        
        ucSum=~ucSum;
        ucSum+=0x33;

        if(gs_RacCtrl.ucBuf[num*4+3]==ucSum)
        {
            gs_sys_run.font_fg |= BIT3_FONT_FG_readpower; // ��ȡ�������ɱ�ʶ//
 
        }else
        {
ENDERR:    gs_RacPhy.uc_Sendstat = FALSE;
             gs_RacPhy.uc_Physendtim = guc_Dy9260FOver;  
             gs_sys_run.font_fg |= BIT4_FONT_FG_EnyBottom;  // ����ˢ��
            // return FALSE;     
        } 
      }
      break;
    default:
      break;
    }

	 
}


uint8 I_Dirct = 0,Aphy_Dly = 0;    
/*=========================================================================================\n
* @function_name: ReadbackABCPhy
* @function_file: ���ʡ���ѹ����ԭʼֵ��ȡ//
* @����: 
* 
* 
* @����: 
* @param:Phyadd  
* @����: 
* @����:   zwk(2013-09-25)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void ReadbackABCPhy(void)
{
    volatile uint8 ucSum;
	uint8 i;
    ST_U32_U08 TempValue,TempValue1,TempValue2,TempValue3,TempValue4;//TempValue5;
    TempValue.u32 = 0;
    TempValue1.u32 = 0;
    TempValue2.u32 = 0;
    TempValue3.u32 = 0;
    TempValue4.u32 = 1;        

    ucSum=0;
    gs_RacPhy.uc_Physendtim = 0;
    for(i=0;i<(5*4)+3;i++)               //��ȡ������255���ֽ�
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }

    ucSum=~ucSum;
    ucSum+=0x33;

    if(gs_RacCtrl.ucBuf[5*4+3]==ucSum)
    {
      if((gs_RacCtrl.ucBuf[1]&0x03) != RacRead)  // ��֤Ϊ������//
      {
        return;
      }
      else
      {
            memcpy((uint8*)TempValue.B08,gs_RacCtrl.ucBuf+3,4); //�й���ƽ������
            Lib_Rev_Copy_Arry(&TempValue.B08[0],4);// ���ֽ���ǰ  ����//
            if(TempValue.B08[3]>0x7f)          //����Ǹ����������ȱ������
            {
                TempValue.u32=~TempValue.u32+1;
            }
            memcpy((uint8*)TempValue1.B08,gs_RacCtrl.ucBuf+7,4);//�޹�
            Lib_Rev_Copy_Arry(&TempValue1.B08[0],4);// ���ֽ���ǰ  ����//
            if(TempValue1.B08[3]>0x7f)          //����Ǹ����������ȱ������
            {
                TempValue1.u32=~(TempValue1.u32) +1;
            }
            memcpy((uint8*)TempValue2.B08,gs_RacCtrl.ucBuf+11,4);//��ѹ
            Lib_Rev_Copy_Arry(&TempValue2.B08[0],4);// ���ֽ���ǰ  ����//
            memcpy((uint8*)TempValue3.B08,gs_RacCtrl.ucBuf+15,4);//����
            Lib_Rev_Copy_Arry(&TempValue3.B08[0],4);// ���ֽ���ǰ  ����//
            memcpy((uint8*)TempValue4.B08,gs_RacCtrl.ucBuf+19,4);//Ƶ��
            Lib_Rev_Copy_Arry(&TempValue4.B08[0],4);// ���ֽ���ǰ  ����//
       }
    }
    else
    {        
      
      TempValue.u32= 0;
      TempValue1.u32 = 0;
      TempValue2.u32 = 0;
      TempValue3.u32 = 0;
      TempValue4.u32 = 1;        
    }
         
         gs_RacPhy.uc_Powstat &= ~power_b;  //�е��־ 
         gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32=  (TempValue.u32+(TempValue.u32/12)); //9811A�й���9260�й���1.086��
	     gs_measure_var_data.gs_really[WIRE_L2].dw_u_val.u32=  TempValue2.u32;
         gs_measure_var_data.gs_really[WIRE_L2].dw_i_val.u32=   TempValue2.u32;
         gs_measure_var_data.gs_really[WIRE_L2].w_freq_val.u16 =  (uint16)(((uint32)23575*divf)/TempValue4.u32);
         if(( gs_measure_var_data.gs_really[WIRE_L2].w_freq_val.u16 /gs_JbPm.ul_PG) < ((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))/500 )
         {
            gs_measure_var_data.gs_really[WIRE_L2].dw_p_val.u32   = 0;
            guc_powerdirB = 0;
         }
}

/*=========================================================================================\n
* @function_name: CheckRegSum
* @function_file: Raccoon.c
* @����: ���У���
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CheckRegSum(void)
{
    uint32 ucSum=0;

    ucSum = 
          + gs_JbPm.gs_JBB.Rac_WARTI 
          + gs_JbPm.gs_JBB.Rac_WARTU 
          + gs_JbPm.gs_JBB.Rac_WAPT
          + gs_JbPm.gs_JBB.Rac_WAPT //�޹��Ȳ�
          + gs_JbPm.gs_JBB.Rac_WWAPT
          + gs_JbPm.gs_JBB.Rac_WWAQT
          + gs_JbPm.Rac_BPFPARA
          + gs_JbPm.Rac_ANCtrl0
          + gs_JbPm.Rac_ANCtrl1
          + gs_JbPm.Rac_ANCtrl2
          + gs_JbPm.Rac_MTPARA0
          + gs_JbPm.gs_JBB.Rac_MTPARA1;   
    ucSum = 0xFFFFFFFF-ucSum;
    WriteRaccoon(ucSum,RegCKSUM,BPhy);

}

 
/*=========================================================================================\n
* @function_name: CheckSysErr
* @function_file: Raccoon.c
* @����: RACCOON��ϵͳ�Լ�У��  CHSUM
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void CheckSysErr(void)
{
    WriteRaccoon( 0x09FB0004,RegSysCtrl,BPhy);
}
/*=========================================================================================\n
* @function_name: V9281_InitDefaultPara
* @function_file: Raccoon.c
* @����: ��ʼ��Ĭ�ϲ���
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void V9260_InitDefaultPara(void)
{
    gs_JbPm.ui_MeterC=1200;             // ����
    gs_JbPm.ui_Un=12700;                // ��Ƶ�ѹ
    gs_JbPm.ui_Ib=5000;                 // ��Ƶ���
    
    gs_JbPm.Com_EGYTH     = 0x067215D8;    //�й�����ֵ
    gs_JbPm.Com_REEGYTH   = 0x067215D8;    //�޹�����ֵ
    
    gs_JbPm.Rac_ANCtrl0 = 0xDf000440;    //0x185 ����I:32��U:1��M:4 �¶�ϵ����ȫ�ֵ���ƫ��
    gs_JbPm.Rac_ANCtrl1 = 0x30000000;    //0x186 
    gs_JbPm.Rac_ANCtrl2 = 0x0000000F;    //0x187     
    gs_JbPm.Rac_BPFPARA = 0x811D2BA7;    //0x0125 ��ͨ�˲���ϵ�� 0x811D2BA7
    gs_JbPm.Rac_MTPARA0 = 0xE0000000;    //0x183 CF����ʱ��3.2MHz ʹ��CF����������ۼӣ���Ǳ����������Դ��ƽ��ȫ���й�
    
    gs_JbPm.gs_JBA.Soc_SCP = 0x00000000;         //�й����ʱȲ�Ĵ���  
    gs_JbPm.gs_JBA.Soc_SCQ = 0x00000000;         //�޹����ʱȲ�Ĵ���  
    gs_JbPm.gs_JBA.Soc_SCU = 0x00000000;         //��ѹ��Чֵ�Ȳ�Ĵ���
    gs_JbPm.gs_JBA.Soc_SCI1 = 0x00000000;        //������Чֵ�Ȳ�Ĵ���
    gs_JbPm.gs_JBA.Soc_PARAPC = 0x00000000;      //�й����ʶ��β���ֵ
    gs_JbPm.gs_JBA.Soc_PARAQC = 0x00000000;      //�޹����ʶ��β���ֵ
    gs_JbPm.gs_JBA.Soc_PHCCtrl1 = 0x00;          //Iaͨ�� �ǲ�Ĵ���
    
    gs_JbPm.gs_JBB.Rac_MTPARA1 = 0x0a0b0900;      //0x184 Mͨ���رգ������������Լ죬ʹ��ȥֱ��ͨ�˲�����ʹ�ܣ�U/I�����룬�ǲ�   
    gs_JbPm.gs_JBB.Rac_WARTU = 0x00000000;        //ȫ����ѹ��Чֵ�Ȳ�   
    gs_JbPm.gs_JBB.Rac_WARTI = 0x00000000;        //ȫ��������Чֵ�Ȳ�
    gs_JbPm.gs_JBB.Rac_WAPT = 0x00000000;         //ȫ���й����ʱȲ�
    gs_JbPm.gs_JBB.Rac_WWAPT = 0x00000000;        //ȫ���й����ʶ��β���ֵ
    gs_JbPm.gs_JBB.Rac_WWAQT = 0x00000000;       //ȫ���޹����ʶ��β���ֵ
    
    gs_JbPm.ul_PG=1;               //���ʱ���ϵ��
    gs_JbPm.ul_URmG=1;             //��ѹͨ������ϵ��
    gs_JbPm.ul_I1RmG=1;            //����ͨ��1����ϵ��
    
    gs_JbPm.Soc_AnaG=0x03;

   gs_JbPm.u16_csck = Lib_get_csck_int16u_num((uint8*)&gs_JbPm,sizeof(S_JBPM)-2,CHECKWORD);

//    gs_JbPm.ui_JbCRC=do_CRC((uint8*)&gs_JbPm,sizeof(S_JBPM)-2);     // У�������CRC���
}
/*=========================================================================================\n
* @function_name: UpdateRaccoonPara
* @function_file: Raccoon.c
* @����: 
* 
* @����: 
* 
* @����: 
* @return: uint8 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
uint8 UpdateRaccoonPara(void)
{
    uint8 i;
    uint32 *ul_pRacReg;
//    uint32 temp = 0;
    
    ul_pRacReg=(uint32*)&gs_JbPm.gs_JBA.Soc_SCP;   //A��
    for( i=0; i<sizeof(RegAddrSoc)/sizeof(uint16)-3; i++) //дRegAddrSoc��SCP,SCQ,SCU,SCI1��9811
    {
      CLRWDT(); 
      SetMeterCfgACK( ul_pRacReg[i],RegAddrSoc[i]);
    }
    //SetMeterCfgACK( 0,SCQ);
//    //1.28S��ʱ�Ǳ��Ѿ��ӽ�ȥ�ˣ���߲���Ҫ������
//    temp = ReadMeterParaACK( PARAPC);//B���й����ʻ�ӵ����β����Ĵ����У�������������
//    temp += gs_JbPm.gs_JBA.Soc_PARAPC;
   SetMeterCfgACK(gs_JbPm.gs_JBA.Soc_PARAPC, PARAPC);
//    temp = ReadMeterParaACK( PARAQC);//B���޹����ʻ�ӵ����β����Ĵ����У�������������
//    temp += gs_JbPm.gs_JBA.Soc_PARAQC;
//    SetMeterCfgACK(temp, PARAQC);
//    SetMeterCfgACK(0X002FFFFF,PARAPC);
    SetMeterCfgACK((uint32)gs_JbPm.gs_JBA.Soc_PHCCtrl1, PHCCtrl1); //Iaͨ�� �ǲ�Ĵ���
    
    CtrlADC0 = gs_JbPm.Soc_AnaG;                         //����ADC����
    if((gs_JbPm.Soc_AnaG&0x07)==0x07)                    //IA����32���Ļ�������IA ƫ�õ���
    {
        CtrlADC4|=BIT4;             //����ƫ�õ���
    }else
    {
        CtrlADC4&=~BIT4;            //����ƫ�õ���
    }
     
     ul_pRacReg=(uint32*)&gs_JbPm.Rac_ANCtrl0;   //B��
 //    uint8 temp2;
 //   BE_ReadP(0x2A,&temp2,1);
  //  gs_JbPm.Rac_ANCtrl0 =((((uint16)temp2)<<8)&0xff00)|0x1D2000C2;    //0x0ff0     0f00 
    //gs_JbPm.Rac_ANCtrl0 = 0x1D2004C2;       
    //  0xC2,0x64,0x20,0x1D,//ģ����ƼĴ���0

    //Ĭ�϶�ֵ
 //   gs_JbPm.Rac_ANCtrl0 = 0x1D2064C2;

    for(i=0;i<sizeof(RegAddrRac)/sizeof(uint16) ;i++) //дRegAddrRac�мĴ�����9250
    {
      CLRWDT(); 
      if(i  < 5)   //ANCTRL0,ANCTRL1,ANCTRL2,BPFPARA,MTPARA0
      {
        WriteRaccoon(ul_pRacReg[i],RegAddrRac[i], BPhy);  
        CLRWDT();  
      }
      if(i == 5)
      {
        ul_pRacReg = (uint32*)&gs_JbPm.gs_JBB.Rac_MTPARA1;
      }
      if(i >= 5)
      {
        WriteRaccoon(ul_pRacReg[i-5],RegAddrRac[i], BPhy);//MTPARA1,WARTU,WARTI,WAPT,WWAPT,WWAQT  
      }
    }
    WriteRaccoon(gs_JbPm.gs_JBB.Rac_WAPT,RegWAPT, BPhy);    //B���޹��Ȳ�ֵ
    
  return TRUE;
}
/*=========================================================================================\n
* @function_name: InitRaccoon
* @function_file: Raccoon.c
* @����: ��ʼ��9260F
* 
* @����: 
* @����: 
* @����:   lwb (2013-07-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void InitRaccoon(void)
{
    Init_RacCtrl();   // ͨѶ 
    Init_9260F_EMU();    // �Ĵ������ó�ʼ��//
}



/*=========================================================================================\n
* @function_name: InitRaccoon
* @function_file: Raccoon.c
* @����:  ����9260F �Ĵ���ֵ
* 
* @����: 
* @����: 
* @����:   zwk (2016-05-10)
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void  Init_9260F_EMU(void)
{
    

	 //ANCTRL0,ANCTRL1,ANCTRL2,BPFPARA,MTPARA0   //  //
        WriteRaccoon(0x1D000483 ,RegANCtrl0, BPhy);   // ģ�ⲻ����1D0004C3   0x185 ����I:32��U:1��M:4 �¶�ϵ����ȫ�ֵ���ƫ��
		WriteRaccoon(0x30000000 ,RegANCtrl1, BPhy);    //0x186  
    
        WriteRaccoon(0x00000000 ,RegANCtrl2, BPhy);    //0x187  
        WriteRaccoon(0x811D2BA7 ,RegBPFPARA, BPhy);    //0x0125 ��ͨ�˲���ϵ�� 0x811D2BA7
        WriteRaccoon(0x00000000 ,RegMTPARA0, BPhy);  //0x183 CF����ʱ��3.2MHz ʹ��CF����������ۼӣ���Ǳ����������Դ��ƽ��ȫ���й�
//B09A9  0x000b0900   0x000B09A9  ��������//  x2 U  x4 I  
//	    WriteRaccoon(0x000b09A9 ,RegMTPARA1, BPhy);  //0x184 Mͨ���رգ������������Լ죬ʹ��ȥֱ��ͨ�˲�����ʹ�ܣ�U/I�����룬�ǲ�  
	    WriteRaccoon(0x000B00D9 ,RegMTPARA1, BPhy);  //0x184 Mͨ���رգ������������Լ죬ʹ��ȥֱ��ͨ�˲�����ʹ�ܣ�U/I�����룬�ǲ�  

        CLRWDT(); 



	//MTPARA1,WARTU,WARTI,WAPT,WWAPT,WWAQT  
        WriteRaccoon(0x00000000 ,RegWARTU, BPhy);    // //ȫ����ѹ��Чֵ�Ȳ�Ĵ���
        WriteRaccoon(0x00000000 ,RegWARTI, BPhy);     //ȫ��������Чֵ�Ȳ�Ĵ��� 
        WriteRaccoon(0x00000000 ,RegWAPT, BPhy);      //ȫ���й����ʱȲ�Ĵ���
        WriteRaccoon(0x00000000 ,RegWWAPT, BPhy);   //ȫ���й����ʶ��β���ֵ�Ĵ���
        WriteRaccoon(0x00000000 ,RegWWAQT, BPhy);  // //ȫ����ѹ��Чֵ�Ȳ�Ĵ���
	 CLRWDT(); 

  //	WriteRaccoon(gs_JbPm.gs_JBB.Rac_WAPT,RegWAPT, BPhy);    //B���޹��Ȳ�ֵ

  

}

/*=========================================================================================\n
* @function_name: Accumulate_abc
* @function_file: EnergyBottom.c
* @����  ���๦���ۼ�
* 
* @����: 
* @����: 
* @����:   
* @��ע: 
*-------------------------------------------------------------------------------------------
* @�޸���:  
* @�޸�����: 
===========================================================================================*/
void Accumulate_abc(void)
{
    /*static uint8 times=0;
    uint32 tmp_egyth;
    times++;
    if(times >=4)     //40ms�ۼ�һ�� ���ں��������Ѿ�/1024��,������������1024/(6400/25)=4�� 
    {
        times = 0;      
         //�����й�����
        tmp_egyth =  (gs_JbPm.Com_EGYTH*4)>>8;   //A��  �����ʺͷ�����ȡ����ֵ֮���ۼ�
        gs_power.ul_power_all[0] += (gs_power.ul_power[0] >> 8);     //��ֹ�ۼ���� ����
        gs_power.ui_power_rem[0] += (gs_power.ul_power[0] & 0x000000ff);
        if(gs_power.ui_power_rem[0] > 255)
        {
            gs_power.ul_power_all[0]++;
            gs_power.ui_power_rem[0] -= 255;
        }

        if(gs_power.ul_power_all[0] >= tmp_egyth)
        {
            gs_power.ul_power_all[0] -= tmp_egyth;
            EnyB_IntCF(0,active);
        }
        
        tmp_egyth =  (gs_JbPm.Com_EGYTH*4)>>8;   //B��  �����ʺͷ�����ȡ����ֵ֮���ۼ�    
        gs_power.ul_power_all[1] += (gs_power.ul_power[1] >> 8);     //��ֹ�ۼ���� ����
        gs_power.ui_power_rem[1] += (gs_power.ul_power[1] & 0x000000ff);
        if(gs_power.ui_power_rem[1] > 255)
        {
            gs_power.ul_power_all[1]++;
            gs_power.ui_power_rem[1] -= 255;
        }

        if(gs_power.ul_power_all[1] >= tmp_egyth)
        {
            gs_power.ul_power_all[1] -= tmp_egyth;
            EnyB_IntCF(1,active);
        }

         //�����޹�����
        tmp_egyth =  (gs_JbPm.Com_REEGYTH*4)>>8; //A��   �����ʺͷ�����ȡ����ֵ֮���ۼ�
        gs_power.ul_Npower_all[0] += (gs_power.ul_Npower[0] >> 8);
        gs_power.ui_Npower_rem[0] += (gs_power.ul_Npower[0] & 0x000000ff);
        if(gs_power.ui_Npower_rem[0] > 255)
        {
            gs_power.ul_Npower_all[0]++;
            gs_power.ui_Npower_rem[0] -= 255;
        }

        if(gs_power.ul_Npower_all[0] >= tmp_egyth)
        {
            gs_power.ul_Npower_all[0] -= tmp_egyth;
            EnyB_IntCF(0,reactive);
        }
        
        tmp_egyth =  (gs_JbPm.Com_REEGYTH*4)>>8; //B��  �����ʺͷ�����ȡ����ֵ֮���ۼ�   
        gs_power.ul_Npower_all[1] += (gs_power.ul_Npower[1] >> 8);
        gs_power.ui_Npower_rem[1] += (gs_power.ul_Npower[1] & 0x000000ff);
        if(gs_power.ui_Npower_rem[1] > 255)
        {
            gs_power.ul_Npower_all[1]++;
            gs_power.ui_Npower_rem[1] -= 255;
        }

        if(gs_power.ul_Npower_all[1] >= tmp_egyth)
        {
            gs_power.ul_Npower_all[1] -= tmp_egyth;
            EnyB_IntCF(1,reactive);
        }
    } */  
}
