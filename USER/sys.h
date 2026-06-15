#ifndef __SYS_H__
#define __SYS_H__
#include "t5los8051.h"
#include <string.h>
//类型重定义
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;
typedef signed char     s8;
typedef signed short    s16;
typedef signed long     s32;

//看门狗宏定义
#define	WDT_ON()				MUX_SEL|=0x02		//开启看门狗
#define	WDT_OFF()				MUX_SEL&=0xFD		//关闭看门狗
#define	WDT_RST()				MUX_SEL|=0x01		//喂狗

//系统主频和1ms定时数值定义
#define FOSC     				206438400UL
#define T1MS    				(65536-FOSC/12/1000)





//app setting parsmetes config
#define CFG_SYS_SLEEP_TIME  180 //3min,unit 1s  


//变量申明
//形状类型
typedef enum {
	SHAPE_TYPE_POINT = 0x0001,//画一个1像素的点
	SHAPE_TYPE_LINE = 0x0002, //端点连线
	SHAPE_TYPE_RECT = 0x0003,//画矩形
	SHAPE_TYPE_FILL_RECT = 0x0004,//画填充矩形
	SHAPE_TYPE_AREA_COPY = 0x0006,//界面区域拷贝
	SHAPE_TYPE_VER_LINE =  0x0009//频谱,其实就是垂直线
}SHAPE_TYPE;

//点形状的数据
typedef struct{
	u16 vp;//往哪一个vp地址写入形状,需要用户配置相应的值
	u16 shape_type;//形状类型,需要用户配置相应的值
	u16 shape_num;//此类形状的个数,内部将会直接固定为1个
	u16 x;//点的x坐标,需要用户配置相应的值
	u16 y;//点的y坐标,需要用户配置相应的值
	u16 color;//点的颜色,需要用户配置相应的值
	u16 end_flag;//结束标记,内部将会直接固定为0xff00
}SHAPE_DATA_POINT;

//端点连线形状或线段形状的数据
typedef struct{
	u16 vp;//往哪一个vp地址写入形状,需要用户配置相应的值
	u16 shape_type;//形状类型,需要用户配置相应的值
	u16 shape_num;//此类形状的个数,内部将会直接固定为1个
	u16 color;//线的颜色,需要用户配置相应的值
	u16 x0;//第一个点的x坐标,需要用户配置相应的值
	u16 y0;//第一个点的y坐标,需要用户配置相应的值
	u16 x1;//第二个点的x坐标,需要用户配置相应的值
	u16 y1;//第二个点的y坐标,需要用户配置相应的值
	u16 end_flag;//结束标记,内部将会直接固定为0xff00
}SHAPE_DATA_LINE;

//矩形或者填充矩形
typedef struct{
	u16 vp;//往哪一个vp地址写入形状,需要用户配置相应的值
	u16 shape_type;//形状类型,需要用户配置相应的值
	u16 shape_num;//此类形状的个数,内部将会直接固定为1个
	u16 xs;//左上角的x坐标,需要用户配置相应的值
	u16 ys;//左上角的y坐标,需要用户配置相应的值
	u16 xe;//右下角的x坐标,需要用户配置相应的值
	u16 ye;//右下角的y坐标,需要用户配置相应的值
	u16 color;//线或者填充颜色,需要用户配置相应的值
	u16 end_flag;//结束标记,内部将会直接固定为0xff00
}SHAPE_DATA_RECT;
typedef SHAPE_DATA_RECT	 SHAPE_DATA_FILL_RECT;
	
//界面区域拷贝
typedef struct{
	u16 vp;//往哪一个vp地址写入形状,需要用户配置相应的值
	u16 shape_type;//形状类型,需要用户配置相应的值
	u16 shape_num;//此类形状的个数,内部将会直接固定为1个
	u16 pic_id;//界面id,代表从哪一个界面上去拷贝区域,需要用户配置相应的值
	u16 xs;//拷贝区域左上角的x坐标,需要用户配置相应的值
	u16 ys;//拷贝区域左上角的y坐标,需要用户配置相应的值
	u16 xe;//拷贝区域右下角的x坐标,需要用户配置相应的值
	u16 ye;//拷贝区域右下角的y坐标,需要用户配置相应的值
	u16 x;//粘贴到当前界面的x坐标
	u16 y;//粘贴到当前界面的y坐标
	u16 end_flag;//结束标记,内部将会直接固定为0xff00
}SHAPE_DATA_AREA_COPY;

//频谱,垂直线
typedef struct{
	u16 vp;//往哪一个vp地址写入形状,需要用户配置相应的值
	u16 shape_type;//形状类型,需要用户配置相应的值
	u16 shape_num;//此类形状的个数,内部将会直接固定为1个
	u16 color;//线的颜色,需要用户配置相应的值
	u16 x0;//x坐标,需要用户配置相应的值
	u16 y0s;//起始y坐标,需要用户配置相应的值
	u16 y0e;//终止y坐标,需要用户配置相应的值
	u16 end_flag;//结束标记,内部将会直接固定为0xff00
}SHAPE_DATA_VER_LINE;

typedef struct {
	int fwd;
	int rev;
	int reset;
	int fault;
	int set_run;
	int set_vol;
}MOTOR_Control_Order;

typedef struct
{
	signed short x;
	signed short y;
	signed short z;
    
}MEMS_TYPEDEF;


typedef struct
{
	MOTOR_Control_Order mc_para;
	MEMS_TYPEDEF str_mems;
//	int real_speed;
	int tank1_temp;
	int tank2_temp;
	int temprature;
	int current;
	int door;
	int lock;
	int set_dc1;
	int set_dc2;
	int set_dc3;
	int set_ac1;
	int set_ac2;
	int set_ac3;
  int set_temp;
	int set_lock;
	int get_lock;
}SysDataDef;

//系统结构体
extern SysDataDef str_sys;

typedef enum
{
  FSM_INIT = 0,
	FSM_READY,
	FSM_START,
	FSM_RUN,
	FSM_RUN_2,
	FSM_LINK_START,
	FSM_PAUSE,
	FSM_PAUSE_DELAY_UNLOCK,
	FSM_STOP,
	FSM_STOP_DELAY_UNLOCK,
}FSM_StateType;         //状态机结构体



//函数申明
void sys_init(void);
void sys_delay_about_ms(u16 ms);
void sys_delay_about_us(u8 us);
void sys_delay_ms(u16 ms);
void sys_read_vp(u16 addr,u8* buf,u16 len);
void sys_write_vp(u16 addr,u8* buf,u16 len);
void sys_draw_shape(u16 * shape_data);
void sys_disp_page(u16 page);
void sys_enable_speaker(u16 enable);
void sys_speaker_work(u16 timer);//unit 8ms
void sys_setdis_light(u16 light_val);
void sys_readflash(u16 addr,u8*buf,u16 num);
void sys_wirteflash(u16 addr,u8*buf,u16 num);
#endif


