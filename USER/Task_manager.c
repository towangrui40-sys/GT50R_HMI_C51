//############################################################
// FILE:  Task_manager.c
// Created on: 2022年2月20日
// Author: Jason
// summary: Task_manager
//############################################################
#include "main.h"


#define Task_Num   4   //task +1
#define HFPeriod_COUNT     1   	//1ms
#define RTUPeriod_COUNT    50    //50ms
#define FSMPeriod_COUNT    10  	//5ms
#define TIMERPeriod_COUNT  250   //250ms

void Send_voltage_service(void);
	
TaskTime xdata TasksPare[Task_Num];
extern code const Product_TypeDef product_data[PRODUCT_TOTAL];
void Timer_Task_Count(void)
{
	u8 data Task_Count=0;
 for(Task_Count=0;Task_Count<Task_Num;Task_Count++)    //TASK_NUM=5
 {
  if((TasksPare[Task_Count].Task_Count<TasksPare[Task_Count].Task_Period)&&(TasksPare[Task_Count].Task_Period>0))
  {
	  TasksPare[Task_Count].Task_Count++;   // 计数 事件任务计数
  }
 }
}

void Execute_Task_List_RUN(void)
{
	u8 data	Task_Count = 0;
 for(Task_Count=0;Task_Count < Task_Num;Task_Count++)
 {
  if((TasksPare[Task_Count].Task_Count>=TasksPare[Task_Count].Task_Period)&&(TasksPare[Task_Count].Task_Period > 0))
  {
		TasksPare[Task_Count].Task_Function();  // 运行计数的时间任务函数
	  TasksPare[Task_Count].Task_Count=0;
  }
 }
}



#define SPK_TIME_OUT_TIMES 5		   //蜂鸣器响5次
void Task_Timer_Sec(void)  //  250ms运行一次
{
	u16 xdata temp;
	static u16 xdata tick = 0;  //1s计时
	static u16 xdata spk = 0;
	static u16 xdata spk_tick = 0;

	
	if(win_str_data.state == FSM_RUN)/**********FSM_RUN***********/
	{
		tick++;	
		if(tick >= 4)    //250ms进一次，4次为1s
		{
			tick = 0;
			temp = win_sec_timer_handle(&win_str_data.run_timer_state);
			if(temp == 1) RTU_Cmd_Pending(RTU_RUN_FWD,MC_FWD);
			else if(temp == 2) RTU_Cmd_Pending(RTU_STOP,MC_STOP);
			else if(temp == 3) spk = 2*SPK_TIME_OUT_TIMES;
			win_str_data.used_time++;
		}
	}	
			
	if(win_str_data.state == FSM_READY)
	{
		win_str_data.run_timer_state = 0;
		if(spk)        //停止蜂鸣器
		{
			spk--;	
			spk_tick = 1-spk_tick;
			if((spk_tick)&&(win_str_data.alarm == 0))
			sys_speaker_work(20);
		}
		if(win_str_data.fan_tick > 0 ) win_str_data.fan_tick--;
		if(win_str_data.fan_tick == 1) RTU_Cmd_Pending(RTU_DCFAN_STOP,0);		
	}
}

void RTU_send_Rece(void)  // 100ms数据周期，可自行修改周期，最快1ms
{
	RTU_Send_Task_Handler();
}

