#include <stdio.h>
#include <string.h>
#include "func_handler.h"
#include "nor_flash.h"
#include "start_win.h"
#include "motor_cmd.h"
#include "main.h"

code const char Software_version[11]={"V1.10"};
code const char modle_version[11]={"--------"};
Product_TypeDef product_data[PRODUCT_TOTAL]={	 

//id,product name,	 level		   	cycle	   work_time	   pause_time	   voltage	   current
{ 0,	"GT300",    1300,1750,	  	 1,99,     1,5999,      1,5999,	    1300,1750},
};



STR_WIN_TypeDef xdata win_str_data;

//man page button handler API
static void win_main_btn_click_handler(u16 btn_val);
//work parameters settings page
static void win_test_para_btn_hold_handler(u16 btn_val);
static void win_prog_para_btn_hold_handler(u16 btn_val);
static void win_para_keypad_btn_click_handler(u16 btn_val);
static void win_factory_keypad_btn_click_handler(u16 btn_val);
static void win_pass_keypad_btn_click_handler(u16 btn_val);
static void win_prog_num_sel_btn_click_handler(u16 btn_val);
//background page button handler API
static void win_light_set_btn_hold_handler(u16 btn_val);
static void win_language_set_btn_hold_handler(u16 btn_val);
static void win_buzz_set_btn_hold_handler(u16 btn_val);
static void win_passcode_btn_click_handler(u16 val);
//factory page button handler api
static void win_fac_set_btn_click_handler(u16 btn_val);
//function
static void win_calc_function(void);
static void win_change_page(u16 page);
static void win_load_setpara(void);
static u8 win_product_select(u16 page,u16 save);

void win_fresh_model_icon(u16 mode);
typedef enum{
	MENU_PAGE = 0,   //�˵�����
	FAC_PAGE1,
	FAC_PAGE2,
	FAC_KEYBORD,
	INFO_PAGE_COMPANY,
	INFO_PAGE_NO_COMPANY,
	SYSTEM_PAGE,
	PASSWORD_PAGE,
	PASSWORD_CONFIRM_PAGE,
	WORK_PAGE,
	WORK_CONFIRM_PAGE,
	TEST_PAGE,
	TEST_CONFIRM_PAGE,
	PROGRAM_PAGE,
	PROGRAM_CONFIRM_PAGE,
	BACK_PAGE,
}Change_Page_Def;

static int Limit_Sat( int Uint,int U_max, int U_min) //���Ƹ�ֵ����
{
	int Uout;
 	if(Uint<= U_min)
		Uout= U_min;
	else if( Uint>=U_max)
		Uout=U_max;
	else
	  Uout= Uint;
  return  Uout;
}


//������������ص�����
void btn_click_callback(u16 addr,u16 val)  //����������
{
	switch(addr)
	{
		case BTN_MAIN_PAGE_ADDR:				//Main page button   0x1100 
				win_main_btn_click_handler(val);
			break;
		case BTN_PARA_KEYPAD_ADDR: 		  //�����������ְ���
				win_para_keypad_btn_click_handler(val);
			break;
		case BTN_FACTORY_KEYPAD_ADDR: 	//����ģʽ���ְ���
				win_factory_keypad_btn_click_handler(val);
			break;
		case BTN_PROG_NUM_SEL_ADDR:    	//��������ҳ�����ҷ�ҳ
				win_prog_num_sel_btn_click_handler(val);
			break;
		case BTN_PASS_KEYPAD_ADDR:  		//����ҳ�� 0X1202 
				win_pass_keypad_btn_click_handler(val);
			break;
 		case BTN_FAC_SET_ADDR:	    		//��������0X1300
				win_fac_set_btn_click_handler(val);
			break;
		
		default:
			break;
	}
}
//���ְ��������ص�����
void btn_hold_res_callback(u16 addr,u16 val)   //��ťֵ�ı��ٽ���
{
	STR_WIN_TypeDef	*obj = &win_str_data;
	switch(addr)
	{
		case BTN_TEST_MODE_SET_ADDR:		//��������0X1110
			win_test_para_btn_hold_handler(val);
			break;
		case BTN_PROG_MODE_SET_ADDR:		//��������0X1112
			win_prog_para_btn_hold_handler(val);
			break;
		case BTN_SET_LIGHT_ADDR:
			win_light_set_btn_hold_handler(val);		
			break;
		case BTN_LANGUAGE_ADDR:
			win_language_set_btn_hold_handler(val);
			break;
		case BTN_SET_BUZZ_ADDR:
			win_buzz_set_btn_hold_handler(val);
			break;
		case BTN_SET_LEVEL_ADDR:
			if(win_str_data.test_mode == 0)
				{
					win_str_data.use_level = val;
					sys_write_vp(BTN_SET_LEVEL_ADDR,(u8 *)&win_str_data.use_level,1);//hjy 20220805
					win_disp_level(MAIN_TIMER,win_str_data.use_level);
				}
				else
				{
					sys_write_vp(BTN_SET_LEVEL_ADDR,(u8 *)&win_str_data.use_level,1);
				}
				
			break;

				
		default:
			break;
	}
}
void btn_hold_click_callback(u16 addr,u16 val)
{
	STR_WIN_TypeDef	*obj = &win_str_data;
	val =0;
	if(win_str_data.start == 1) return;
	switch(addr)
	{		
		default:
			break;
	}
}
//����ʱ�䰴����ʱ�ص�����
void btn_hold_callback(u16 addr,u16 val)
{
	STR_WIN_TypeDef	*obj = &win_str_data;
	val =0;
	if(win_str_data.start == 1) return;
	switch(addr)
	{
		default:
			break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////���ܵ����Ӻ���/////////////////////////////////////////////////////////////////

static void win_calc_function(void)
{
	float xdata temp,temp1;
	STR_WIN_TypeDef	*obj = &win_str_data;	
	temp = product_data[win_str_data.product_id].vol_max-product_data[win_str_data.product_id].vol_min;
	temp1 = product_data[win_str_data.product_id].para1.max-product_data[win_str_data.product_id].para1.min;
	win_str_data.k_factor = temp/temp1;
	
	temp = win_str_data.k_factor*product_data[win_str_data.product_id].para1.min;
	win_str_data.b_factor = product_data[win_str_data.product_id].vol_min-temp;
}


u16 const page_reg[16]={						//���Ľ���
	{ 1},  //MENU_PAGE
	{ 3},  //FAC_PAGE1
	{ 4},  //FAC_PAGE2
	{ 5},  //FAC_KEYBORD
	{ 6},  //INFO_PAGE_COMPANY
	{ 7},  //INFO_PAGE_NO_COMPANY
	{ 8},  //SYSTEM_PAGE2
	{ 9},  //PASSWORD_PAGE
	{10},  //PASSWORD_PAGE_CONFIRM
	{12},  //WORK_PAGE
	{13},  //WORK_PAGE_CONFIRM
	{18},  //TEST_PAGE
	{19},  //TEST_PAGE_CONFIRM
	{21},  //PROGRAM_PAGE
	{22},  //PROGRAM_PAGE_CONFIRM
	};


static void win_change_page(u16 page)    //�л�����ҳ��
{
	u16 xdata temp;

	temp = page_reg[page];  //English
	//fresh page
	switch(page)
	{
		case WORK_PAGE:
		/*
				win_str_data.disp_count_time = ((u32)win_str_data.use_cycle * (win_str_data.use_run_time + win_str_data.use_pause_time)) - win_str_data.use_pause_time;
		
				win_disp_level(MAIN_TIMER,win_str_data.use_level);
				win_disp_cycle(MAIN_TIMER,win_str_data.use_cycle);
				win_disp_run_time(MAIN_TIMER,win_str_data.use_run_time);
				win_disp_pause_time(MAIN_TIMER,win_str_data.use_pause_time);
		*/
			break;
		
		case TEST_PAGE:
			win_disp_level(SET_TIMER,win_str_data.set_level);
			win_disp_cycle(SET_TIMER,win_str_data.set_cycle);
			win_disp_run_time(SET_TIMER,win_str_data.set_run_time);
			win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);
			break;

		case PROGRAM_PAGE:
			win_disp_level(SET_TIMER,win_str_data.set_level);   //��ʾ����ֵ
			win_disp_cycle(SET_TIMER,win_str_data.set_cycle);
			win_disp_run_time(SET_TIMER,win_str_data.set_run_time);
			win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);
			
			win_disp_level(PROG_TIMER,win_str_data.prog_level); //��ʾ����ֵ
			win_disp_cycle(PROG_TIMER,win_str_data.prog_cycle);
			win_disp_run_time(PROG_TIMER,win_str_data.prog_run_time);
			win_disp_pause_time(PROG_TIMER,win_str_data.prog_pause_time);
			break;
		
		  default:
			break;
	}
	//goto page
	sys_disp_page(temp);
}
/*********************ѡ�������ͺ�**************************/
static u8 win_product_select(u16 page,u16 save)
{
	u16 xdata index_id,temp,no;
	u16 xdata i,rang1,rang2;
	u32 xdata save_flag1;
	STR_WIN_TypeDef	*obj = &win_str_data;	

	// total page
	win_str_data.product_tpage = (PRODUCT_TOTAL-1)/5+1;
	sys_write_vp(DATA_PRODUCT_TOTALPAGE_ADDR,(u8*)&win_str_data.product_tpage,2);
	if((page > win_str_data.product_tpage)||(page == 0)) return 1;
	// current page
	sys_write_vp(DATA_PRODUCT_CURPAGE_ADDR,(u8*)&page,2);	
	if(save ==1)
	{
		sys_read_vp(BTN_FAC_SEL_ADDR,(u8*)&no,2);
		if(no == 1) no=0;   			//ѡ������ҳ��   ��1��
		else if(no == 2) no=1;		//��2��
		else if(no ==4) no = 2;		//��3��
		else if(no ==8) no =3;		//��4��
		else if(no == 16) no =4;	//��5��
		else no =0;
		no = (page-1)*5+no;
		if(no>=PRODUCT_TOTAL) return 1;
		win_str_data.product_id = no;
		norflash_write(SYS_PRUDUCT_OFFSET,(u8*)&win_str_data.product_id,2);
		
		save_flag1 = 0;  //�л�����ʱ������ȡ���������
		norflash_write(SAVE_FLAG,(u8 *)&save_flag1,2);
		
		return 0;
	}
	//fresh select block
	rang1 = page;
	rang2 = page;
	rang1=(rang1-1)*5;
	rang2= rang2*5;
	if((win_str_data.product_id < rang2)&&(win_str_data.product_id  >= rang1))
	{
		temp = 1<<(win_str_data.product_id %5);
		sys_write_vp(BTN_FAC_SEL_ADDR,(u8*)&temp,2);
	}
	else
	{
		temp =0;
		sys_write_vp(BTN_FAC_SEL_ADDR,(u8*)&temp,2);
	}
	index_id = page;
	index_id = (index_id-1)*5;
	//fresh form
	for(i =0;i<5;i++)
	{
		//id
		sys_write_vp(DATA_PRODUCT_ID1_ADDR+i*2,(u8*)&index_id,2);
		if(index_id<PRODUCT_TOTAL)
		{
			sys_write_vp(DATA_PRODUCT_P1MIN1_ADDR+i*2,(u8*)&product_data[index_id].para1.min,2);	
			sys_write_vp(DATA_PRODUCT_P1MAX1_ADDR+i*2,(u8*)&product_data[index_id].para1.max,2);	
			sys_write_vp(DATA_PRODUCT_VOLMIN1_ADDR+i*2,(u8*)&product_data[index_id].vol_min,2);	
			sys_write_vp(DATA_PRODUCT_VOLMAX1_ADDR+i*2,(u8*)&product_data[index_id].vol_max,2);
			sys_write_vp(STRING_PRODUCT_NAME1_ADDR+i*0x100,(u8*)product_data[index_id].name,sizeof(product_data[index_id].name));		
		}
		else
		{
			temp = 0;
			sys_write_vp(DATA_PRODUCT_P1MIN1_ADDR+i*2,(u8*)&temp,2);	
			sys_write_vp(DATA_PRODUCT_P1MAX1_ADDR+i*2,(u8*)&temp,2);	
			sys_write_vp(DATA_PRODUCT_P1UNIT1_ADDR+i*2,(u8*)&temp,2);
			sys_write_vp(DATA_PRODUCT_VOLMIN1_ADDR+i*2,(u8*)&temp,2);	
			sys_write_vp(DATA_PRODUCT_VOLMAX1_ADDR+i*2,(u8*)&temp,2);	
			sys_write_vp(DATA_PRODUCT_CCW1_ADDR+i*2,(u8*)&temp,2);				
			sys_write_vp(STRING_PRODUCT_NAME1_ADDR+i*0x100,"--",2);				
		}	
		index_id++;		
	}	
	return 0;
}


