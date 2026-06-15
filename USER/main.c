#include "main.h"
#include "string.h"

/***********************Version record********************************
 
 Version:V4.0
 date:2022/06/20
 author:Jason
 Note:
 1.Initial creation 
 ------------------------------------------
 
 Version:V4.01
 date:2022/xx/xx
 author:Rui.Wang
 Note:xx

 -----------------------------------------
 
 Version:V4.02
 date:2022/xx/xx
 author:Rui.Wang
 Note:xx
 ------------------------------------------
 
 Version:V4.03
 date:2022/xx/xx
 author:Rui.Wang
 Note:xx  
 ------------------------------------------
 
 Version:V4.04
 date:2022/12/05
 author:Jason
 Note:
 1.modify FM200&BMxx-series door-lock logic
 2.fix FM200 work parameters 
 3.modify modbus RTU protocol layer:
	use the data sending cache to replace data variables 
 to solve the problem that data is overwritten when tasks
 are out of synchronization
 4.add BMxx-series the window pop-up tips when the status change form pause to start,
**********************************************************************/

/**************************************************
触摸地址设置在#include "func_handler.h"
显示变量地址在#include "start_win.h"

//点击按键处理回调函数
void btn_click_callback(u16 addr,u16 val)

**************************************************/
SysDataDef xdata str_sys;

void Init_Rtu_RegHoldingBuf(SysDataDef* dat)
{
	memset(&str_sys,0,sizeof(str_sys));
	usRegHoldingBuf[ADDR_MOTOR_GET_FWD].dat = (u16 *)&str_sys.mc_para.fwd;
	usRegHoldingBuf[ADDR_MOTOR_GET_REV].dat = (u16 *)&str_sys.mc_para.rev;
	usRegHoldingBuf[ADDR_MOTOR_GET_RESET].dat = (u16 *)&str_sys.mc_para.reset;
	usRegHoldingBuf[ADDR_MOTOR_GET_FAULT].dat = (u16 *)&str_sys.mc_para.fault;
	usRegHoldingBuf[ADDR_MOTOR_SET_RUN].dat = (u16 *)&str_sys.mc_para.set_run;
	usRegHoldingBuf[ADDR_MOTOR_SET_VOL].dat = (u16 *)&str_sys.mc_para.set_vol;

	usRegHoldingBuf[ADDR_GET_MEMSX].dat = (u16 *)&dat->str_mems.x;
	usRegHoldingBuf[ADDR_GET_MEMSY].dat = (u16 *)&dat->str_mems.y;
	usRegHoldingBuf[ADDR_GET_MEMSZ].dat = (u16 *)&dat->str_mems.z;

	usRegHoldingBuf[ADDR_TANK1_TEMP].dat = (u16 *)&str_sys.tank1_temp;
	usRegHoldingBuf[ADDR_TANK2_TEMP].dat = (u16 *)&str_sys.tank2_temp;
	
	usRegHoldingBuf[ADDR_TEMP].dat = (u16 *)&str_sys.temprature;
	usRegHoldingBuf[ADDR_CUR].dat = (u16 *)&str_sys.current;
  usRegHoldingBuf[ADDR_DOOR].dat = (u16 *)&str_sys.door;//
	usRegHoldingBuf[ADDR_LOCK].dat = (u16 *)&str_sys.lock;//

	usRegHoldingBuf[ADDR_SET_DC1].dat = (u16 *)&dat->set_dc1;
	usRegHoldingBuf[ADDR_SET_DC2].dat = (u16 *)&dat->set_dc2;
	usRegHoldingBuf[ADDR_SET_DC3].dat = (u16 *)&dat->set_dc3;
	
	usRegHoldingBuf[ADDR_SET_AC1].dat = (u16 *)&dat->set_ac1;
	usRegHoldingBuf[ADDR_SET_AC2].dat = (u16 *)&dat->set_ac2;
	usRegHoldingBuf[ADDR_SET_AC3].dat = (u16 *)&dat->set_ac3;
	
	usRegHoldingBuf[ADDR_SET_TEMP].dat = (u16 *)&dat->set_temp;
		
	usRegHoldingBuf[ADDR_SET_LOCK].dat = (u16 *)&dat->set_lock;
	usRegHoldingBuf[ADDR_GET_LOCK].dat = (u16 *)&dat->get_lock;

}

void main(void)
{
	sys_init();//系统初始化
	Init_Rtu_RegHoldingBuf(&str_sys);
	rtu_data_init();
	win_init();
	btn_init();
	T5L_HMIslvInit(); //初始化通讯参数
	EA=1;
	Task_Manage_List_Init( );  // 事件任务函数初始化
	while(1)
	{
		Execute_Task_List_RUN();

		T5L_HMIslvSub(); //功能：在线更新触摸屏程序

	}
}


