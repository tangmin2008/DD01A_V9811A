/*******************************************************************************
*ģ����   : LoadProfile.c
*����ļ� : 
*�ļ����� : ���ɼ�¼����ģ��
*����     : �з�����
*��д���� :
-------------------------------------------------------------------------------
*�޸ļ�¼ : 
*�� ��        �汾     �޸���              �޸�����
*YYYY/MM/DD   X.Y      <���߻��޸�����>    <�޸�����>
*******************************************************************************/
#include <MyIncludes_H.h>


#if  FUNC_LoadPro   //���ɼ�¼ʹ�ܿ���

//----------------------------------------------------------------ȫ�ֱ���������
LPREAD	    LPRd;            //���ɼ�¼�洢��Ϣ
LPRUNPARA   LPRunPa;         //���ɼ�¼����
LPReadPRO  LPReadPara;
//-------------------------------------------------------------ȫ�ֱ���������end

/*********************************************************************
ԭ�ͣ�void LoadProRst(void)
���ܣ���λ���ɱ�
��ע���ޡ�
*********************************************************************/
void LoadProRst(void)
{
    Lib_Set_String( (uint8*)&LPRunPa, 0x00, (sizeof(LPRUNPARA)) );
    mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (uint8*)&LPRunPa, LP_RunPara_Len, MEM_E2P1);// ���ɼ�¼ָ�롢��¼����  !!
    
    Lib_Set_String( &LPReadPara.Original_Ptr, 0, 5) ;		




}

/*********************************************************************
ԭ�ͣ�uchar ReadLPRunPa(void)
���ܣ���ȡ���ɼ�¼����
��ע���ޡ�
*********************************************************************/
uchar ReadLPRunPa(void)
{
    return TRUE;
}

/*******************************************************************************
		���ͨѶ
*********************************************************************************/
/*********************************************************************
ԭ�ͣ�uchar LPRec_Read(void)
���ܣ������ɱ�����  -- ��ID
��ע���ޡ�
*********************************************************************/
uchar LPRec_Read_1(INT8U *dst,INT16U idNo)
{
	uchar  RecLen;
	INT8U Buffer[20];
	INT32U  LPAddr;
	INT16U ptr;


	if(LPRunPa.Ptr[0] >= idNo)
	{
		ptr = LPRunPa.Ptr[0] - idNo;	// ǰ�ڼ���
	}
	else
	{
	    ptr = LPRd.MaxRecNum - (idNo - LPRunPa.Ptr[0] );
	}
	
	RecLen = 18;	//һ��ͨ�����ݳ���
	LPAddr = (ADR_BLOCK180_LOAD_START_E2P+ (INT32U)ptr*RecLen);
	mem_read( &Buffer[0],LPAddr, RecLen, MEM_E2P1);
	Lib_BCD_AscA(dst,(char*)&Buffer[0],RecLen); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+5],4); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+14],2); 
//	Lib_BCD_AscA(dst,(char*)&Buffer[0+21],3);
//  Lib_BCD_AscA(dst,(char*)&Buffer[0+29],RecLen);

  
  return   (RecLen);
}


