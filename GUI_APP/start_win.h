#ifndef __START_WIN_H__
#define __START_WIN_H__
#include "sys.h"


#define SS2000      0
#define SS300 			1
#define SS450       2

#define PRODUCT_TOTAL 3     //机型数量
/*---------------------define arae-------------------------*/
/********************************************************
地址OX2000开头 显示变量
地址OX2000开头 显示变量
系统变量接口（0x0000-0x0FFF 变量存储器空间）
0x1000-0xFFFF 变量存储空间用户可以任意使用。

**********************LOGO*****************************/
#define ANIMOTION_LOGO_ADDR		    0x2100
//WORK PAGE VAR
/*******************主页面显示*************************/
//0x2000~0x201F
#define ARTNUM_MAIN_LEVEL_ADDR		    	  0x2000

#define ARTNUM_MAIN_CYCLE_ADDR   	 			  0x2002
#define ARTNUM_MAIN_CYCLE_HADDR		  		  0X2003

#define ARTNUM_MAIN_RUN_TIME_ADDR 	   		0x2004
#define ARTNUM_MAIN_RUN_TIME_HADDR 	   		0x2005

#define ARTNUM_MAIN_PAUSE_TIME_ADDR 	 		0x2006
#define ARTNUM_MAIN_PAUSE_TIME_HADDR 	 		0x2007

#define ARTNUM_MAIN_PROG_NUM_ADDR					0x200A
#define ARTNUM_MAIN_LOAD_BAR_ADDR					0x200B

/*******************参数设置*************************   秒*/  
//0x2020~0x203F
#define ARTNUM_SET_LEVEL_ADDR		        0x2020

#define ARTNUM_SET_RUN_TIME_ADDR 	    	0x2021
#define ARTNUM_SET_RUN_TIME_HADDR 	    0x2022

#define ARTNUM_SET_PAUSE_TIME_ADDR 	 		0x2023
#define ARTNUM_SET_PAUSE_TIME_HADDR 		0x2024

#define ARTNUM_SET_CYCLE_ADDR     	  	0x2025
#define ARTNUM_SET_CYCLE_HADDR   	    	0x2026
/******************程序参数显示**********************/
//0x2040~0x205F
#define ARTMUM_PROG_LEVEL_ADDR			  	0x2040

#define ARTNUM_PROG_RUN_TIME_ADDR		  	0x2041
#define ARTNUM_PROG_RUN_TIME_HADDR			0x2042

#define ARTNUM_PROG_PAUSE_TIME_ADDR			0x2043
#define ARTNUM_PROG_PAUSE_TIME_HADDR		0x2044

#define ARTNUM_PROG_CYCLE_ADDR  		  	0x2045
#define ARTNUM_PROG_CYCLE_HADDR       	0x2046

#define ARTNUM_PROG_NUM_ADDR						0x2047
/*******************************************************/

#define ARTNUM_PASSCODE_ADDR					0x2060  //显示输入密码
#define ARTNUM_INPUTNUM_ADDR		    	0x2061  //键盘显示输入数据
#define ARTNUM_UT_ADDR								0x2062 //service used time	 总运行时间


#define ARTNUM_VOLTAGE_MIN_ADDR       0x2080
#define ARTNUM_VOLTAGE_MAX_ADDR       0x2081
#define ARTNUM_CURRENT_MIN_ADDR       0x2082
#define ARTNUM_CURRENT_MAX_ADDR       0x2083



//功能图标
#define ICON_START_ADDR 	0x2500
#define ICON_PAUSE_ADDR 	0x2501
#define ICON_DOOR_ADDR    0x2502
//程序、实验模式图标
#define ICON_PROG_ADDR 	  0x2503
#define ICON_TEST_ADDR 	  0x2504

#define ICON_INPUT1_ADDR 	0x2505
#define ICON_INPUT2_ADDR 	0x2506
#define ICON_INPUT3_ADDR 	0x2507
#define ICON_INPUT4_ADDR 	0x2508

#define ICON_INPUT1_WORD_ADDR 	0x2509
#define ICON_INPUT2_WORD_ADDR 	0x250A
#define ICON_INPUT3_WORD_ADDR 	0x250B
#define ICON_INPUT4_WORD_ADDR 	0x250C

//string
#define STRING_PRODUCT_ADDR 	0x3000
#define STRING_VERSION_ADDR 	0x3100
#define STRING_MAIN_DIR_ADDR	0X3200
#define STRING_SET_DIR_ADDR	  0X3300
#define STRING_PROG_DIR_ADDR	0X3400
#define SHAPE_LOAD_BAR_ADDR   0x3A00
//factory 
//step 1 page
#define DATA_PRODUCT_ID1_ADDR	0x2A00
#define DATA_PRODUCT_ID2_ADDR	0x2A02
#define DATA_PRODUCT_ID3_ADDR	0x2A04
#define DATA_PRODUCT_ID4_ADDR	0x2A06
#define DATA_PRODUCT_ID5_ADDR	0x2A08