//---------------------------------------------------------------//
//name: win_load_setpara
//---------------------------------------------------------------//
static void win_load_setpara(void)
{
	//1.load startup logo
	if(win_str_data.logo >0) win_str_data.logo = 4;
	sys_write_vp(ANIMOTION_LOGO_ADDR,(u8*)&win_str_data.logo,2);	//no logo for oem

	if(win_str_data.light>100) win_str_data.light = 100;
	if(win_str_data.light<10) win_str_data.light = 10;
	sys_setdis_light(win_str_data.light);
	sys_write_vp(BTN_SET_LIGHT_ADDR,(u8*)&win_str_data.light,1);	

	if(win_str_data.alarm > 1) win_str_data.alarm = 1;
	if(win_str_data.alarm < 0) win_str_data.alarm = 0;
	sys_enable_speaker(1-win_str_data.alarm);		
	sys_write_vp(BTN_SET_BUZZ_ADDR,(u8*)&win_str_data.alarm,1);
	
	if(win_str_data.language >= 3) win_str_data.language = 3;
	if(win_str_data.language <= 0) win_str_data.language = 0;	
	sys_write_vp(BTN_LANGUAGE_ADDR,(u8*)&win_str_data.language,1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////�¼���������//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������水ť����¼�	
static void win_main_btn_click_handler(u16 btn_val)
{
	#define  BTN_VAL_START	     0X01    //start
	#define  BTN_VAL_PAUSE    	 0X02    //pause
	#define  BTN_VAL_STOP        0X03    //stop 
	
	#define  BTN_VAL_TEST        0X05 	 //test mode
	#define  BTN_VAL_MENU        0X06    //
	#define  BTN_VAL_SET         0X07    //settings into
	#define  BTN_VAL_CONFIRM     0X08    //Confirm startup
	#define  BTN_VAL_INFO        0X09
	#define  BTN_VAL_BACK_MAIN   0X0A    //settings background
	#define  BTN_VAL_BACK_MENU   0X0B    //settings background
	#define  BTN_VAL_RESET       0X0C
	

	u16 xdata temp = 0;
	STR_WIN_TypeDef	*obj = &win_str_data;
	switch(btn_val)
	{
		case BTN_VAL_START:     //Page1:Push Start
			if(win_str_data.pause == 1)		//pause state  ��ͣ״̬
			{
				win_str_data.start = 1;
				win_str_data.pause = 0;
			}
			else if((win_str_data.pause == 0)&&(win_str_data.start == 0))
			{
				win_change_page(WORK_CONFIRM_PAGE);				// goto set work parameters page(Popup Window)
			}
			break;
		case BTN_VAL_CONFIRM://Page13:********************  ��������ȷ������
				win_str_data.start = 1;
				win_str_data.pause = 0;
				win_change_page(WORK_PAGE);
			break;
		case BTN_VAL_STOP:     //Page1:push Stop************************
					win_str_data.start = 0;
					win_str_data.pause = 0;
			break;
		case BTN_VAL_PAUSE:     //Page1:push pause***********************
				if(win_str_data.start != 0)
				{
					win_str_data.start = 0;
					win_str_data.pause = 1;
				}
			break; 
		case BTN_VAL_TEST:			//Page1:selected test mode****************
				if((win_str_data.pause == 1)||(win_str_data.start != 0)) break;
				win_str_data.test_mode = 0;
				win_str_data.prog_grp = 0;
				sys_write_vp(ARTNUM_MAIN_PROG_NUM_ADDR,(u8*)&win_str_data.prog_grp,1);
				win_fresh_model_icon(win_str_data.test_mode);
				norflash_write(SYS_LAST_PROG_OFFSET,(u8*)&win_str_data.prog_grp,2);  // save prog_grp com_prog_grp
				win_load_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.use_level,4);

				win_disp_level(MAIN_TIMER,win_str_data.use_level);
				win_disp_cycle(MAIN_TIMER,win_str_data.use_cycle);
				win_disp_run_time(MAIN_TIMER,win_str_data.use_run_time);
				win_disp_pause_time(MAIN_TIMER,win_str_data.use_pause_time);
			break;
		case BTN_VAL_SET:         //Page1:Setting*****************************
				Fresh_Service_Time();
				win_change_page(SYSTEM_PAGE);
			break;
		case BTN_VAL_BACK_MAIN:
				win_change_page(WORK_PAGE);
			break;
		case BTN_VAL_BACK_MENU:
				win_change_page(MENU_PAGE);
			break;
		case BTN_VAL_MENU:
				if((win_str_data.pause == 1)||(win_str_data.start != 0)) break;  //�������ͣ״̬���߲���ֹͣ״̬
					win_change_page(MENU_PAGE);
			break;
		case BTN_VAL_INFO:
			if(win_str_data.logo == 4) //no logo
				win_change_page(INFO_PAGE_NO_COMPANY);
			else
				win_change_page(INFO_PAGE_COMPANY);
			break;
		case BTN_VAL_RESET:
				win_change_page(PASSWORD_PAGE);
			break;		
		default:
			break;
	}
}


#define  BTN_VAL_NO	   			0X1C00
#define  BTN_VAL_YES	   	  0X1400 //������
#define  BTN_VAL_RETURN			0X1000 //����
#define  BTN_VAL_UPSET	 		0X0C00 //���������ϴ�����
#define  BTN_VAL_OVERSET  	0X0800 //���ǳ���
#define  BTN_VAL_TOSET	    0X0400 //��ҳ��ѡ���'

#define  BTN_VAL_SEL_CYCLE		    	0X04   //��ʱ��   total time
#define  BTN_VAL_SEL_PAUSE_TIME			0X03	 //��ͣʱ�� pause time
#define  BTN_VAL_SEL_RUN_TIME		  	0X02   //����ʱ�� rum time
#define  BTN_VAL_SEL_LEVEL	        0X01   //ת��/���
void disp_input_icon(u16 btn_val)
{
	sys_write_vp(ICON_INPUT1_ADDR,(u8 *)&win_str_data.input_disp1,4);
	if(win_str_data.language == 0)  //CN 
	{
		if(btn_val == BTN_VAL_SEL_LEVEL) win_str_data.input_disp1 = win_str_data.input_disp1;
		if(btn_val == BTN_VAL_SEL_RUN_TIME) win_str_data.input_disp2 = win_str_data.input_disp2;
		if(btn_val == BTN_VAL_SEL_PAUSE_TIME) win_str_data.input_disp3 = win_str_data.input_disp3;
		if(btn_val == BTN_VAL_SEL_CYCLE) win_str_data.input_disp4 = win_str_data.input_disp4;
		sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
	}
	else if(win_str_data.language == 1)  //EN
	{
		if(btn_val == BTN_VAL_SEL_LEVEL) win_str_data.input_disp1 = win_str_data.input_disp1 + 1;
		if(btn_val == BTN_VAL_SEL_RUN_TIME) win_str_data.input_disp2 = win_str_data.input_disp2 + 1;
		if(btn_val == BTN_VAL_SEL_PAUSE_TIME) win_str_data.input_disp3 = win_str_data.input_disp3 + 1;
		if(btn_val == BTN_VAL_SEL_CYCLE) win_str_data.input_disp4 = win_str_data.input_disp4 + 1;
		sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
	}
	else if(win_str_data.language == 2)  //RUS
	{
		if(btn_val == BTN_VAL_SEL_LEVEL) win_str_data.input_disp1 = win_str_data.input_disp1 + 2;
		if(btn_val == BTN_VAL_SEL_RUN_TIME) win_str_data.input_disp2 = win_str_data.input_disp2 + 2;
		if(btn_val == BTN_VAL_SEL_PAUSE_TIME) win_str_data.input_disp3 = win_str_data.input_disp3 + 2;
		if(btn_val == BTN_VAL_SEL_CYCLE) win_str_data.input_disp4 = win_str_data.input_disp4 + 2;
		sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
	}
	else  //JAPAN
	{
		if(btn_val == BTN_VAL_SEL_LEVEL) win_str_data.input_disp1 = win_str_data.input_disp1 + 3;
		if(btn_val == BTN_VAL_SEL_RUN_TIME) win_str_data.input_disp2 = win_str_data.input_disp2 + 3;
		if(btn_val == BTN_VAL_SEL_PAUSE_TIME) win_str_data.input_disp3 = win_str_data.input_disp3 + 3;
		if(btn_val == BTN_VAL_SEL_CYCLE) win_str_data.input_disp4 = win_str_data.input_disp4 + 3;
		sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
	}
}
/********************************************************
�������ƣ�win_work_para_btn_hold_handler
�������ܣ��������ô�������
*********************************************************/
static void win_test_para_btn_hold_handler(u16 btn_val)
{
	u16 xdata temp;
	STR_WIN_TypeDef	*obj = &win_str_data;
	if(win_str_data.paras_sel == BTN_VAL_SEL_LEVEL)
	{
		if(win_str_data.set_level > product_data[win_str_data.product_id].para1.max) win_str_data.set_level = product_data[win_str_data.product_id].para1.max;
		else if(win_str_data.set_level < product_data[win_str_data.product_id].para1.min) win_str_data.set_level = product_data[win_str_data.product_id].para1.min;	
		win_disp_level(SET_TIMER,win_str_data.set_level);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_CYCLE)   //������ʱ��
	{
		if(win_str_data.set_cycle > product_data[win_str_data.product_id].para2.max) win_str_data.set_cycle = product_data[win_str_data.product_id].para2.max;
		else if(win_str_data.set_cycle < product_data[win_str_data.product_id].para2.min) win_str_data.set_cycle = product_data[win_str_data.product_id].para2.min;
		win_disp_cycle(SET_TIMER,win_str_data.set_cycle);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_RUN_TIME)
	{
		if(win_str_data.set_run_time > product_data[win_str_data.product_id].para3.max) win_str_data.set_run_time = product_data[win_str_data.product_id].para3.max;
		else if(win_str_data.set_run_time < product_data[win_str_data.product_id].para3.min) win_str_data.set_run_time = product_data[win_str_data.product_id].para3.min;
		win_disp_run_time(SET_TIMER,win_str_data.set_run_time);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_PAUSE_TIME)	
	{
		if(win_str_data.set_pause_time > product_data[win_str_data.product_id].para4.max) win_str_data.set_pause_time = product_data[win_str_data.product_id].para4.max;
		else if(win_str_data.set_pause_time < product_data[win_str_data.product_id].para4.min) win_str_data.set_pause_time = product_data[win_str_data.product_id].para4.min;
		win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);								
	}
	switch(btn_val)
	{
		case BTN_VAL_SEL_LEVEL:
				win_str_data.input_disp1 = 0x01;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				disp_input_icon(btn_val);
				win_str_data.paras_sel = btn_val;
			break;		
		case BTN_VAL_SEL_RUN_TIME:
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x01;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				disp_input_icon(btn_val);
				win_str_data.paras_sel = btn_val;
		
				win_str_data.set_run_time_bit = 4;
				win_str_data.set_run_time = 0;
				win_disp_run_time(SET_TIMER,win_str_data.set_run_time);		
			break;
		case BTN_VAL_SEL_PAUSE_TIME:
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x01;
				win_str_data.input_disp4 = 0x00;
				disp_input_icon(btn_val);
				win_str_data.paras_sel = btn_val;
				
				win_str_data.set_pause_time_bit = 4;
				win_str_data.set_pause_time = 0;
				win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);			
			break;
		case BTN_VAL_SEL_CYCLE:
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x01;
				disp_input_icon(btn_val);
				win_str_data.paras_sel = btn_val;
			break;
		case BTN_VAL_TOSET:    //Into parameter 
			if(win_str_data.test_mode == 0)  //Is test mode
			{
				memcpy((u8 *)&win_str_data.set_level,(u8 *)&win_str_data.use_level,8);
				win_change_page(TEST_PAGE);	     
			}
			else  // No test mode
			{
				temp = 0;
				sys_write_vp(BTN_TEST_MODE_SET_ADDR,(u8 *)&temp,1);
			}
			break;
		case BTN_VAL_RETURN:   //return
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				sys_write_vp(ICON_INPUT1_ADDR,(u8 *)&win_str_data.input_disp1,4);
				sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
		
				temp = 0;
				sys_write_vp(BTN_TEST_MODE_SET_ADDR,(u8*)&temp,1);
				sys_write_vp(BTN_PROG_MODE_SET_ADDR,(u8*)&temp,1);
				win_str_data.paras_sel = 0;		
				win_change_page(WORK_PAGE);
			break;
		case BTN_VAL_OVERSET:  //jump to pop up
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				sys_write_vp(ICON_INPUT1_ADDR,(u8 *)&win_str_data.input_disp1,4);
				sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
		
				win_change_page(TEST_CONFIRM_PAGE);
			break;
		case BTN_VAL_YES:    	 //Pop up�� save and upset
				memcpy((u8 *)&win_str_data.use_level,(u8 *)&win_str_data.set_level,8);		  
				win_save_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.use_level,4);
				win_change_page(TEST_PAGE);
			break;
		case BTN_VAL_NO:       //Pop up��
				win_change_page(TEST_PAGE);
			break;
			
		default:
			break;
	}
}

