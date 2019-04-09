/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name        : MyIncludes_H.H
**Author      : maji
**date        : 2016-04-18
**description : define for  all 
**note		  : ͨ��
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef 	_MYINCLUDES_H
#define	_MYINCLUDES_H


#define MCU_V98XX   0			
#define MCU_G80F92X 10
#define MCU_TYPE    MCU_V98XX   // оƬѡ�� MCU_G80F92X   ������Ĭ��Ϊ��оƬ//
#define	DEBUG_TEST 0 
#define D_1P3W 1
#define D_1P2W 0
#define meter_type D_1P2W
/***********************************************************
*     �����ļ�����
************************************************************/
//keil �Դ��ļ�����  //
#include <math.h>
#include <stdlib.h>
#include <intrins.h>	//KEIL ��NOP�Ͳ���λ�����ĺ����������ļ�  //
#include <string.h>

//MCU ��Ҫ�Ĳ���  //
#include <dw8051.h>
#include <LeoPartReg.h>


//���岿��  //
#include <SYS_typedef_H.h>
#include <SYS_Config_H.h>
#include <SYS_DATA_MACRO_H.h>
#include <SYS_MCU_MACRO_H.h>
#include <SYS_IOdefine_H.h>        //MCU  IO �ڲ��������ļ�  //


// ������д������벿�� //
#include <HardDrv_H.h>
#include <Lib_myLib_H.h>
#include <ApiProtocol_H.h>
#include <Api_plat_H.h>
#include <Api_user_H.h>
#include <System_H.h>
#endif



/***************************************************************
*    END
****************************************************************/