#define DATA_PRODUCT_P1MIN1_ADDR 0x2a0a
#define DATA_PRODUCT_P1MIN2_ADDR 0x2a0c
#define DATA_PRODUCT_P1MIN3_ADDR 0x2a0e
#define DATA_PRODUCT_P1MIN4_ADDR 0x2a10
#define DATA_PRODUCT_P1MIN5_ADDR 0x2a12

#define DATA_PRODUCT_P1MAX1_ADDR 0x2a14
#define DATA_PRODUCT_P1MAX2_ADDR 0x2a16
#define DATA_PRODUCT_P1MAX3_ADDR 0x2a18
#define DATA_PRODUCT_P1MAX4_ADDR 0x2a1A
#define DATA_PRODUCT_P1MAX5_ADDR 0x2a1C

#define DATA_PRODUCT_P1UNIT1_ADDR 0x2a1E
#define DATA_PRODUCT_P1UNIT2_ADDR 0x2a20
#define DATA_PRODUCT_P1UNIT3_ADDR 0x2a22
#define DATA_PRODUCT_P1UNIT4_ADDR 0x2a24
#define DATA_PRODUCT_P1UNIT5_ADDR 0x2a26

#define DATA_PRODUCT_VOLMIN1_ADDR 0x2a28
#define DATA_PRODUCT_VOLMIN2_ADDR 0x2a2A
#define DATA_PRODUCT_VOLMIN3_ADDR 0x2a2C
#define DATA_PRODUCT_VOLMIN4_ADDR 0x2a2E
#define DATA_PRODUCT_VOLMIN5_ADDR 0x2a30

#define DATA_PRODUCT_VOLMAX1_ADDR 0x2a32
#define DATA_PRODUCT_VOLMAX2_ADDR 0x2a34
#define DATA_PRODUCT_VOLMAX3_ADDR 0x2a36
#define DATA_PRODUCT_VOLMAX4_ADDR 0x2a38
#define DATA_PRODUCT_VOLMAX5_ADDR 0x2a3A

#define DATA_PRODUCT_CURMIN1_ADDR 0x2a3C
#define DATA_PRODUCT_CURMIN2_ADDR 0x2a3E
#define DATA_PRODUCT_CURMIN3_ADDR 0x2a40
#define DATA_PRODUCT_CURMIN4_ADDR 0x2a42
#define DATA_PRODUCT_CURMIN5_ADDR 0x2a44

#define DATA_PRODUCT_CURMAX1_ADDR 0x2a46
#define DATA_PRODUCT_CURMAX2_ADDR 0x2a48
#define DATA_PRODUCT_CURMAX3_ADDR 0x2a4A
#define DATA_PRODUCT_CURMAX4_ADDR 0x2a4C
#define DATA_PRODUCT_CURMAX5_ADDR 0x2a4E

#define DATA_PRODUCT_CCW1_ADDR 0x2a50
#define DATA_PRODUCT_CCW2_ADDR 0x2a52
#define DATA_PRODUCT_CCW3_ADDR 0x2a54
#define DATA_PRODUCT_CCW4_ADDR 0x2a56
#define DATA_PRODUCT_CCW5_ADDR 0x2a58

#define DATA_PRODUCT_CURPAGE_ADDR 0X2A5A
#define DATA_PRODUCT_TOTALPAGE_ADDR 0X2A5C


#define STRING_PRODUCT_NAME1_ADDR	0X2B00
#define STRING_PRODUCT_NAME2_ADDR	0X2C00
#define STRING_PRODUCT_NAME3_ADDR	0X2D00
#define STRING_PRODUCT_NAME4_ADDR	0X2E00
#define STRING_PRODUCT_NAME5_ADDR	0X2F00


//nor flash address list,the address lentgh is 2 bytes 

#define SYS_LOGO_OFFSET					0X00
#define SYS_LANGUAGE_OFFSET			0X01
#define SYS_LIGHT_OFFSET				0X02
#define SYS_BUZZ_OFFSET				  0X03
#define SYS_PRUDUCT_OFFSET			0X04
#define SYS_LAST_PROG_OFFSET		0X05
#define RUNTIME_MIN_HADDR 	    0X06
#define RUNTIME_MIN_LADDR 			0X07

