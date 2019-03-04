/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name			: Drv_KEY_IO_H.H
**Author		: maji
**date			: 2016-04-20 
**description	: MCU core内核模块的硬件驱动C代码的声明文件
**note			: V9811A ，MERTER FOR DL03C
**--------------------Version History -------------------------------------
** NO. Date         Ver      By         Description 
**==============================================================
** 1   2016-04-26   v01.00   sosomj     1. frist version                             
**
**==============================================================
*/

#ifndef _Drv_KEY_IO_H
#define _Drv_KEY_IO_H

/***************************************************************
*    1     立即数预定义   
****************************************************************/

 

/***************************************************************
*    2     数据结构声明     
****************************************************************/



/***************************************************************
*    3    变量声明       
****************************************************************/



/***************************************************************
*    4     函数声明        
****************************************************************/
extern uint8 Judge_PRO_key(void);
extern uint8 Judge_DIS_key(void);
extern uint8 Judge_ERR_ADJ_key(void);
extern uint8 Judge_ERR_COVER_key(void);
         

/***************************************************************
*    5     其他内容，例如伪指令等       
****************************************************************/


/***************************************************************
*    END
****************************************************************/
#endif
