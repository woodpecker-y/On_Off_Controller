#ifndef _SYS_PARAMS_H
#define _SYS_PARAMS_H

#include "adf.h"
#include "board.h"
#include "valve_deal.h"
#include "error.h"


extern const char *foryon;
//extern const char *foryon_log;

//阀门系统功能裁剪
#define DEBUG_SWITCH              0     //调试日志开关
#define ONE_WEEK_SWITCH           1     //一周自转功能
#define OUTAGE_POWER_SWITCH       1     //断电开阀功能
#define RF_LOSS_OPEN_VALVE_SWITCH 1     //用户控制模式无线失联则打开阀门  一个小时没有和温控器建立连接则表示失联，则阀门在用户控制模式下自动打开，阀门显示失联
#define LCD_SWITCH                1     //液晶屏开关

#define IN_OUT_TEMP_SWITCH        0     //进回水温度开关
#define WHOLE_SWITCH              1     //一体阀开关 (只用来区分不同的硬件版本号)
    
//软硬件版本号
/*通断控制器版本记录：
    带温度探头的   H：13   S：62
    不带温度探头的 H：13   S：61
    
    一体阀的通断控制器版本记录：
    带温度探头的   H：11   S：62
    不带温度探头的 H：11   S：61
*/

/*------------ 硬件版本号  ------------*/
#if WHOLE_SWITCH
    //一体阀通断控制器硬件版本
    #define HARDWARE_VERSION      11    //硬件版本号 11 表示V1.1
#else
    //通断控制器硬件版本
    #define HARDWARE_VERSION      13    //硬件版本号 11 表示V1.1
#endif

/*------------ 软件版本号  ------------*/
#if IN_OUT_TEMP_SWITCH
    //带温度探头的软件版本
    #define SOFTWARE_VERSION      62    //软件版本号 60 为延续老版本通断阀协议
#else
    //不带温度探头的软件版本
    #define SOFTWARE_VERSION      61    //软件版本号 60 为延续老版本通断阀协议
#endif

    
    
//时间宏定义
#define UNIT_SECOND	              1UL
#define UNIT_MIN		          (60*UNIT_SECOND)
#define UNIT_HOUR                 (60*UNIT_MIN)
#define UNIT_DAY                  (24*UNIT_HOUR)

//系统宏定义
//一周自转功能宏
#define ONE_WEEK_TURN_TIME         7*UNIT_DAY
//LCD显示时间 单位为s   秒
#define LCD_DISP_DELAY             3   


#pragma pack(1)                 /* 指定按n字节对齐 */

typedef enum
{
	State_OK    = 0x00,
	State_Error = 0xAA
} State_TypeDef;

//实时时钟结构体
 typedef struct
 {
    u8  Second;
    u8  Minute;
    u8  Hour;
    u8  Week;
    u8  Day;
    u8  Month;
    u16 Year;
    u16 Msec;
 }RealClock_Str;

 //阀门状态
 typedef enum
{
	//动作请求 定义
	FORCE_NOR		= 0X00,	    //非强制状态
	FORCE_CLOSE		= 0XAA,	    //强制关阀
	FORCE_OPEN		= 0X55,	    //强制开阀
	
	VALVE_NOR		= 0X00,	    //阀门无动作请求
	
	//球阀状态(位置) 动作方向 定义
	VALVE_OPEN	    = 0X0A,	    //开阀	请求开启标志
	VALVE_MIDDLE	= 0X55,	    //中间状态
	VALVE_CLOSE	    = 0XA0,	    //关阀	请求开启标志
	VALVE_BREAK	    = 0XFF,	    //故障状态
	
//	SW_ON		    = 0XAA,	    //开关状态-闭合
//	SW_OFF		    = 0X00	    //开关状态-断开
}Motor_Type;