/********************************************************
�������ƣ�win_prog_para_btn_hold_handler
�������ܣ��������ô�������
*********************************************************/
static void win_prog_para_btn_hold_handler(u16 btn_val)
{
	u16 xdata temp;
	STR_WIN_TypeDef	*obj = &win_str_data;
	if((win_str_data.pause == 1)||(win_str_data.start != 0))
	{
		temp = 0;
		sys_write_vp(BTN_TEST_MODE_SET_ADDR,(u8*)&temp,1);
		sys_write_vp(BTN_PROG_MODE_SET_ADDR,(u8*)&temp,1);
		return;
	}
	//jugde other dat correct
	if(win_str_data.paras_sel == BTN_VAL_SEL_LEVEL)
	{
		if(win_str_data.set_level>product_data[win_str_data.product_id].para1.max) win_str_data.set_level =product_data[win_str_data.product_id].para1.max;
		else if(win_str_data.set_level < product_data[win_str_data.product_id].para1.min) win_str_data.set_level =product_data[win_str_data.product_id].para1.min;
		win_disp_level(SET_TIMER,win_str_data.set_level);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_CYCLE)
	{
		if(win_str_data.set_cycle >product_data[win_str_data.product_id].para2.max) win_str_data.set_cycle =product_data[win_str_data.product_id].para2.max;
		else if(win_str_data.set_cycle < product_data[win_str_data.product_id].para2.min) win_str_data.set_cycle =product_data[win_str_data.product_id].para2.min;
		win_disp_cycle(SET_TIMER,win_str_data.set_cycle);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_RUN_TIME)
	{
		if(win_str_data.set_run_time >product_data[win_str_data.product_id].para3.max) win_str_data.set_run_time =product_data[win_str_data.product_id].para3.max;
		else if(win_str_data.set_run_time < product_data[win_str_data.product_id].para3.min) win_str_data.set_run_time =product_data[win_str_data.product_id].para3.min;
		win_disp_run_time(SET_TIMER,win_str_data.set_run_time);					
	}
	else if(win_str_data.paras_sel == BTN_VAL_SEL_PAUSE_TIME)	
	{
		if(win_str_data.set_pause_time >product_data[win_str_data.product_id].para4.max) win_str_data.set_pause_time =product_data[win_str_data.product_id].para4.max;
		else if(win_str_data.set_pause_time < product_data[win_str_data.product_id].para4.min) win_str_data.set_pause_time =product_data[win_str_data.product_id].para4.min;
		win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);								
	}
	switch(btn_val)	
	{
		case BTN_VAL_TOSET:
				if(win_str_data.prog_grp == 0) win_str_data.prog_grp = 1;
				sys_write_vp(ARTNUM_PROG_NUM_ADDR,(u8*)&win_str_data.prog_grp,2);
				memcpy((u8 *)&win_str_data.set_level,(u8 *)&win_str_data.use_level,8);	         //display set
				win_load_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.prog_level,4);  //read prog para from flash
				win_change_page(PROGRAM_PAGE);	 //change page
			break;
		case BTN_VAL_RETURN: //Reset the key value when returning and jump page and clean paras_sel
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				sys_write_vp(ICON_INPUT1_ADDR,(u8 *)&win_str_data.input_disp1,4);
				sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
		
				win_str_data.paras_sel = 0;
				if(win_str_data.test_mode == 0) win_str_data.prog_grp = 0; //No update parameter
				win_change_page(WORK_PAGE);
			break;
		case BTN_VAL_OVERSET:  //Jump to pop-up page
				win_str_data.input_disp1 = 0x00;
				win_str_data.input_disp2 = 0x00;
				win_str_data.input_disp3 = 0x00;
				win_str_data.input_disp4 = 0x00;
				sys_write_vp(ICON_INPUT1_ADDR,(u8 *)&win_str_data.input_disp1,4);
				sys_write_vp(ICON_INPUT1_WORD_ADDR,(u8 *)&win_str_data.input_disp1,4);
		
				win_change_page(PROGRAM_CONFIRM_PAGE);	
			break;
		case BTN_VAL_YES:  	 //pop-up page YES   Put Set -> Prog and save flash
				memcpy((u8 *)&win_str_data.prog_level,(u8 *)&win_str_data.set_level,8);  					//Set value to prog value
				win_save_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.prog_level,4);	//write norflash
				win_change_page(PROGRAM_PAGE);		//change page
			break;
		case BTN_VAL_NO:
				win_change_page(PROGRAM_PAGE);	
			break;
		case BTN_VAL_UPSET://�ϴ�download program parameters to current parameters
				win_str_data.test_mode = 1;
				win_fresh_model_icon(win_str_data.test_mode);
		
				norflash_write(SYS_LAST_PROG_OFFSET,(u8*)&win_str_data.prog_grp,2);
				sys_write_vp(ARTNUM_MAIN_PROG_NUM_ADDR,(u8*)&win_str_data.prog_grp,1);
		
				memcpy((u8 *)&win_str_data.set_level,(u8 *)&win_str_data.prog_level,8);  //����ֵ�ϴ�������ֵ
				memcpy((u8 *)&win_str_data.use_level,(u8 *)&win_str_data.prog_level,8);  //����ֵ=����ֵ
		
				win_disp_level(SET_TIMER,win_str_data.set_level);      		    //��������ҳ�����ò�������ʾ
				win_disp_cycle(SET_TIMER,win_str_data.set_cycle);
				win_disp_run_time(SET_TIMER,win_str_data.set_run_time);
				win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);
			break;
						
		default:
			break;
	}
}


