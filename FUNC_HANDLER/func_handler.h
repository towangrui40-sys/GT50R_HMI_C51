#ifndef __FUNC_HANDLER_H__
#define __FUNC_HANDLER_H__
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////button paras config///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*---------------------define arae-------------------------*/
#define BTN_LANGUAGE_ADDR			0x1105  //������
//main page
#define BTN_MAIN_PAGE_ADDR		0x1100
#define BTN_SET_LEVEL_ADDR	    0x1101



//paras input //��������
#define BTN_TEST_MODE_SET_ADDR		  	0X1110    //ʵ��ģʽ���ð�ť
#define BTN_PROG_MODE_SET_ADDR	  		0X1112		//����ģʽ���ð�ť
#define BTN_PROG_NUM_SEL_ADDR			 0X1114    //����ѡ��
#define BTN_PARA_KEYPAD_ADDR	      	0X1116    //�������ü���


//system settings  ���ý���
#define BTN_SET_LIGHT_ADDR				0x1200 
#define BTN_SET_BUZZ_ADDR					0x1201 
#define BTN_PASS_KEYPAD_ADDR			0X1202   //�������		
#define BTN_FACTORY_KEYPAD_ADDR		0X1206   //
//factory mode
#define BTN_FAC_SET_ADDR		0X1300
#define BTN_FAC_SEL_ADDR		0X1302
/*-------------------------------------------------------*/


/*----------------------config arae-----------------------------*/
#define HOLD_ACTIVE_ADDR		0x1150	//hold button active register default 1
#define HOLD_PASSIVE_ADDR		0x1152	//hold button passive register default 0

#define BTN_EVENT_NUM 		 12    //��ť����



#define BTN_SCAN_PERIOD		30   //��ť����¼���ɨ������,��λms
/*-------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////




//ö��  
//#define BTN_TYPE_CLICK  0
//#define BTN_TYPE_HOLD_RES  1
//#define BTN_TYPE_HOLD_TIME  2
enum BTN_TYPE { BTN_TYPE_CLICK = 0,BTN_TYPE_HOLD_RES,BTN_TYPE_HOLD_TIME};


typedef struct{
	u16 addr;
	u16 val;
	u16 val_temp;
	u16 timer;
	enum BTN_TYPE  type;
	void (*func_callback)(u16 addr,u16 val);
	void (*func_hold_callback)(u16 addr,u16 val);	
}STR_BTN_TypeDef;

void btn_init(void);
void btn_click_tick(void);
void btn_click_handler(void);


void btn_click_callback(u16 addr,u16 val);
void btn_hold_res_callback(u16 addr,u16 val);
void btn_hold_click_callback(u16 addr,u16 val);
void btn_hold_callback(u16 addr,u16 val);

#endif
