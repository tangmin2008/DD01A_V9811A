/*
*****************Copyright (c)*************************************
**      Hangzhou Xili Watthour Meter Manufacture Co., Ltd. 
**--------------file info--------------------------------------------
**name:         API_Relay_H.H
**Author:       maji
**date: 
**description:  DEFINE code for 继电器控制应用层
**note:         meter type  DI03B
**--------------------Version History -------------------------------------
** NO.	Date        		Ver    		By     			Description 
**==============================================================
** 1    2017-09-15 		0.1  		sosomj   		1. frist version                             
**
**==============================================================
*/

#ifndef _API_Relay_H
#define _API_Relay_H

//******************************************************************
//              1     立即数预定义                                            //                      
//******************************************************************
#define CMD_RELAY_ON        0XA3  //0XA3代表拉闸 //
#define CMD_RELAY_OFF		0     //!=0XA3代表合闸//
#define TMR_OPERA_RELAY     7     //默认继电器驱动操作电平70ms //

//******************************************************************
//               2     数据结构声明                                                 //
//******************************************************************
// 继电器管理控制变量结构体定义// 
typedef struct 
{
    INT8U    u8_CMD;      //继电器命令字//
    INT8U    u8_STA;      //继电器拉合闸操作字//
    INT8U    u8_tmr_opera;      //继电器硬件驱动操作计数器 //
}RELAY_MANAGE_VAR_TYPE;


//******************************************************************
//              3    变量声明                                                             //
//******************************************************************
extern RELAY_MANAGE_VAR_TYPE gs_relay_manage_var;

//******************************************************************
//             4     函数声明                                                              //
//******************************************************************
extern void Drv_relay_run_check(void);
extern void Drv_relay_powerup_check(void);
extern INT8U Drv_relay_COMM_CMD_operating(INT8U *ptr_cmd);

//******************************************************************
//             5     其他内容，例如伪指令等              //
//******************************************************************





#endif
/*******************************************************************
*
*  end of file                                    
*
*******************************************************************/