#define  BTN_VAL_ACC	    0X02 //��
#define  BTN_VAL_DEC	    0X01 //��
/********************************************************
�������ƣ�win_test_mode_btn_click_handler
�������ܣ���������ҳ�����ѡ�����
*********************************************************/
static void win_prog_num_sel_btn_click_handler(u16 btn_val)
{
	u16 xdata temp;
	STR_WIN_TypeDef	*obj = &win_str_data;
	switch(btn_val)
	{
		case BTN_VAL_ACC:
				if(win_str_data.prog_grp < 20) win_str_data.prog_grp++;	
			break;
		case BTN_VAL_DEC:
				if(win_str_data.prog_grp > 1) win_str_data.prog_grp--;	
			break;
		default:break;
	}
	sys_write_vp(ARTNUM_PROG_NUM_ADDR,(u8*)&win_str_data.prog_grp,2);	
	//read nor flash  fresh display
	win_load_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.prog_level,6);

	win_disp_level(PROG_TIMER,win_str_data.prog_level);
	win_disp_cycle(PROG_TIMER,win_str_data.prog_cycle);
	win_disp_run_time(PROG_TIMER,win_str_data.prog_run_time);
	win_disp_pause_time(PROG_TIMER,win_str_data.prog_pause_time);
	temp = 0;
	sys_write_vp(BTN_TEST_MODE_SET_ADDR,(u8*)&temp,2);
	sys_write_vp(BTN_PROG_MODE_SET_ADDR,(u8*)&temp,2);	

}