// 运行参数，仅存于内存中
typedef struct _RunParams{
    // 温度相关
	u16	Cur_Temp;				//当前室内温度     此值是通过温控器传输得到的，温控器直接赋值为2位小数 则 2555 = 25.55摄氏度
	u16	Ave_Temp;				//平均温度         此值是通过温控器传输得到的，温控器直接赋值为2位小数 则 2555 = 25.55摄氏度
	s16	Input_Temp;			    //进水温度  255=25.5℃  协议中是保留2位小数的 即 2550 = 25.50摄氏度 在协议中直接乘以10处理
	s16	Output_Temp;			//回水温度  255=25.5℃  协议中是保留2位小数的 即 2550 = 25.50摄氏度 在协议中直接乘以10处理
    u8	Face_State;	            //面板故障状态    电量低0X01  通信故障0X80
    
    // 阀门相关
    u8  ValveState;             //阀门现在状态  0X55中间状态  0X00:中间状态  0X0A:开阀  0XA0:关阀
    u8  ValvEerror;             //阀门故障状态  0xFF(堵转、两个微动开关同时闭合、驱动芯片坏)     0x00正常
    u8  Plug_valve_state;       //阀门堵转状态  0x05开阀堵转  0x50关阀堵转   0x55全部堵转   0x00无堵转
    u16 All_open_delay;         //阀门广播开阀等待 阀间动作延时单位毫秒
    u16 All_close_delay;        //阀门广播关阀等待 阀间动作延时单位毫秒
    
    // LED相关
    Valve_led_twinkle valve_led_action;       //阀门led灯显示状态
    
    //RF433无线
    u16  rf_comm_timer;           //无线是否建立链接计时器
    u16  rf_comm_flag;          //和无线保持联系状态 0失联，1链接
    
    
    //故障标识
//	u8	FactPara_StateErr;		//出厂参数错误状态指示器
//	u8	DevPara_StateErr;		//设备参数错误状态指示器
//	u8	ShareCtrl_StateErr;		//分摊参数错误状态指示器
//	u8	Record_StateErr;		//记录参数错误状态指示器	
    
                                //0x80RTC故障、        0x40:入口传感器故障、 0x20:出口温度故障、 0x10:传动机构故障、
                                //0x08:出厂参数错误、   0x04:用参数错误、     0x02:分摊参数错误、 0x01:记录参数错误
	u8	DevErrState;			//设备错误状态指示器   位标志   用来上传和显示  
//	u8 VavleReceiveFaceState;	//阀门接收面板状态，  0xAA  阀一小时未与温控面板通讯， 0 阀与面板在1小时内有通讯

    //计算相关
    u8  WorkFlg;                //供暖季标志   当前状态 是否在供暖季 
    u32 heating_timer;        //检测供暖期计时器
	u32 Com_OK;				    //通讯成功次数
	u32 Com_Total;				//通讯总次数
    u16 save_cycle_optim_timer; //定时10分钟保存一次开阀时间 
    u32 one_week_turn_timer;          //一周自转
    
    //断电开阀标志
    u8 PowerDownFlag;           //供电标志  0断电，1上电
    
    u32 Rtc_check_sec;          //保存时间秒数
    u32 Rtc_check_timer;        //RTC检测定时器
    u32 Check_time;             //自动检测RTC时间   5 或 UNIT_HOUR/2
    u8  Deviation;
//	u8	RTC_State;				//实时时钟状态    用来标志RTC的健康状态  注意:只有在硬件对时和复位对时时该标志才会被更新
//	u8	SensorIN_State;			//入口传感器故障状态指示器
//	u8	SensorOUT_State;		//出口传感器故障状态指示器 
//	u8	Motor_State;			//传动机构故障状态指示器	


//自动化测试使用
    u8 auto_test_flag;          //1:激活自动化测试0：退出自动化测试
    u8 auto_test_led_flag;      //自动测试LED测试
}RunParams;

typedef enum
{
    SYS_SAVE_FACTORY_PARAMS    = 0,   // 出厂参数保存
    SYS_SAVE_DEVICE_PARAMS     = 1,   // 设备参数保存
    SYS_SAVE_SHARE_PARAMS      = 12,  // 分摊参数保存
    SYS_SAVE_RECORD_PARAMS     = 48,  // 记录参数保存
}save_enum;



