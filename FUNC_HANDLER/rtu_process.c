#include "sys.h"
#include "string.h"
#include "usart.h"
#include "rtu_api.h"
#include "motor_cmd.h"
#include "rtu_protocol.h"
#include "rtu_process.h"
#include "start_win.h"

 void RTU_Cmd_Pending(u8 cmd,u16 dat)
{
	rtu_structure.cmd[rtu_structure.pending] = cmd;
	rtu_structure.cmd_data[rtu_structure.pending] = dat;
	rtu_structure.pending = ++rtu_structure.pending%MAX_BUFFER_NUMBER;
}
void RTU_Send_Cmd(u8 index)
{
	switch(rtu_structure.cmd[index])
	{
		case RTU_STOP:
		case RTU_START:
			rtu_protocol_format_wr(ADDR_MOTOR_SET_FWD,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_SET_VLOTAGE:
			rtu_protocol_format_wr(ADDR_MOTOR_SET_VOL,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_DOOR_LOCK://                                                  //����DC1
		case RTU_DOOR_UNLOCK://
			rtu_protocol_format_wr(ADDR_SET_DC1,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;		
		case RTU_DCFAN_RUN:   //���ȿ�ʼת��
		case RTU_DCFAN_STOP:   	 //����ֹͣ   str_sys.set_dc2 = 0;
			rtu_protocol_format_wr(ADDR_SET_DC2,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_ACFAN_RUN:   //���ȿ�ʼת��  str_sys.set_ac1 = 0x01;
		case RTU_ACFAN_STOP:   	 //����ֹͣ  			str_sys.set_ac1 = 0;
			rtu_protocol_format_wr(ADDR_SET_AC1,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_SET_TEMP:
			rtu_protocol_format_wr(ADDR_SET_TEMP,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_SET_LOCK:
		rtu_protocol_format_wr(ADDR_SET_LOCK,1,(u16 *)&rtu_structure.cmd_data[index]);
			break;
		case RTU_IDlE:
			rtu_protocol_format_rd(ADDR_MOTOR_SET_FWD,ADDR_HREG_TOTAL);
			break;		
		default:
			break;
	}
}
void RTU_Send_Task_Handler(void)
{
	if(rtu_structure.event == EV_REPEAT)
	{
		if(rtu_structure.repeat == 0)
		{
			sys_speaker_work(1);
			rtu_structure.addr = 0;
			rtu_structure.funcode = 0;
			rtu_structure.event = EV_READY;
		}
		else
		{
			rtu_structure.processed--;
			RTU_Send_Cmd(rtu_structure.processed);
			rtu_structure.event = EV_EXECUTE;
			rtu_structure.cmd[rtu_structure.processed] = RTU_IDlE;
			rtu_structure.processed = ++rtu_structure.processed % MAX_BUFFER_NUMBER;
		}
		if(rtu_structure.repeat>0) rtu_structure.repeat--;
	}
	if(rtu_structure.event!=EV_READY)
	return;
	if(rtu_structure.processed == rtu_structure.pending)
	{
		RTU_Cmd_Pending(RTU_IDlE,0);
	}

	//start process send cmd
	rtu_structure.event = EV_FRAME_SENT;
	RTU_Send_Cmd(rtu_structure.processed);
	rtu_structure.repeat = 3;
	rtu_structure.event = EV_EXECUTE;
	//rtu_structure.cmd[rtu_structure.processed] = RTU_IDlE;
	rtu_structure.processed = ++rtu_structure.processed % MAX_BUFFER_NUMBER;
}
//dispose receive_data
void API_Callback_Handler(API_StructDef* request, API_Data_StructDef* response_data)
{
	unsigned short idata addr=0;
	unsigned short idata index=0;
	unsigned short* dat = request->Data;
	response_data = 0;
	addr = RTU_MEM(request->addr);
	switch(request->funcode)
	{
		case RTU_FUNC_CODE_RD:
			for(index = 0;index<request->len;index++)
			{
				usRegHoldingBuf[addr+index].dat[0] = request->Data[index];
			}
		break;
		case RTU_FUNC_CODE_WR:
			break;
		case RTU_FUNC_CODE_CWR:
		break;
		case RTU_FUNC_CODE_ERROR:
			
		break;
		default: break;
	}
}