/********************************************************
�������ƣ�win_para_keypad_btn_click_handler
�������ܣ����ּ��̴�������
*********************************************************/
static void win_para_keypad_btn_click_handler(u16 btn_val)
{
	#define  BTN_VAL_1		1
	#define  BTN_VAL_2		2
	#define  BTN_VAL_3	  3
	#define  BTN_VAL_4    4
	#define  BTN_VAL_5    5
	#define  BTN_VAL_6		6
	#define  BTN_VAL_7		7
	#define  BTN_VAL_8    8
	#define  BTN_VAL_9		9
	#define  BTN_VAL_0		10	
	#define  BTN_VAL_cancel	11
	u16 xdata temp;
	switch(win_str_data.paras_sel)
	{
/****************************SEL_LEVEL***********************************/		
		case BTN_VAL_SEL_LEVEL:
				temp = win_str_data.set_level;
				if(btn_val<BTN_VAL_cancel) //****Number button****
				{
					if(btn_val == 10) btn_val =0; 
					if(product_data[win_str_data.product_id].para1.max > 9999)		temp = temp%10000;
					else if(product_data[win_str_data.product_id].para1.max > 999)temp = temp%1000;
					else if(product_data[win_str_data.product_id].para1.max > 99)	temp = temp%100;
					else if(product_data[win_str_data.product_id].para1.max > 9)	temp = temp%10;
					temp = temp*10;
					temp += btn_val;
				}
				else//************cancel*********************
				{
					temp = temp/10;
				}
				win_str_data.set_level = temp;
				win_disp_level(SET_TIMER,win_str_data.set_level);
			break;
/****************************TOTAL_TIME***********************************/
		case BTN_VAL_SEL_CYCLE:
			temp = win_str_data.set_cycle;
			if(btn_val<BTN_VAL_cancel) //****Number button****
			{
				if(btn_val == 10) btn_val = 0;
				if(product_data[win_str_data.product_id].para2.max > 9999)	  temp = temp%10000;					
				else if(product_data[win_str_data.product_id].para2.max > 999)temp = temp%1000;
				else if(product_data[win_str_data.product_id].para2.max > 99)	temp = temp%100;
				else if(product_data[win_str_data.product_id].para2.max > 9)	temp = temp%10;
				temp = temp*10;
				temp += btn_val;
			}
			else//***cance****
			{
				temp = temp/10;
			}	
			win_str_data.set_cycle = temp;	
			win_disp_cycle(SET_TIMER,win_str_data.set_cycle);
			break;
/*******************************RUN_TIME***********************************/			
		case BTN_VAL_SEL_RUN_TIME:
			temp = win_str_data.set_run_time;
			if(btn_val<BTN_VAL_cancel) //***********���ְ���************
			{
				if(btn_val == 10) btn_val =0;
				if(win_str_data.set_run_time_bit==4) temp += btn_val*600;// decade min
				else if(win_str_data.set_run_time_bit==3) temp += btn_val*60;//bits min
				else if(win_str_data.set_run_time_bit==2)
				{
					if(btn_val>5) btn_val=5;
					temp += btn_val*10;//decade sec
				}
				else if(win_str_data.set_run_time_bit==1) temp += btn_val; //bits min
				if(win_str_data.set_run_time_bit>0)win_str_data.set_run_time_bit--;		
			}
			else		//*******************cancel***********************
			{
				if(win_str_data.set_run_time_bit<4)win_str_data.set_run_time_bit++;
				if(win_str_data.set_run_time_bit==4) temp = 0;
				else if(win_str_data.set_run_time_bit==3) temp -= temp%600;
				else if(win_str_data.set_run_time_bit==2) temp -= temp%60;
				else if(win_str_data.set_run_time_bit==1) temp -= temp%10; 	
			}
			win_str_data.set_run_time = temp;			
			win_disp_run_time(SET_TIMER,win_str_data.set_run_time);		
			break;

/*****************************PAUSE_TIME***********************************/		
		case BTN_VAL_SEL_PAUSE_TIME:
			temp = win_str_data.set_pause_time;			
			if(btn_val < BTN_VAL_cancel)      //***********���ְ���************
			{
				if(btn_val == 10) btn_val =0;
				if(win_str_data.set_pause_time_bit==4) temp += btn_val*600;// decade min
				else if(win_str_data.set_pause_time_bit==3) temp += btn_val*60;//bits min
				else if(win_str_data.set_pause_time_bit==2)
				{
					if(btn_val>5) btn_val=5;
					temp += btn_val*10;//decade sec
				}
				else if(win_str_data.set_pause_time_bit==1) temp += btn_val; //bits min     //MH200
				if(win_str_data.set_pause_time_bit>0)win_str_data.set_pause_time_bit--;			
			}
			else//*******************cancel****************************
			{
				if(win_str_data.set_pause_time_bit<4)win_str_data.set_pause_time_bit++;
				if(win_str_data.set_pause_time_bit==4) temp = 0;
				else if(win_str_data.set_pause_time_bit==3) temp -= temp%600;
				else if(win_str_data.set_pause_time_bit==2) temp -= temp%60;
				else if(win_str_data.set_pause_time_bit==1) temp -= temp%10; 			
			}
			win_str_data.set_pause_time = temp;
			win_disp_pause_time(SET_TIMER,win_str_data.set_pause_time);
			break;
			
		default:
			break;
	}
}
/********************************************************
win_pass_keypad_btn_click_handler
�������ܣ���̨�������ּ��̴�������
*********************************************************/
static void win_pass_keypad_btn_click_handler(u16 btn_val)
{
	u16 xdata temp;
	temp = win_str_data.passcode;
	if(btn_val == 0X0C)   //������
	{
		win_str_data.used_time_h = 0;
		win_str_data.used_time_l = 0;

		norflash_write(RUNTIME_MIN_HADDR,(u8 *)&win_str_data.used_time_h,2);

		win_str_data.used_time = win_str_data.used_time_h;
		win_str_data.used_time <<= 16;
		win_str_data.used_time |= win_str_data.used_time_l;
		win_str_data.disp_time = win_str_data.used_time/3600;  //Сʱ��
	
		sys_write_vp(ARTNUM_UT_ADDR,(u8*)&win_str_data.disp_time,1);	 //������ʱ������
		win_str_data.passcode = 0;
		
		win_change_page(PASSWORD_PAGE);
	}
	if(btn_val == 0X0D)   //���� ��
	{
		win_change_page(PASSWORD_PAGE);
	}
	
	
	if(btn_val < BTN_VAL_cancel)
	{
		if(btn_val == 10) btn_val =0;
		temp = temp%1000;
		temp = temp*10;
		temp +=btn_val;
		if(temp>9999) temp =9999;
		else if(temp <0) temp =0;
		win_str_data.passcode = temp;
		sys_write_vp(ARTNUM_PASSCODE_ADDR,(u8*)&win_str_data.passcode,2);				
	}
	else
	{			
		temp= temp/10;
		if(temp <0) temp=0;
		win_str_data.passcode = temp;
		sys_write_vp(ARTNUM_PASSCODE_ADDR,(u8*)&win_str_data.passcode,2);			
	}
	
	if(temp == 1010)            //��λ����
	{
		win_str_data.passcode = 0;
		sys_write_vp(ARTNUM_PASSCODE_ADDR,(u8*)&win_str_data.passcode,2);	
		win_change_page(PASSWORD_CONFIRM_PAGE);
	}
	else if(temp == 5973)      //��̨����
	{
		win_str_data.product_cpage = 1;
		win_product_select(win_str_data.product_cpage,0);
		win_change_page(FAC_PAGE1);
		win_str_data.passcode = 0;
		sys_write_vp(ARTNUM_PASSCODE_ADDR,(u8*)&win_str_data.passcode,2);	
	}
}

/********************************************************
��������win_light_set_btn_hold_handler()
�������ܣ���������
*********************************************************/
static void win_light_set_btn_hold_handler(u16 btn_val)
{
	win_str_data.light = btn_val;
	sys_setdis_light(win_str_data.light);
	norflash_write(SYS_LIGHT_OFFSET,(u8 *)&win_str_data.light,2);	
}
/********************************************************
��������win_language_set_btn_hold_handler()
�������ܣ���������
*********************************************************/
static void win_language_set_btn_hold_handler(u16 btn_val)
{
	win_str_data.language = btn_val;
	norflash_write(SYS_LANGUAGE_OFFSET,(u8 *)&win_str_data.language,2);	
}
/********************************************************
��������win_buzz_set_btn_hold_handler()
�������ܣ�����������
*********************************************************/
static void win_buzz_set_btn_hold_handler(u16 btn_val)
{
	win_str_data.alarm = btn_val;
	sys_enable_speaker(1-win_str_data.alarm);
	norflash_write(SYS_BUZZ_OFFSET,(u8 *)&win_str_data.alarm,2);	
}

	
			