#define CUSTOM_BASE_NORADDR		0x10
#define PROG1_BASE_NORADDR		0x20
#define PROG2_BASE_NORADDR		0x30
#define PROG3_BASE_NORADDR		0x40
#define PROG4_BASE_NORADDR		0x50
#define PROG5_BASE_NORADDR		0x60
#define PROG6_BASE_NORADDR		0x70
#define PROG7_BASE_NORADDR		0x80
#define PROG8_BASE_NORADDR		0x90
#define PROG9_BASE_NORADDR		0xA0
#define PROG10_BASE_NORADDR		0xB0
#define PROG11_BASE_NORADDR		0xC0
#define PROG12_BASE_NORADDR		0xD0
#define PROG13_BASE_NORADDR		0xE0
#define PROG14_BASE_NORADDR		0xF0
#define PROG15_BASE_NORADDR		0x110
#define PROG16_BASE_NORADDR		0x120
#define PROG17_BASE_NORADDR		0x130
#define PROG18_BASE_NORADDR		0x140
#define PROG19_BASE_NORADDR		0x150
#define PROG20_BASE_NORADDR		0x160
// each addr include 2 byte
#define PROG_LEVEL_OFFSET		 0
#define PROG_TT_OFFSET 			 1  //Total time
#define PROG_WT_OFFSET			 2  //Run time
#define PROG_PT_OFFSET			 3  //Pause time


#define SYS_VOL_MIN					0X170
#define SYS_VOL_MAX					0X171
#define SYS_CUR_MIN					0X172
#define SYS_CUR_MAX					0X173
#define SAVE_FLAG     			0X174




/*-------------------------------------------------------*/

typedef struct{
	u16 min;	//para min value
	u16 max;	//para max value
}PARA_RangeDef;

typedef struct{
	u16 model_id;
	char name[20];//product name

	PARA_RangeDef para1;//level
	PARA_RangeDef para2;//total time
	PARA_RangeDef para3;//work time
	PARA_RangeDef para4;//pause time
	
	u16 vol_min;		//the minimum level vs the minimum output voltage
	u16 vol_max;		//the maximum level vs the maximum output voltage
	u16 cur_min;		//the minimum AC current vs load bar 0%
	u16 cur_max;		//the maximum AC current vs load bar 100%
	
}Product_TypeDef;



	
typedef struct{
	
	//norflash
	u16 logo;     //0:on  1:off
	u16 language;
	u16 light;
	u16 alarm;    //0:on  1:off
	u16 product_id;
 	u16 prog_grp; //0:test mode,1-20 program mode
	u16 used_time_h;   //记秒数高位
	u16 used_time_l;   //记秒数低位

	u32 used_time;     //记总时间 秒数
	u16 disp_time;     ///显示时间 总小时数
	
	u16 start;	// 0:stop 1:run:2:run-pause
	u16 pause;	//0:No_pause 1:pause
	u16 paras_sel;//current work parasmeters selected 
	u16 factory_sel;
	float k_factor;
	float b_factor;

	//work parasmeters
	u16 sys_voltage_min;  //voltage
	u16 sys_voltage_max;
  u16 sys_current_min;
  u16 sys_current_max;
	u16 sys_save_flag;
	u16 sys_input_num;
	//work parasmeters
	u16 use_level;
	u16 use_run_time;
	u16 use_pause_time; 
	u16 use_cycle;
	//background settigns parameters 
	u16 set_level;
	u16 set_run_time;
	u16 set_pause_time;	
	u16 set_cycle;
	u16 set_run_time_bit;
	u16 set_pause_time_bit;	
	//background program parameters 
	u16 prog_level;
	u16 prog_run_time;
	u16 prog_pause_time;
	u16 prog_cycle;
	//main page display parameters during running state
	u16 disp_run_time;
	u16 disp_pause_time;
	u16 disp_cycle;
	u32 disp_count_time;
	//nor flash cache variable//
	/********************/
	u16 test_mode;		//0:test mode 1:program mode/com program mode
	u16 door_warning;
	u16 passcode;
	u16 product_cpage;
	u16 product_tpage;
	/*-----------------------------------------*/
/*******************************/	
	u16 state;  			//状态机
	u16 model_num;

	u16 run_timer_state;  //0 run 1pause

	u16 input_disp1;
	u16 input_disp2;
	u16 input_disp3;
	u16 input_disp4;
	u16 fan_tick;
}STR_WIN_TypeDef;

//START_WIN结构体
extern STR_WIN_TypeDef xdata win_str_data;

typedef enum
{
	MAIN_TIMER=1,
	SET_TIMER=2,
	PROG_TIMER=4,
}Timer_CH_Def;

void win_disp_level(u16 ch,u16 val);
void win_disp_cycle(u16 ch,u16 val);
void win_disp_run_time(u16 ch,u16 val);
void win_disp_pause_time(u16 ch,u16 val);
u16 win_sec_timer_handle(u16* stage);

void win_init(void);

u16 win_level_trans_volt(u16 level);
void win_fresh_timer_block(void);
void win_fresh_window(SysDataDef *sys);
void Fresh_Service_Time(void);
void win_fresh_load_bar(u16 current);
void win_save_prog_paras(u16 offaddr,u8 *var_addr,u16 len);
void win_load_prog_paras(u16 offaddr,u8 *var_addr,u16 len);
#endif

