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
������ַ������#include "func_handler.h"
��ʾ������ַ��#include "start_win.h"

//������������ص�����
void btn_click_callback(u16 addr,u16 val)

**************************************************/
SysDataDef xdata str_sys;

void Init_Rtu_RegHoldingBuf(SysDataDef* dat)
{
	memset(&str_sys,0,sizeof(str_sys));
	usRegHoldingBuf[ADDR_MOTOR_GET_FWD].dat = (u16 *)&dat->fwd;
	usRegHoldingBuf[ADDR_MOTOR_SET_VOL].dat = (u16 *)&dat->set_vol;
	usRegHoldingBuf[ADDR_SET_TEMP_CMD].dat = (u16 *)&dat->set_temp_cmd;
	usRegHoldingBuf[ADDR_SET_TEMP].dat = (u16 *)&dat->set_temp;   
	usRegHoldingBuf[ADDR_NTC_TEMP].dat = (u16 *)&dat->ntc_temp;
	
	usRegHoldingBuf[ADDR_SET_FEQ].dat = (u16 *)&dat->set_feq;
	usRegHoldingBuf[ADDR_DISP_FREQ].dat = (u16 *)&dat->disp_freq;
  	usRegHoldingBuf[ADDR_DOOR].dat = (u16 *)&dat->door;
	usRegHoldingBuf[ADDR_LOCK].dat = (u16 *)&dat->lock;

	usRegHoldingBuf[ADDR_SET_LOCK].dat = (u16 *)&dat->set_lock;  //门锁控制
	usRegHoldingBuf[ADDR_SET_DC1].dat = (u16 *)&dat->set_dc1;
	usRegHoldingBuf[ADDR_SET_DC2].dat = (u16 *)&dat->set_dc2;
	usRegHoldingBuf[ADDR_SET_DC3].dat = (u16 *)&dat->set_dc3;

	usRegHoldingBuf[ADDR_SET_AC1].dat = (u16 *)&dat->set_ac1;   //压缩机风扇控制
	usRegHoldingBuf[ADDR_SET_AC2].dat = (u16 *)&dat->set_ac2;
	usRegHoldingBuf[ADDR_SET_AC3].dat = (u16 *)&dat->set_ac3;

}

void main(void)
{
	sys_init();//ϵͳ��ʼ��
	Init_Rtu_RegHoldingBuf(&str_sys);
	rtu_data_init();
	win_init();
	btn_init();
	T5L_HMIslvInit(); //��ʼ��ͨѶ����
	EA=1;
	Task_Manage_List_Init( );  // �¼���������ʼ��
	while(1)
	{
		Execute_Task_List_RUN();

		T5L_HMIslvSub(); //���ܣ����߸��´���������

	}
}