#define BTN_PAGE1_DEV   0X01
#define BTN_PAGE1_ADD		0X02
#define BTN_PAGE1_SKIP	0X03
#define BTN_PAGE1_OK	  0X04
#define BTN_PAGE2_SKIP	0X05
#define BTN_PAGE2_OK	  0X06
#define	BTN_LINK      	0X07
#define BTN_LOGO      	0X08
#define BTN_RETURN      0X09

#define BTN_VOLTAGE_MIN	0X0B
#define BTN_VOLTAGE_MAX	0X0C
#define BTN_CURRENT_MIN	0X0D
#define BTN_CURRENT_MAX	0X0E


/********************************************************
��������win_fac_set_btn_click_handler(u16 btn_val)
�������ܣ���������
*********************************************************/
static void win_fac_set_btn_click_handler(u16 btn_val)
{
	u16 xdata temp;
	STR_WIN_TypeDef	*obj = &win_str_data;
	win_str_data.factory_sel = 0;
	switch(btn_val)//��̨����0X1300
	{
		case BTN_PAGE1_DEV://dec  ��
			if(win_str_data.product_cpage>1)	win_str_data.product_cpage--;
			win_product_select(win_str_data.product_cpage,0);			
			break;
		case BTN_PAGE1_ADD://add  ��
			if(win_str_data.product_cpage<(PRODUCT_TOTAL-1)/5+1)	win_str_data.product_cpage++;
			win_product_select(win_str_data.product_cpage,0);
			break;
		case BTN_PAGE1_SKIP://page1 skip
			sys_write_vp(ARTNUM_VOLTAGE_MIN_ADDR,(u8*)&win_str_data.sys_voltage_min,1);
			sys_write_vp(ARTNUM_VOLTAGE_MAX_ADDR,(u8*)&win_str_data.sys_voltage_max,1);
			sys_write_vp(ARTNUM_CURRENT_MIN_ADDR,(u8*)&win_str_data.sys_current_min,1);
			sys_write_vp(ARTNUM_CURRENT_MAX_ADDR,(u8*)&win_str_data.sys_current_max,1);
			win_change_page(FAC_PAGE2);
			break;
		case BTN_PAGE1_OK://page 1 ok
			temp = win_product_select(win_str_data.product_cpage,1);
			if(temp == 0)//success
			{
				win_change_page(FAC_PAGE2);
				memcpy((u8 *)&win_str_data.sys_voltage_min,(u8 *)&product_data[win_str_data.product_id].vol_min,4);
				sys_write_vp(ARTNUM_VOLTAGE_MIN_ADDR,(u8*)&win_str_data.sys_voltage_min,1);
				sys_write_vp(ARTNUM_VOLTAGE_MAX_ADDR,(u8*)&win_str_data.sys_voltage_max,1);
				sys_write_vp(ARTNUM_CURRENT_MIN_ADDR,(u8*)&win_str_data.sys_current_min,1);
				sys_write_vp(ARTNUM_CURRENT_MAX_ADDR,(u8*)&win_str_data.sys_current_max,1);
			}
			break;
		case BTN_PAGE2_SKIP://page 1 ok
			win_change_page(PASSWORD_PAGE);
			break;			
		case BTN_PAGE2_OK://page 2 ok   save and jump page
			win_change_page(PASSWORD_PAGE);
			win_str_data.sys_save_flag = 0XFFFF;
			norflash_write(SYS_VOL_MIN,(u8 *)&win_str_data.sys_voltage_min,6);
			memcpy((u8 *)&product_data[win_str_data.product_id].vol_min,(u8 *)&win_str_data.sys_voltage_min,4);
			win_calc_function();
			break;
		case BTN_LOGO://logo
			win_str_data.logo = 4 - win_str_data.logo;
			if(win_str_data.logo >0) win_str_data.logo = 4;
			sys_write_vp(ANIMOTION_LOGO_ADDR,(u8*)&win_str_data.logo,1);	//no logo for oem	
			norflash_write(SYS_LOGO_OFFSET,(u8 *)&win_str_data.logo,2);
			if(win_str_data.logo == 4)
				sys_write_vp(STRING_PRODUCT_ADDR,(u8*)modle_version,sizeof(modle_version));
			else
				sys_write_vp(STRING_PRODUCT_ADDR,(u8*)product_data[win_str_data.product_id].name,sizeof(product_data[win_str_data.product_id].name));
			break;
		
		case BTN_VOLTAGE_MIN://Voltage min
		case BTN_VOLTAGE_MAX:
		case BTN_CURRENT_MIN:
		case BTN_CURRENT_MAX:
			win_str_data.sys_input_num = 0;
			sys_write_vp(ARTNUM_INPUTNUM_ADDR,(u8 *)&win_str_data.sys_input_num,1);
			win_change_page(FAC_KEYBORD);
			break;
		
	

		default:
			break;
	}
	win_str_data.factory_sel = btn_val;
}

static void win_factory_keypad_btn_click_handler(u16 btn_val)
{
	#define  BTN_FACTORY_VAL_CANCLE	0X0B	
	#define  BTN_FACTORY_VAL_ENTER	0X0C
	u16 xdata temp;
	switch(win_str_data.factory_sel)
	{
/*******************************VOLTAGE_MIN***********************************/			
		case BTN_VOLTAGE_MIN:
			temp = win_str_data.sys_input_num;
			if(btn_val < BTN_FACTORY_VAL_CANCLE) //****Number button****
			{
				if(btn_val == 10) btn_val =0;
				temp = temp%1000;					
				temp = temp*10;
				temp += btn_val;
			}
			if(btn_val == BTN_FACTORY_VAL_CANCLE)//************cancel*********************
			{
				temp = temp/10;
			}
			win_str_data.sys_input_num = temp;
			sys_write_vp(ARTNUM_INPUTNUM_ADDR,(u8 *)&temp,1);
			if(btn_val == BTN_FACTORY_VAL_ENTER)
			{
				win_str_data.sys_input_num = temp;
				win_str_data.sys_voltage_min = win_str_data.sys_input_num;
				win_str_data.factory_sel = 0;
				sys_write_vp(ARTNUM_VOLTAGE_MIN_ADDR,(u8 *)&temp,1);
				win_change_page(FAC_PAGE2);
			}
			break;
/*****************************VOLTAGE_MAX***********************************/		
		case BTN_VOLTAGE_MAX:
			temp = win_str_data.sys_input_num;
			if(btn_val < BTN_FACTORY_VAL_CANCLE) //****Number button****
			{
				if(btn_val == 10) btn_val =0;
				temp = temp%1000;					
				temp = temp*10;
				temp += btn_val;
			}
			if(btn_val == BTN_FACTORY_VAL_CANCLE)//************cancel*********************
			{
				temp = temp/10;
			}
			win_str_data.sys_input_num = temp;
			sys_write_vp(ARTNUM_INPUTNUM_ADDR,(u8 *)&temp,1);
			if(btn_val == BTN_FACTORY_VAL_ENTER)
			{
				win_str_data.sys_input_num = temp;
				win_str_data.sys_voltage_max = win_str_data.sys_input_num;
				win_str_data.factory_sel = 0;
				sys_write_vp(ARTNUM_VOLTAGE_MAX_ADDR,(u8 *)&temp,1);
				win_change_page(FAC_PAGE2);
			}
			break;

/*****************************BTN_CURRENT_MIN***********************************/		
		case BTN_CURRENT_MIN:
			temp = win_str_data.sys_input_num;
			if(btn_val < BTN_FACTORY_VAL_CANCLE) //****Number button****
			{
				if(btn_val == 10) btn_val =0;
				temp = temp%1000;					
				temp = temp*10;
				temp += btn_val;
			}
			if(btn_val == BTN_FACTORY_VAL_CANCLE)//************cancel*********************
			{
				temp = temp/10;
			}
			win_str_data.sys_input_num = temp;
			sys_write_vp(ARTNUM_INPUTNUM_ADDR,(u8 *)&temp,2);
			if(btn_val == BTN_FACTORY_VAL_ENTER)
			{
				win_str_data.sys_input_num = temp;
				win_str_data.sys_current_min = win_str_data.sys_input_num;
				win_str_data.factory_sel = 0;
				sys_write_vp(ARTNUM_CURRENT_MIN_ADDR,(u8 *)&temp,1);
				win_change_page(FAC_PAGE2);
			}
			break;
			
/*****************************BTN_CURRENT_MAX***********************************/		
		case BTN_CURRENT_MAX:
			temp = win_str_data.sys_input_num;
			if(btn_val < BTN_FACTORY_VAL_CANCLE) //****Number button****
			{
				if(btn_val == 10) btn_val =0;
				temp = temp%10000;					
				temp = temp*10;
				temp += btn_val;
			}
			if(btn_val == BTN_FACTORY_VAL_CANCLE)//************cancel*********************
			{
				temp = temp/10;
			}
			win_str_data.sys_input_num = temp;
			sys_write_vp(ARTNUM_INPUTNUM_ADDR,(u8 *)&temp,2);
			if(btn_val == BTN_FACTORY_VAL_ENTER)
			{
				win_str_data.sys_input_num = temp;
				win_str_data.sys_current_max = win_str_data.sys_input_num;
				win_str_data.factory_sel = 0;
				sys_write_vp(ARTNUM_CURRENT_MAX_ADDR,(u8 *)&temp,1);
				win_change_page(FAC_PAGE2);
			}
			break;
		default:
			break;
	}
}

