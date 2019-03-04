/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_KEY_IO.c
**Author		: maji
**date			: 2016-04-20 
**description	: MCU KEYL类似内核模块的硬件驱动C代码
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#include <MyIncludes_H.h>


         


/*******************************************************************************************
** 函数名称: Judge_ERR_ADJ_key
** 函数描述: 判断硬件校表短接点电平状态
** 输入参数: =TRUE 短接有效；=FALSE 未短接
** 输出参数: 无 
** 说明    : 短接电平为0，未短接为1
*******************************************************************************************/
uint8 Judge_ERR_ADJ_key(void)
{
	uint8 i;


	for(i=0;i<10;i++)
	{
		if(KEY_READ_ERR_ADJ != FALSE)   break;
	}

	if(i>7)
	{
		return (TRUE); 
	}
	else
	{
		return(FALSE);
	}
                       
}                  
/*******************************************************************************************
** 函数名称: 
** 函数描述: 
** 输入参数: 
** 输出参数: 
** 说明    :
*******************************************************************************************/
uint8 Judge_PG_DN_key(void)
{
	uint8 i;


	for(i=0;i<10;i++)
	{
		if((P1ID&BIT4) != FALSE)   break;
	}

	if(i>7)
	{
		return (TRUE); 
	}
	else
	{
		return(FALSE);
	}
                       
}                  
/*******************************************************************************************
** 函数名称: 
** 函数描述: 
** 输入参数: 
** 输出参数: 
** 说明    :
*******************************************************************************************/
uint8 Judge_MD_CLEAR_key(void)
{
	uint8 i;


	for(i=0;i<10;i++)
	{
		if((P9DI&BIT1) != FALSE)   break;
	}

	if(i>7)
	{
		return (TRUE); 
	}
	else
	{
		return(FALSE);
	}
                       
}    

/*******************************************************************************************
** 函数名称: 
** 函数描述: 
** 输入参数: 
** 输出参数: 
** 说明    :
*******************************************************************************************/
uint8 Judge_MAGN_CHK_key(void)
{
	uint8 i;


	for(i=0;i<10;i++)
	{
		if((P1ID&BIT3) != FALSE)   break;
	}

	if(i>7)
	{
		return (TRUE); 
	}
	else
	{
		return(FALSE);
	}
                       
}                  


/*******************************************************************************************
** 函数名称:
** 函数描述: 
** 输入参数:
** 输出参数:
** 说明    :
*******************************************************************************************/
uint8 Judge_ERR_COVER_key(void)
{
	uint8 i;
  

	for(i=0;i<10;i++)
	{
		if(KEY_READ_ERR_COVER != TRUE)   break;
	}

	if(i>7)
	{
		return (TRUE); 
	}
	else
	{
		return(FALSE);
	}
                       
}     

/*******************************************************************************************
**    END
*******************************************************************************************/