/*********************************************************************
ԭ�ͣ�uchar LPRec_Read_Blank(void)
���ܣ������ɱ�����  -- ��ID
��ע���ޡ�
*********************************************************************/
INT16U LPRec_Read_Block(INT8U *dst,INT16U StartARESS,INT16U EndARESS)
{
	uchar Ch,RecLen;
	INT8U Buffer[200];
	INT32U  LPAddr,LPAddr2;
	INT16U  HGAddr,LWAddr;
	INT16U ComLenth,ptr;
	INT16U ComNo;
	ST_U16_U08 TempLen;

	ptr =0; ComLenth =0;
	CLRWDT(); 
  if((LPReadPara.Original_FrameNum==0)||( LPReadPara.Original_FrameNum < StartARESS ))return(FALSE); 
	else LWAddr = StartARESS;
	
	if(EndARESS >= READLP_MAX_LEN) HGAddr = READLP_MAX_LEN-1;
	else	HGAddr = EndARESS;
	
	TempLen.u16 = HGAddr - LWAddr + 1;	
	
	ptr = (LPReadPara.Original_Ptr+ READLP_MAX_LEN - LWAddr)%READLP_MAX_LEN ;	
	
	RecLen = 18;	//һ��ͨ�����ݳ���
	
	LPAddr = (ADR_BLOCK180_LOAD_START_E2P);
  CLRWDT(); 
	for( Ch=0; Ch<BLOCK_MAX_LEN; Ch++)
	{
    LPAddr2=LPAddr+((ptr+ READLP_MAX_LEN-Ch)%READLP_MAX_LEN)*RecLen;
		mem_read( &Buffer[0+Ch*RecLen],LPAddr2 , RecLen, MEM_E2P1);	  

	}

	if(( TempLen.u16 >= ( LPReadPara.Original_FrameNum-StartARESS ))&&( LPReadPara.Original_FrameNum <READLP_MAX_LEN))
		TempLen.u16 = LPReadPara.Original_FrameNum-StartARESS;
	
	if( TempLen.u16 <= BLOCK_MAX_LEN )
		ComNo = TempLen.u16;
	else 
		ComNo = BLOCK_MAX_LEN;
	ptr =0; 
	
	
	CLRWDT(); 
	for( Ch=0; Ch < ComNo; Ch++)
	{
		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[Ch*RecLen],RecLen); 
//	   ptr += 10;			
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[5+Ch*RecLen],4); 
//	   ptr += 8;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[14+Ch*RecLen],2); 
//	   ptr += 4;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[21+Ch*RecLen],3); 
//	   ptr += 6;
//		 Lib_BCD_AscA(dst+ptr,(char*)&Buffer[29+Ch*RecLen],4); 
	   ptr += 36;
     ComLenth += 36;			
	}
	 
  return  (ComLenth);
}

/*********************************************************************
ԭ�ͣ�uchar Get_LoadRegNo(uchar Code)
���ܣ��õ�LoadReg�����±�
��ע��ͨ���Ŵ�0��ʼ
*********************************************************************/
//uchar Get_LoadRegNo(uchar Code)
//{
//    uchar i;
//    
//    for(i=0; i<CLPRegNum; i++)
//    {
//	if(Code == LPReg[i].Code)
//	    return i;
//    }
//    return FALSE_ERR;
//}

/*********************************************************************
ԭ�ͣ�char Load_Rec(char Ch)
���ܣ��ֶ���¼��������
��ע��ͨ���Ŵ�0��ʼ
*********************************************************************/

//char Load_Rec(char Ch)//һ��ͨ��
//{
//    INT32U  LPAddr;
//    INT8U   Len;  //,CTmp ,RegNo i,
//    INT8U Buffer[20];
 

//    if(Ch >= MaxLPCh) return FALSE;
//    if( FALSE == ReadLPRunPa() ) return FALSE;
    /////////////////////////////////////////////////////////////////////////////////////////////	
    ///////////////////ȡһ��ͨ��һ�μ�¼����
    /****ͨ�����ݺ�ͨѶ˳���෴
    ͨѶ˳��:������ʱ�� ��REG1����~�ͣ���REG2����~�ͣ�����
    �洢˳��������ʱ�� ��REG1����~�ͣ���REG2����~�ͣ�����
    *************************************/
//    Lib_Set_String( (uchar*)&Buffer[0], 0x00, 20 );
//    Len = 0;
//	 Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &gs_CurDateTime.Year, 5 );// ȡ��ǰʱ��
//    Len += 5;
   //////���ɼ�¼�й��ܵ��� ��ȡ���� 4�ֽڷ���   BUFF 5   ////////  4�ֽ�XXXXXX.XX  kwh  BCD 
//     api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
//     Len += 4;
   /*     ���ɼ�¼���� 3�ֽڷ���   BUFF 5   //////// �����ʽΪXX.XXXX kw   BCD
      Lib_long_bcd4(&Buffer[6],gs_md_var.min_var[0].val_min);          //  16����ת10����// 		
	  Buffer[Len] =  0x00 ;
	  Buffer[Len+1]= Buffer[7];
      Buffer[Len+2]= Buffer[8]; 
      Buffer[Len+3]= Buffer[9]; 
      Len += 4;
      */
    //////��һ��ͨ��һ�μ�¼����//////////////
    //��ͨ���洢ָ�������+1