/********************************************************
�������ƣ�void win_save_prog_paras(u16 offaddr,u8 *var_addr,u16 len)
�������ܣ��洢����
�������ݣ�
u16 offaddr   flash��ʼ��ַ
u8 *var_addr  ������ַ
u16 len       �������� 
*********************************************************/
void win_save_prog_paras(u16 offaddr,u8 *var_addr,u16 len)
{
	u16 addr;
	STR_WIN_TypeDef	*obj = &win_str_data;	
	if(win_str_data.prog_grp>0)
	{
		addr = PROG1_BASE_NORADDR + 0x10 * (win_str_data.prog_grp - 1) + offaddr;
	}
	else
	{
		addr = CUSTOM_BASE_NORADDR+offaddr;
	}
	norflash_write(addr,var_addr,len);
}

/********************************************************
�������ƣ�void win_load_prog_paras(u16 offaddr,u8 *var_addr,u16 len)
�������ܣ���ȡ����
�������ݣ�
u16 offaddr   flash��ʼ��ַ
u8 *var_addr  ������ַ
u16 len       �������� 
*********************************************************/

void win_load_prog_paras(u16 offaddr,u8 *var_addr,u16 len)
{
	u16 addr;
	u16 xdata i;
	STR_WIN_TypeDef	*obj = &win_str_data;	
	if(win_str_data.prog_grp>0)
	{
		addr = PROG1_BASE_NORADDR + 0x10 * (win_str_data.prog_grp - 1) + offaddr;
	}
	else
	{
		addr = CUSTOM_BASE_NORADDR+offaddr;
	}
	norflash_read(addr,var_addr,len);
	
  for(i = 0;i<len;i++)
	{
		addr = offaddr + i;
		switch(addr)
		{
			case PROG_LEVEL_OFFSET:
				if(win_str_data.prog_level > product_data[win_str_data.product_id].para1.max)	win_str_data.prog_level = product_data[win_str_data.product_id].para1.min;	
				if(win_str_data.prog_level < product_data[win_str_data.product_id].para1.min)	win_str_data.prog_level = product_data[win_str_data.product_id].para1.min;	
			break;
			case PROG_TT_OFFSET:	
				if(win_str_data.prog_cycle > product_data[win_str_data.product_id].para2.max) win_str_data.prog_cycle = product_data[win_str_data.product_id].para2.min;
				if(win_str_data.prog_cycle < product_data[win_str_data.product_id].para2.min) win_str_data.prog_cycle = product_data[win_str_data.product_id].para2.min;				
			break;
			case PROG_WT_OFFSET:
				if(win_str_data.prog_run_time >product_data[win_str_data.product_id].para3.max) win_str_data.prog_run_time = product_data[win_str_data.product_id].para3.min;
				if(win_str_data.prog_run_time <product_data[win_str_data.product_id].para3.min) win_str_data.prog_run_time = product_data[win_str_data.product_id].para3.min;
			break;
			case PROG_PT_OFFSET:
				if(win_str_data.prog_pause_time >product_data[win_str_data.product_id].para4.max) win_str_data.prog_pause_time =  product_data[win_str_data.product_id].para4.min;	
				if(win_str_data.prog_pause_time <product_data[win_str_data.product_id].para4.min) win_str_data.prog_pause_time = product_data[win_str_data.product_id].para4.min;
			break;

		
			default:break;
		}
	}
}


void win_disp_level(u16 ch,u16 val)   //��ʾ
{
	if(ch & MAIN_TIMER)
	{
		//sys_write_vp(ARTNUM_MAIN_LEVEL_ADDR,(u8 *)&val,1);
	}
	if(ch & SET_TIMER)
	{
		sys_write_vp(ARTNUM_SET_LEVEL_ADDR,(u8 *)&val,1);
	}
	if(ch & PROG_TIMER)
	{
		sys_write_vp(ARTMUM_PROG_LEVEL_ADDR,(u8 *)&val,1);	
	}
}

/******************************************************************************************
������void win_disp_cycle(u16 ch,u16 val)
�������ܣ���ʾ��ʱ��																														
*******************************************************************************************/

void win_disp_cycle(u16 ch,u16 val)  //��ʾ��ʱ�� ����ѭ������
{
	u16 xdata h_dat;
	u16 xdata l_dat;

	if(ch & MAIN_TIMER)  //��ҳ��
	{
		h_dat = win_str_data.use_cycle - val+1;	//���д�����1��ʼ
		l_dat = win_str_data.use_cycle;
	}
	else  //����ҳ��
	{
		l_dat = val;
	}
	
	if(ch & MAIN_TIMER)
	{
		sys_write_vp(ARTNUM_MAIN_CYCLE_ADDR,(u8 *)&l_dat,1);
		sys_write_vp(ARTNUM_MAIN_CYCLE_HADDR,(u8 *)&h_dat,1);
	}
	if(ch & SET_TIMER)
	{
		sys_write_vp(ARTNUM_SET_CYCLE_ADDR,(u8 *)&l_dat,1);
	}
	if(ch & PROG_TIMER)
	{
		sys_write_vp(ARTNUM_PROG_CYCLE_ADDR,(u8 *)&l_dat,1);	
	}
}