// 统计参数，需保存到文件系统
typedef struct _SysParams{
    
    /*出产标志*/ //sizeof--sizeof(SysParams)
    u8          factory_flag;/*0*/           //出厂标志
    
    
    
    
    /*设备参数 sizeof--11 */
    u32         Device_SN;/*1*/		        //设备序列号
    u16         Device_DESN;/*5*/	        //速检序号
    u16         Device_GN;/*7*/		        //设备网络组号
    u8          Version_H;/*9*/		        //硬件版本号
    u8          Version_S;/*10*/	        //软件版本号
	u8	        DeviceType;/*11*/	        //设备类型
    
    
    
    
    /*计量分摊参数 sizeof--36 */
    u8	        Buld;/*12*/			        //楼栋号
	u8	        Unit;/*13*/			        //单元号
	u8	        Floor;/*14*/			    //楼层号
	u8	        Cell;/*15*/			        //房间号
	u16	        CellArea;/*16*/		        //房间面积
	RealClock_Str   Start_Time;/*18*/         //供暖开始时间
	RealClock_Str   Over_Time;/*28*/          //供暖结束时间
	RealClock_Str   RecordTime;/*38*/         //最后阀门动作时间

    
    
    
    /*系统记录参数 sizeof--40 */
    //控费相关
    u8	        ControlType;/*48*/	        //用户费控类型 0X00未启用预付费  0XA0 已启用预付费没有欠费  0XAA用户已经欠费
    //阀门控制
    u8          ValveCtrlDemandFlg;/*49*/   //阀门控制命令标志  0x0A 开阀      0xA0关阀
	u8	        ForceCtrl;/*50*/		    //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消
    //温度相关
	u8	        TempCtrlType;/*51*/	        //温度设置类型 0X00 用户设置  0XAA 系统设置   系统设置条件下用户将不能更改温度设置值
	u16	        TempSetValue;/*52*/	        //温度设置值
	u8	        LimtTE_Flag;/*54*/	        //极限温度设置标志 0XAA系统设定极限温度    0XFF系统取消设定极限温度
	u16	        LimtTE_Value;/*55*/	        //极限温度设置值  此值是通过采集器传输得到的，温控器直接赋值为2位小数 则 2555 = 25.55摄氏度
    //热量相关
    u32	        Cycle_OpTim;/*57*/			//周期开阀时间 单位（秒）
    u32	        Apportion_Energy;/*61*/     //周期分摊热量 使用热量 单位（kwh）用户的使用热量或者分摊热量(KWH) 含2位小数
    
    u32         loop_time_59;/*65*/         //周期供暖时间 单位（秒）
	u32	        Remain_Heat;/*69*/	        //剩余热量 单位（kwh）
	u32	        Buy_Total_Heat;/*73*/ 	    //用户购入热量 单位（kwh）
    
    u16         Offect_Temp;/*77*/	        //校准温度点
	s16	        RakeRatioOffect_L;/*79*/	//低区斜率偏差温度折算值每0.001度
	s16	        RakeRatioOffect_H;/*81*/	//高区斜率偏差温度折算值0.001度
	s16	        Offect_Input;/*83*/		    //入口温度偏差
	s16	        Offect_Output;/*85*/		//出口温度偏差
    
    u8          DebugFlag;/*86*/            //DEBUG 打开关闭标志
	struct tm   sys_t;
}SysParams;

//自动测试使用结构体。
typedef struct auto_test_struct{
    char test_0;
    
    u8   test_1;
    s8   test_2;
    
    u16  test_3;
    s16  test_4;
    
    u32  test_5;
    s32  test_6;
    
    int  test_7;
}AutoTestStruct;


#pragma pack()/* 取消指定对齐, 恢复缺省对齐 */


extern SysParams g_sys_params;
extern RunParams g_run_params;
extern AutoTestStruct AutoTest;

//系统复位
void soft_reset(void);

//恢复出厂设置
void sys_factory_reset(void);

//存储参数信息
void save_params(save_enum params_flag);
#endif