//    LPRunPa.Num[Ch]++;
//    if( LPRunPa.Num[Ch] > LPRd.MaxRecNum ) LPRunPa.Num[Ch] = LPRd.MaxRecNum;
//    LPRunPa.Ptr[Ch]++;
//    if( LPRunPa.Ptr[Ch] > LPRd.MaxRecNum ) LPRunPa.Ptr[Ch] = 0;


//    LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//    LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;

//	if((LPAddr +Len) > (ADR_BLOCK180_LOAD_START_E2P+LP_MAX_LEN*9))		// ������������ֽ���	54000   
//	{
//		LPRunPa.Ptr[Ch] = 0;
//		LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//		LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
//	}
	    
//    mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_E2P1);

//    
//    return TRUE;
//}

char Load_Rec(char Ch)//һ��ͨ��
{
	INT32U  LPAddr;
	INT8U   Len;  //,CTmp ,RegNo i,
	INT8U Buffer[20];


	if(Ch >= MaxLPCh) return FALSE;
	//    if( FALSE == ReadLPRunPa() ) return FALSE;
	/////////////////////////////////////////////////////////////////////////////////////////////	
	///////////////////ȡһ��ͨ��һ�μ�¼����
	/****ͨ�����ݺ�ͨѶ˳���෴
	ͨѶ˳��:������ʱ�� ��REG1����~�ͣ���REG2����~�ͣ�����
	�洢˳��������ʱ�� ��REG1����~�ͣ���REG2����~�ͣ�����
	*************************************/
	Lib_Set_String( (uchar*)&Buffer[0], 0x00, 20 );
	Len = 0;
	Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &gs_CurDateTime.Year, 5 );// ȡ��ǰʱ��
	Len += 5;
//	switch (Ch)
//	{
//		case 0:
//			//////���ɼ�¼�й��ܵ��� ��ȡ���� 4�ֽڷ���   BUFF 5   ////////  4�ֽ�XXXXXX.XX  kwh  BCD 
			api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
			Len += 4;
//			break;
//		case 1:
			//////���ɼ�¼��ǰ��ѹ ��ȡ��ѹ		2�ֽ�   xxx.x V BCD	
			//api_get_energy_LCD(&gs_energy_user_data.us_val[0][0].buf[0], 0x62,&Buffer[Len]);
			Lib_long_bcd4((char*)&Buffer[Len],gs_measure_var_data.gs_really[PHASE_TT].dw_u_val.u32);
			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], (char*)&Buffer[Len+2], 2 );
			Len += 2;
//			break;
//		case 2:
			//////���ɼ�¼��ǰ���� ��ȡ����  	3�ֽ�XXX.XXX A BCD
//			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], &(gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32), 3 );
			Lib_long_bcd4((char*)&Buffer[Len],gs_measure_var_data.gs_really[PHASE_TT].dw_i_val.u32);
			Lib_Copy_Str_TwoArry( (char*)&Buffer[Len], (char*)&Buffer[Len+1], 3 );
			Len += 3;
//			break;
//		case 3:
			//////���ɼ�¼��ǰ���� ��ȡ���� 	4�ֽ�XXXXXX.XX  kwh  ASCII
			mem_read((char*)&Buffer[Len], ADR_BLOCK01_MMD_L0_E2P, 4, MEM_E2P1);
			Len += 4;
