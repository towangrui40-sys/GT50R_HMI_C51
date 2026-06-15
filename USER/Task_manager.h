//############################################################
// FILE:  Task_manager_H
// Created on: 2019年7月8日
// Author: XY
// summary: Header file  and definition
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32F4电机控制开发板
//硕历达电子  
//网址: https://shuolidianzi.taobao.com
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################


#ifndef _Task_manager_H_
#define _Task_manager_H_
#include "sys.h"

//void (* data g_pTaskList[TASKLISTCNT])();							// 函数指针
typedef  void ( * FTimer_P) ( void );

typedef struct {
	    u16	 Task_Period;  			//time out for calling function
	    u16  Task_Count;
	    FTimer_P Task_Function;		//Send function defines in application
	   }TaskTime;

void Timer_Task_Count(void);
void Execute_Task_List_RUN(void);
void Task_Manage_List_Init(void);
void Task_LED(void);
void HFPeriod_2msTask(void);
void Pack_TestPare(void);
void task_send_Rece(void);
void LCD12864KEY_RUN(void);
u16 Boost_service_prog(void);
unsigned int GetCRC16(unsigned char *ptr,  unsigned char len);
void Write_Single_Holding_Register(u16 addr,u16 num);
void Modbus_SendData(u8 *buff,u8 len);

#endif  //Task_manager_H