/*********************状态机******************************/
void Run_FSM(void)   //FSM状态机 
{

	switch(win_str_data.state)
	{
		case FSM_INIT:    //***************状态机：状态机初始化*****************************************************
			win_str_data.state = FSM_READY;   //初始化位READY状态
			break;
		case FSM_READY:   //****************状态机：准备状态********************************************************	
				/**************显示值设定为设置值*******************/
				memcpy((u8 *)&win_str_data.disp_run_time,(u8 *)&win_str_data.use_run_time,6);
//				win_disp_level(MAIN_TIMER,win_str_data.use_level);
				/**************************************************/
				if(win_str_data.start == 1 && win_str_data.door_warning == 0)    //接收到Start_win  start=1开始信号
				{
					RTU_Cmd_Pending(RTU_DCFAN_RUN,1);  //DC2
					win_str_data.state = FSM_START;
				}
				else
				{
					win_str_data.start = 0;
				}
			break;
				
		case FSM_START:/*******************************************状态机：开始状态********************************/
				RTU_Cmd_Pending(RTU_RUN_FWD,MC_FWD);
				win_str_data.state = FSM_RUN;
			break;		
	case FSM_RUN:/*******************************************状态机：运行状态*********************************/   //运行过程中
				Send_voltage_service();
				if(win_str_data.disp_count_time == 0)
				{
					win_str_data.start = 0;
				}
				
				if(win_str_data.pause == 1)/********** pause = 1暂停信号 pause = 0无暂停信号******************************/
				{
					RTU_Cmd_Pending(RTU_STOP,MC_STOP);   //send motor stop
					win_str_data.state = FSM_PAUSE;
				}	
				else if(win_str_data.start == 0) //start = 0停止信号 start = 1 运行信号
				{
					RTU_Cmd_Pending(RTU_STOP,MC_STOP);   //send motor stop
					win_str_data.state = FSM_STOP;
				}				
				else if(win_str_data.door_warning == 1)/********** 运行过程中开门**********/
				{
					win_str_data.start = 0;
					win_str_data.pause = 1;
				}


			break;
		case FSM_PAUSE: /**********************************状态机：暂停状态********************************/
				if((win_str_data.start == 1) && (win_str_data.pause == 0))     //点运行按钮,恢复运行
				{
					win_str_data.state = FSM_START;
				}
				if((win_str_data.start == 0) && (win_str_data.pause == 0)) //start = 0停止信号 start = 1 运行信号
				{
					win_str_data.state = FSM_STOP;
				}
			break;
		case FSM_STOP:
			
				win_str_data.disp_count_time = ((u32)win_str_data.use_cycle * (win_str_data.use_run_time + win_str_data.use_pause_time)) - win_str_data.use_pause_time;
		
				win_str_data.used_time_l = win_str_data.used_time & 0xffff;  //Save used time
				win_str_data.used_time_h = (win_str_data.used_time>>16) & 0xffff; 
				norflash_write(RUNTIME_MIN_HADDR,(u8*)&win_str_data.used_time_h,2);
	
				win_disp_run_time(MAIN_TIMER,win_str_data.use_run_time);
				win_disp_pause_time(MAIN_TIMER,win_str_data.use_pause_time);
				win_disp_cycle(MAIN_TIMER,win_str_data.use_cycle);		
		
				win_str_data.state = FSM_READY;
			break;
	
		default:
			break;
	}
}
void HFPeriod_1msTask(void)
{
	btn_click_handler();//按钮点击事件处理	
	rtu_response_handler();
	win_fresh_window(&str_sys);     //刷新界面
}


void Task_Manage_List_Init(void)
{
	TasksPare[0].Task_Period=HFPeriod_COUNT; //PERIOD_COUNT=1    1ms
	TasksPare[0].Task_Count=1;
	TasksPare[0].Task_Function=HFPeriod_1msTask;  

	TasksPare[1].Task_Period=RTUPeriod_COUNT; //2ms
	TasksPare[1].Task_Count=8;
	TasksPare[1].Task_Function=RTU_send_Rece; //  通讯接收和发送;

	TasksPare[2].Task_Period=FSMPeriod_COUNT; //100ms
	TasksPare[2].Task_Count=80;
	TasksPare[2].Task_Function=Run_FSM;//  通讯接收和发送;

	TasksPare[3].Task_Period = TIMERPeriod_COUNT;//500ms
	TasksPare[3].Task_Count=300;
	TasksPare[3].Task_Function = Task_Timer_Sec;   // 500ms的LED的闪烁
}

void Send_voltage_service(void)
{
	u16 vol_temp;
	vol_temp = win_level_trans_volt(win_str_data.use_level);   //读取计算电压值
	if(str_sys.mc_para.set_vol != vol_temp)	   //读取到下位机电压值不等于设置电压值
	{
		str_sys.mc_para.set_vol = vol_temp;
		RTU_Cmd_Pending(RTU_SET_VLOTAGE,vol_temp);
	}
}