/******************************************************************************************
������void win_disp_run_time(u16 ch,u16 val)
�������ܣ���ʾ����ʱ��																														
*******************************************************************************************/
void win_disp_run_time(u16 ch,u16 val)		//��ʾ����ʱ��
{
	u16 xdata h_dat;
	u16 xdata l_dat;
	
	h_dat = val/60;
	l_dat = val%60;
	
	if(ch & MAIN_TIMER)
	{
		sys_write_vp(ARTNUM_MAIN_RUN_TIME_ADDR,(u8 *)&l_dat,1);
		sys_write_vp(ARTNUM_MAIN_RUN_TIME_HADDR,(u8 *)&h_dat,1);
	}	
	if(ch & SET_TIMER)
	{
		sys_write_vp(ARTNUM_SET_RUN_TIME_ADDR,(u8 *)&l_dat,1);
		sys_write_vp(ARTNUM_SET_RUN_TIME_HADDR,(u8 *)&h_dat,1);
	}
	if(ch & PROG_TIMER)
	{
		sys_write_vp(ARTNUM_PROG_RUN_TIME_ADDR,(u8 *)&l_dat,1);	
		sys_write_vp(ARTNUM_PROG_RUN_TIME_HADDR,(u8 *)&h_dat,1);
	}
}
/******************************************************************************************
������void win_disp_pause_time(u16 ch,u16 val)
�������ܣ���ʾ��ͣʱ��																														
*******************************************************************************************/
void win_disp_pause_time(u16 ch,u16 val)  //��ʾ��ͣʱ��
{
	u16 xdata h_dat;
	u16 xdata l_dat;
	
	h_dat = val/60;
	l_dat = val%60;
	
	if(ch & MAIN_TIMER)
	{
		sys_write_vp(ARTNUM_MAIN_PAUSE_TIME_ADDR,(u8 *)&l_dat,1);
		sys_write_vp(ARTNUM_MAIN_PAUSE_TIME_HADDR,(u8 *)&h_dat,1);
	}
	if(ch & SET_TIMER)
	{
		sys_write_vp(ARTNUM_SET_PAUSE_TIME_ADDR,(u8 *)&l_dat,1);
		sys_write_vp(ARTNUM_SET_PAUSE_TIME_HADDR,(u8 *)&h_dat,1);
	}
	if(ch & PROG_TIMER)
	{
		sys_write_vp(ARTNUM_PROG_PAUSE_TIME_ADDR,(u8 *)&l_dat,1);	
		sys_write_vp(ARTNUM_PROG_PAUSE_TIME_HADDR,(u8 *)&h_dat,1);
	}
}
//****************************************************************************************
u16 win_sec_timer_handle(u16* stage)
{
	u16 xdata ret = 0;//����ֵ
	if(*stage == 0)//run stage
	{
		win_str_data.start = 1;//����ͼ��
		if(win_str_data.disp_run_time > 0)win_str_data.disp_run_time--;
		if(win_str_data.disp_run_time == 0)
		{
			win_str_data.disp_run_time = win_str_data.use_run_time; 	
			*stage = 1;	
			ret= 2;// return1  pause
		}
	}
	else if(*stage == 1)//pause stage
	{
		win_str_data.start = 2;//��ͣͼ��
		if(win_str_data.disp_pause_time > 0)win_str_data.disp_pause_time--;
		if(win_str_data.disp_pause_time == 0)
		{
			win_str_data.disp_pause_time = win_str_data.use_pause_time; 
			if(win_str_data.disp_cycle > 0)  //ѭ������
				win_str_data.disp_cycle--;				
			*stage = 0;
			ret= 1;  //return2 start
		}
	}

	if(win_str_data.disp_count_time > 0)			//process total time
	{
		win_str_data.disp_count_time--;
	}
	if(win_str_data.disp_count_time == 0)
	{
	  ret= 3;
	}	
	return ret;
}
/******************************************************************************************
������void win_fresh_window(SysDataDef *sys)
�������ܣ�ˢ�´���ͼ��																														
*******************************************************************************************/
void win_fresh_window(SysDataDef *sys)
{
	if(str_sys.door == 0)
		win_str_data.door_warning = 1;
	else 
		win_str_data.door_warning = 0;
	
	sys_write_vp(ICON_DOOR_ADDR,(u8*)&win_str_data.door_warning,1);   //0��ͼ�� �Ź� 1��ͼ�� �ſ�
	sys_write_vp(ICON_START_ADDR,(u8*)&win_str_data.start,1);
	sys_write_vp(ICON_PAUSE_ADDR,(u8*)&win_str_data.pause,1);

	//fresh main page timer block
	win_fresh_timer_block();
	//fresh load bar

}
/******************************************************************************************
������void win_fresh_model_icon(SysDataDef *sys)
�������ܣ�ˢ�´���ģʽͼ��																														
*******************************************************************************************/
void win_fresh_model_icon(u16 mode)
{
	u16 xdata temp;
	if(mode == 0)
	{
		temp = 1;
		sys_write_vp(ICON_TEST_ADDR,(u8*)&temp,1);
		temp = 0;
		sys_write_vp(ICON_PROG_ADDR,(u8*)&temp,1);
	}
	else
	{
		temp = 0;
		sys_write_vp(ICON_TEST_ADDR,(u8*)&temp,1);
		temp = 1;
		sys_write_vp(ICON_PROG_ADDR,(u8*)&temp,1);		
	}
}		
/******************************************************************************************
������u16 win_level_trans_volt(u16 level)
�������ܣ����������ѹֵ																																		
*******************************************************************************************/
u16 win_level_trans_volt(u16 level)       //�����ѹֵ
{
	float temp;
	STR_WIN_TypeDef	*obj = &win_str_data;
	
	temp = level;

	temp = (float)(win_str_data.k_factor * temp)+win_str_data.b_factor;	
	
	if(temp >6000)
	temp =6000;

	return (u16)temp;
}
/*************ˢ������ʱ��************************/
void win_fresh_timer_block(void)
{
	static u16 timer_fresh_ratio;
	STR_WIN_TypeDef	*obj = &win_str_data;	
	if(timer_fresh_ratio >0) timer_fresh_ratio--;	
	{
		timer_fresh_ratio = 50;//20hz
		if((win_str_data.start != 0)&&(win_str_data.pause == 0))  
		{
			win_disp_cycle(MAIN_TIMER,win_str_data.disp_cycle);
			win_disp_run_time(MAIN_TIMER,win_str_data.disp_run_time);
			win_disp_pause_time(MAIN_TIMER,win_str_data.disp_pause_time);
		}
	}
}

void Fresh_Service_Time(void)
{
	win_str_data.disp_time = win_str_data.used_time/3600;  //Сʱ��
	sys_write_vp(ARTNUM_UT_ADDR,(u8*)&win_str_data.disp_time,1);
}

void Init_read_norflash(void)
{
	norflash_read(SYS_LOGO_OFFSET,(u8 *)&win_str_data.logo,8);
	
	if(win_str_data.prog_grp > 20)  //�����ж�
	{
		win_str_data.prog_grp = 0;
		win_str_data.test_mode = 0;
	}
}
void Prog_mode_para_read(void)
{
	if(win_str_data.prog_grp == 0) 
	{
		win_str_data.test_mode = 0;
		win_load_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.prog_level,4);
		memcpy((u8*)&win_str_data.use_level,(u8*)&win_str_data.prog_level,8);
		memcpy((u8*)&win_str_data.set_level,(u8*)&win_str_data.prog_level,8);
	}	
	else if(win_str_data.prog_grp > 0) 
	{
		win_str_data.test_mode = 1;
		win_load_prog_paras(PROG_LEVEL_OFFSET,(u8*)&win_str_data.prog_level,4);
		memcpy((u8*)&win_str_data.use_level,(u8*)&win_str_data.prog_level,8);
		memcpy((u8*)&win_str_data.set_level,(u8*)&win_str_data.prog_level,8);
	}
	sys_write_vp(ARTNUM_MAIN_PROG_NUM_ADDR,(u8*)&win_str_data.prog_grp,1);
	win_fresh_model_icon(win_str_data.test_mode);
}
//****************************�������ݳ�ʼ��****************************
void win_init(void)
{
	u16 temp = 0;
	/*
	u16 temp = 0;
	STR_WIN_TypeDef	*obj = &win_str_data;
	memset(&win_str_data,0,sizeof(win_str_data));
//init workbence para
//read from flash///////////////////////////////////////////
	Init_read_norflash();
//1.load startup logo
	if(win_str_data.logo >0) win_str_data.logo = 4;
	sys_write_vp(ANIMOTION_LOGO_ADDR,(u8*)&win_str_data.logo,2);	//no logo for oem
//2.load system parametes settings
	win_load_setpara();
//3	service time
	win_str_data.used_time = win_str_data.used_time_h;
	win_str_data.used_time <<= 16;
	win_str_data.used_time |= win_str_data.used_time_l;
	win_str_data.disp_time = win_str_data.used_time/3600;  //Сʱ��

	if(win_str_data.product_id > PRODUCT_TOTAL) win_str_data.product_id = 0;
//.��������΢����ѹ��ȡ
	norflash_read(SYS_VOL_MIN,(u8 *)&win_str_data.sys_voltage_min,6);	
	if(win_str_data.sys_save_flag == 0XFFFF)
	{
		memcpy((u8 *)&product_data[win_str_data.product_id].vol_min,(u8 *)&win_str_data.sys_voltage_min,8);
	}
	else
	{
		memcpy((u8 *)&win_str_data.sys_voltage_min,(u8 *)&product_data[win_str_data.product_id].vol_min,8);
	}

//��ϳ���ģʽ�� \ ����ģʽ \ ʵ��ģʽ�жϲ�����
//4 program group
	Prog_mode_para_read();

//7.����ƥ��	
	if(win_str_data.logo == 4)
	{
		sys_write_vp(STRING_PRODUCT_ADDR,(u8*)modle_version,sizeof(modle_version));
	}
	else
	{
		sys_write_vp(STRING_PRODUCT_ADDR,(u8*)product_data[win_str_data.product_id].name,sizeof(product_data[win_str_data.product_id].name));
	}
	sys_write_vp(STRING_VERSION_ADDR,(u8*)Software_version,sizeof(Software_version));

	win_str_data.model_num = product_data[win_str_data.product_id].model_id;

//10. calc level vs voltage function
	win_calc_function();
	*/
	temp = 0x2800;
	sys_write_vp(0X1105,(u8*)&temp,1);
	sys_delay_about_ms(4000);  
//11. jump work page	

	win_change_page(WORK_PAGE);
}
