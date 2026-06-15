#include "sys.h"
#include "rtu_protocol.h"
#include "motor_cmd.h"

#include "main.h"

ST_MOTOR_PARAS_DEF st_motor_paras;

void motor_set_station(u8 station)
{
		st_motor_paras.station = station;
}
/*--------------------------------/
命令内容含义：
0001：正转运行（无参数）
0002：反转运行（无参数）
0003：减速停机
0004：自由停机
0005：正转点动起动
0006：正转点动停机
0007：保留
0008：运行（无方向）
0009：故障复位
000A: 反转点动起动
000B: 反转点动停机
000C：休眠唤醒
/--------------------------------*/