//			break;
//		default:
//			break;
		
		
	

	/*     ���ɼ�¼���� 3�ֽڷ���   BUFF 5   //////// �����ʽΪXX.XXXX kw   BCD
	Lib_long_bcd4(&Buffer[6],gs_md_var.min_var[0].val_min);          //  16����ת10����// 		
	Buffer[Len] =  0x00 ;
	Buffer[Len+1]= Buffer[7];
	Buffer[Len+2]= Buffer[8]; 
	Buffer[Len+3]= Buffer[9]; 
	Len += 4;
	*/
	//////��һ��ͨ��һ�μ�¼����//////////////
	//��ͨ���洢ָ�������+1
	LPRunPa.Num[Ch]++;
	if( LPRunPa.Num[Ch] > LPRd.MaxRecNum ) LPRunPa.Num[Ch] = LPRd.MaxRecNum;
	LPRunPa.Ptr[Ch]++;
	if( LPRunPa.Ptr[Ch] >= LPRd.MaxRecNum ) LPRunPa.Ptr[Ch] = 0;


	LPAddr = ADR_BLOCK180_LOAD_START_E2P;
	LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
  
	mem_db_write( ADR_METER_PARAM1_LOAD_PTR, (uint8*)&LPRunPa, LP_RunPara_Len, MEM_E2P1);// ���ɼ�¼ָ�롢��¼����  !!
//	if((LPAddr +Len) > (ADR_BLOCK180_LOAD_START_E2P+LP_MAX_LEN*9))		// ������������ֽ���	54000   
//	{
//		LPRunPa.Ptr[Ch] = 0;
//		LPAddr = ADR_BLOCK180_LOAD_START_E2P;
//		LPAddr += (INT32U)LPRunPa.Ptr[Ch]*Len;
//	}
	    
//	mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_DMFLASH);
	mem_write( (LPAddr),(char*)&Buffer[0], Len, MEM_E2P1);

	return TRUE;
}


/*********************************************************************
ԭ�ͣ�void LoadPro_Rec(void)
���ܣ��������ݼ�¼
��ע��
*********************************************************************/
void LoadPro_Rec(void)
{
	INT8U Ch;
	INT16U Addr;
	INT16U TimeInv;
	INT8U flag;
	INT8U Buffer[2];
	
	for( Ch=0; Ch<MaxLPCh; Ch++ ) 
	{
		Addr = ADR_METER_PARAM1_load_lptime_inv_var + 2*Ch;
		//��ȡ��ͨ����¼���ʱ��
		mem_read( (char*)&Buffer[0], (int)Addr, 2, MEM_E2P1 );
		TimeInv = Lib_bcd2_word((uchar *)&Buffer[0]);
		if( TimeInv != 0 ) 
		{
			flag = 0;
			if ( TimeInv <= 60 )      //Сʱ���㣬�޶�4Сʱ		 
			{
#ifdef _DEBUGE_FUNCTION
				Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Second); 
				if(!(Buffer[0]%TimeInv)) flag = 1;
				// Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Minute); 
				// if(!(Buffer[0]%TimeInv)) flag = 1;
#endif		
				Buffer[0] = Lib_bcd_byte(gs_CurDateTime.Minute); 
				if(!(Buffer[0]%TimeInv)) flag = 1;
			}	
			if(flag)
			{
				Load_Rec(Ch);
			}
		}
	}
}



/*********************************************************************
ԭ�ͣ�void LoadPro_Rec(void)
���ܣ����������ϵ��ʼ��
��ע��ͨѶ����Ҫ���ã�ע���ȱ��渺�ɼ�¼����
*********************************************************************/
void Init_LoadPro_param(void)
{

	// �ǵ���һ�θ��ɼ�¼�����ı���
	mem_read((char*)&LPRunPa, ADR_METER_PARAM1_LOAD_PTR, LP_RunPara_Len, MEM_E2P1);              
	//��ȡ�Ĵ�������
	mem_read((char*)&LPRd.RegNum, ADR_METER_PARAM1_load_egcode_var, 1, MEM_E2P1);
	if( ( LPRd.RegNum < 1 ) || ( LPRd.RegNum > MaxLPReg ) ) 
	{
		LPRd.RegNum = 1;
	}
	
	//ȡ����¼����  
	LPRd.MaxRecNum =  LP_MAX_LEN ;   // ���60�죬ÿ4Сʱһ����¼������ ʱ��+����   YYMMDDhhmm+[XXXXXX.XX]+[XX.XXXX]

	mem_read(&LPRd.loadp_reg_code[0],ADR_METER_PARAM1_load_egcode_var, 10, MEM_E2P1);	// ���Ĵ�������

#if DEBUG_TEST

#endif

}


#endif  