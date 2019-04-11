/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name: Lib_mySpring.c
**Author: maji
**date: 
**description: c code for general library function 
**note: memer type  DJ04A
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1		2014-07-08  		0.1  			sosomj   			1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


/****************************************************************
Function:   	INT8U Lib_Cmp_TwoArry(INT8U* s1, INT8U* s2, INT8U len) 
Description: 	����ͬ�����ȵ��ַ����Ƚϴ�С 
Input:		INT8U* s1   �Ƚ�����1�׵�ַ
			INT8U* s2   �Ƚ�����2�׵�ַ  
			INT8U len    ���鳤��
Parameters:     
Returns:		s1 > s2 ʱ,OUT = 1
			s1 < s2 ʱ,OUT = 255
			s1 == s2 ʱ ,OUT = 0
Others:	
****************************************************************/
INT8U Lib_Cmp_TwoArry(INT8U* s1, INT8U* s2, INT8U len) 
{
    INT8U i;
    
    s1=s1+(char)(len-1);
    s2=s2+(char)(len-1);
    for (i=0; i<len;i++)
    {
        if((*s1) > (*s2))  return (1);
        if((*s1) < (*s2))  return (255);
        s1--;
        s2--;
    }
    return (0);
}


/****************************************************************
Function:   	void  Lib_Rev_Copy_Arry(INT8U* s1, INT8U len) 
Description: 	ʵ����������˳��ߵ� 
			such as   A0A1A2A3 ---> A3A2A1A0;
Input:		INT8U* s1   ����1�׵�ַ
			INT8U len    ���鳤��
Parameters:     
Returns:		no
Others:	
****************************************************************/
void  Lib_Rev_Copy_Arry(INT8U* s1, INT8U len) 
{
    INT8U tmpData;
    INT8U i = len/2;

    for(i=0; i<len/2;i++)
    {
        tmpData=  *(s1+i) ;
         *(s1+i) = *(s1+len-1-i);
         *(s1+len-1-i) = tmpData;   
    }
}


/****************************************************************
Function:   	void  Lib_Copy_Str_TwoArry(INT8U *s1, INT8U *s2, INT8U len) 
Description: 	��S2�ַ�������ԭ��˳�򿽱���S1���ݴ�
Input:		INT8U* s1   Ŀ�������׵�ַ
			INT8U* s2   Դ�����׵�ַ  
			INT8U len    ���鳤��
Parameters:     
Returns:		NO
Others:	
****************************************************************/
void  Lib_Copy_Str_TwoArry(INT8U *s1, INT8U *s2, INT16U len) 
{
    while(len > 0)
    {
        *s1 =*s2;
        s1++;
        s2++;
        len--;
    }
}


/****************************************************************
Function:   	void  Lib_Set_String(INT8U *s1, INT8U tmp, INT8U len) 
Description: 	��s1��ַ��ʼ��len����������Ϊtmpֵ
Input:		INT8U* s1   Ŀ�������׵�ַ
			INT8U tmp   ��Ҫ�趨����ֵ  
			INT8U len    ���鳤��
Parameters:     
Returns:		NO
Others:	
****************************************************************/
void  Lib_Set_String(INT8U *s1, INT8U tmp, INT16U len) 
{
    while(len > 0)
    {
        *s1 =tmp;
        s1++;
        len--;
//		CLRWDT();
    }
}


/****************************************************************
Function:   	void  Lib_Clr_String_U16(INT16U *s1,  INT8U len) 
Description: 	���������ַ���U16��ʽ����������
Input:		INT16U *s1   Ŀ�������׵�ַ��16λ����ָ��
			INT8U len    ���鳤��
Parameters:     
Returns:		NO
Others:	
****************************************************************/
void  Lib_Clr_String_U16(INT16U *s1,  INT16U len) 
{
    while(len > 0)
    {
        *s1 =0;
        s1++;
        len--;
    }
    
}

/****************************************************************
Function:   	void  Lib_Clr_String(INT8U *s1,  INT8U len) 
Description: 	���������ַ���U16��ʽ����������//
Input:		INT8U *s1   Ŀ�������׵�ַ��8λ����ָ��
			INT8U len    ���鳤��
Parameters:     
Returns:		NO
Others:	
****************************************************************/
void  Lib_Clr_String(INT8U *s1,  INT16U len) 
{
    while(len > 0)
    {
        *s1 =0;
        s1++;
        len--;
    }
    
}


//---------------------------------------------------------------------//
//����:   ��������ʼ��ַ��ʼ��F��ֹ�ĳ���//
//     0000F     return 4
//	*Addr ������ʼ��ַ��num���ݳ���
//---------------------------------------------------------------------//
INT16U Lib_get_data_0xFF_asclen(INT8U *Addr,INT16U numbyte)
{
	INT16U i;
	INT16U asclen=0;
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


//---------------------------------------------------------------------//
//����: ���ֽڵĵ�λ�Ƶ����ֽڵĸ�λ  �൱�ڴ�����ư���ֽ�
//   addrΪ��ָ�뿪ʼ��numbyte���ֽ����ư���ֽ�
//	*addr ������ʼ��ַ��numbyte���ݳ���  
//��� 0x12 0x34 �� 0x01 0x23    ���鳤2���ֽ�, 4�������¸��ֽ�Ų��ǰ��0(ʹ��ʱע�����鲻ҪԽ��)    
//---------------------------------------------------------------------//
void Lib_MVHalfByte_R(INT8U *addr,INT8U numbyte)
{
	INT8U i;


	for(i=numbyte-1;i>0;i--)
	{
		addr[i] = ((addr[i]>>4) & 0x0f) + ((addr[i-1]<<4) & 0xf0);
	}
	addr[0] = (addr[0]>>4) & 0x0f;

}

//---------------------------------------------------------------------//
//����: ���ֽڵĸ�λ�Ƶ����ֽڵĵ�λ  �൱�ڴ�����ư���ֽ�
//   addrΪ��ָ�뿪ʼ��numbyte���ֽ����ư���ֽ�
//	*addr ������ʼ��ַ��numbyte���ݳ���  
//��� 0x12 0x34 �� 0x23 0x40    ���鳤2���ֽڣ���0(ʹ��ʱע�����鲻ҪԽ��)    
//---------------------------------------------------------------------//
void Lib_MVHalfByte_L(INT8U *addr,INT8U numbyte)
{
	INT8U i;


	for(i=0;i<numbyte-1;i++)
	{
		addr[i] = ((addr[i+1]>>4) & 0x0f) + ((addr[i]<<4) & 0xf0);
	}
	addr[numbyte-1] = (addr[numbyte-1]<<4) & 0Xf0;

}



// //����Flash���ݵ�RAM�� 
void Lib_FCpyTMem(uxdata8 *des,const INT8U code *src,INT8U len)
{
    while(len--)
    {
        *(des++)=*(src++);
    }
}

/***************************************************************
*    END
****************************************************************/


