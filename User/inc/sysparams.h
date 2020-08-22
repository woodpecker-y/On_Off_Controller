#ifndef _SYS_PARAMS_H
#define _SYS_PARAMS_H

#include "adf.h"
#include "board.h"
#include "valve_deal.h"
#include "error.h"


extern const char *foryon;
//extern const char *foryon_log;

//����ϵͳ���ܲü�
#define DEBUG_SWITCH              0     //������־����
#define ONE_WEEK_SWITCH           1     //һ����ת����
#define OUTAGE_POWER_SWITCH       1     //�ϵ翪������
#define RF_LOSS_OPEN_VALVE_SWITCH 1     //�û�����ģʽ����ʧ����򿪷���  һ��Сʱû�к��¿��������������ʾʧ�����������û�����ģʽ���Զ��򿪣�������ʾʧ��
#define LCD_SWITCH                1     //Һ��������

#define IN_OUT_TEMP_SWITCH        0     //����ˮ�¶ȿ���
#define WHOLE_SWITCH              1     //һ�巧���� (ֻ�������ֲ�ͬ��Ӳ���汾��)
    
//��Ӳ���汾��
/*ͨ�Ͽ������汾��¼��
    ���¶�̽ͷ��   H��13   S��62
    �����¶�̽ͷ�� H��13   S��61
    
    һ�巧��ͨ�Ͽ������汾��¼��
    ���¶�̽ͷ��   H��11   S��62
    �����¶�̽ͷ�� H��11   S��61
*/

/*------------ Ӳ���汾��  ------------*/
#if WHOLE_SWITCH
    //һ�巧ͨ�Ͽ�����Ӳ���汾
    #define HARDWARE_VERSION      11    //Ӳ���汾�� 11 ��ʾV1.1
#else
    //ͨ�Ͽ�����Ӳ���汾
    #define HARDWARE_VERSION      13    //Ӳ���汾�� 11 ��ʾV1.1
#endif

/*------------ ����汾��  ------------*/
#if IN_OUT_TEMP_SWITCH
    //���¶�̽ͷ������汾
    #define SOFTWARE_VERSION      62    //����汾�� 60 Ϊ�����ϰ汾ͨ�Ϸ�Э��
#else
    //�����¶�̽ͷ������汾
    #define SOFTWARE_VERSION      61    //����汾�� 60 Ϊ�����ϰ汾ͨ�Ϸ�Э��
#endif

    
    
//ʱ��궨��
#define UNIT_SECOND	              1UL
#define UNIT_MIN		          (60*UNIT_SECOND)
#define UNIT_HOUR                 (60*UNIT_MIN)
#define UNIT_DAY                  (24*UNIT_HOUR)

//ϵͳ�궨��
//һ����ת���ܺ�
#define ONE_WEEK_TURN_TIME         7*UNIT_DAY
//LCD��ʾʱ�� ��λΪs   ��
#define LCD_DISP_DELAY             3   


#pragma pack(1)                 /* ָ����n�ֽڶ��� */

typedef enum
{
	State_OK    = 0x00,
	State_Error = 0xAA
} State_TypeDef;

//ʵʱʱ�ӽṹ��
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

 //����״̬
 typedef enum
{
	//�������� ����
	FORCE_NOR		= 0X00,	    //��ǿ��״̬
	FORCE_CLOSE		= 0XAA,	    //ǿ�ƹط�
	FORCE_OPEN		= 0X55,	    //ǿ�ƿ���
	
	VALVE_NOR		= 0X00,	    //�����޶�������
	
	//��״̬(λ��) �������� ����
	VALVE_OPEN	    = 0X0A,	    //����	��������־
	VALVE_MIDDLE	= 0X55,	    //�м�״̬
	VALVE_CLOSE	    = 0XA0,	    //�ط�	��������־
	VALVE_BREAK	    = 0XFF,	    //����״̬
	
//	SW_ON		    = 0XAA,	    //����״̬-�պ�
//	SW_OFF		    = 0X00	    //����״̬-�Ͽ�
}Motor_Type;

// ���в������������ڴ���
typedef struct _RunParams{
    // �¶����
	u16	Cur_Temp;				//��ǰ�����¶�     ��ֵ��ͨ���¿�������õ��ģ��¿���ֱ�Ӹ�ֵΪ2λС�� �� 2555 = 25.55���϶�
	u16	Ave_Temp;				//ƽ���¶�         ��ֵ��ͨ���¿�������õ��ģ��¿���ֱ�Ӹ�ֵΪ2λС�� �� 2555 = 25.55���϶�
	s16	Input_Temp;			    //��ˮ�¶�  255=25.5��  Э�����Ǳ���2λС���� �� 2550 = 25.50���϶� ��Э����ֱ�ӳ���10����
	s16	Output_Temp;			//��ˮ�¶�  255=25.5��  Э�����Ǳ���2λС���� �� 2550 = 25.50���϶� ��Э����ֱ�ӳ���10����
    u8	Face_State;	            //������״̬    ������0X01  ͨ�Ź���0X80
    
    // �������
    u8  ValveState;             //��������״̬  0X55�м�״̬  0X00:�м�״̬  0X0A:����  0XA0:�ط�
    u8  ValvEerror;             //���Ź���״̬  0xFF(��ת������΢������ͬʱ�պϡ�����оƬ��)     0x00����
    u8  Plug_valve_state;       //���Ŷ�ת״̬  0x05������ת  0x50�ط���ת   0x55ȫ����ת   0x00�޶�ת
    u16 All_open_delay;         //���Ź㲥�����ȴ� ���䶯����ʱ��λ����
    u16 All_close_delay;        //���Ź㲥�ط��ȴ� ���䶯����ʱ��λ����
    
    // LED���
    Valve_led_twinkle valve_led_action;       //����led����ʾ״̬
    
    //RF433����
    u16  rf_comm_timer;           //�����Ƿ������Ӽ�ʱ��
    u16  rf_comm_flag;          //�����߱�����ϵ״̬ 0ʧ����1����
    
    
    //���ϱ�ʶ
//	u8	FactPara_StateErr;		//������������״ָ̬ʾ��
//	u8	DevPara_StateErr;		//�豸��������״ָ̬ʾ��
//	u8	ShareCtrl_StateErr;		//��̯��������״ָ̬ʾ��
//	u8	Record_StateErr;		//��¼��������״ָ̬ʾ��	
    
                                //0x80RTC���ϡ�        0x40:��ڴ��������ϡ� 0x20:�����¶ȹ��ϡ� 0x10:�����������ϡ�
                                //0x08:������������   0x04:�ò�������     0x02:��̯�������� 0x01:��¼��������
	u8	DevErrState;			//�豸����״ָ̬ʾ��   λ��־   �����ϴ�����ʾ  
//	u8 VavleReceiveFaceState;	//���Ž������״̬��  0xAA  ��һСʱδ���¿����ͨѶ�� 0 ���������1Сʱ����ͨѶ

    //�������
    u8  WorkFlg;                //��ů����־   ��ǰ״̬ �Ƿ��ڹ�ů�� 
    u32 heating_timer;        //��⹩ů�ڼ�ʱ��
	u32 Com_OK;				    //ͨѶ�ɹ�����
	u32 Com_Total;				//ͨѶ�ܴ���
    u16 save_cycle_optim_timer; //��ʱ10���ӱ���һ�ο���ʱ�� 
    u32 one_week_turn_timer;          //һ����ת
    
    //�ϵ翪����־
    u8 PowerDownFlag;           //�����־  0�ϵ磬1�ϵ�
    
    u32 Rtc_check_sec;          //����ʱ������
    u32 Rtc_check_timer;        //RTC��ⶨʱ��
    u32 Check_time;             //�Զ����RTCʱ��   5 �� UNIT_HOUR/2
    u8  Deviation;
//	u8	RTC_State;				//ʵʱʱ��״̬    ������־RTC�Ľ���״̬  ע��:ֻ����Ӳ����ʱ�͸�λ��ʱʱ�ñ�־�Żᱻ����
//	u8	SensorIN_State;			//��ڴ���������״ָ̬ʾ��
//	u8	SensorOUT_State;		//���ڴ���������״ָ̬ʾ�� 
//	u8	Motor_State;			//������������״ָ̬ʾ��	


//�Զ�������ʹ��
    u8 auto_test_flag;          //1:�����Զ�������0���˳��Զ�������
    u8 auto_test_led_flag;      //�Զ�����LED����
}RunParams;

typedef enum
{
    SYS_SAVE_FACTORY_PARAMS    = 0,   // ������������
    SYS_SAVE_DEVICE_PARAMS     = 1,   // �豸��������
    SYS_SAVE_SHARE_PARAMS      = 12,  // ��̯��������
    SYS_SAVE_RECORD_PARAMS     = 48,  // ��¼��������
}save_enum;



// ͳ�Ʋ������豣�浽�ļ�ϵͳ
typedef struct _SysParams{
    
    /*������־*/ //sizeof--sizeof(SysParams)
    u8          factory_flag;/*0*/           //������־
    
    
    
    
    /*�豸���� sizeof--11 */
    u32         Device_SN;/*1*/		        //�豸���к�
    u16         Device_DESN;/*5*/	        //�ټ����
    u16         Device_GN;/*7*/		        //�豸�������
    u8          Version_H;/*9*/		        //Ӳ���汾��
    u8          Version_S;/*10*/	        //����汾��
	u8	        DeviceType;/*11*/	        //�豸����
    
    
    
    
    /*������̯���� sizeof--36 */
    u8	        Buld;/*12*/			        //¥����
	u8	        Unit;/*13*/			        //��Ԫ��
	u8	        Floor;/*14*/			    //¥���
	u8	        Cell;/*15*/			        //�����
	u16	        CellArea;/*16*/		        //�������
	RealClock_Str   Start_Time;/*18*/         //��ů��ʼʱ��
	RealClock_Str   Over_Time;/*28*/          //��ů����ʱ��
	RealClock_Str   RecordTime;/*38*/         //����Ŷ���ʱ��

    
    
    
    /*ϵͳ��¼���� sizeof--40 */
    //�ط����
    u8	        ControlType;/*48*/	        //�û��ѿ����� 0X00δ����Ԥ����  0XA0 ������Ԥ����û��Ƿ��  0XAA�û��Ѿ�Ƿ��
    //���ſ���
    u8          ValveCtrlDemandFlg;/*49*/   //���ſ��������־  0x0A ����      0xA0�ط�
	u8	        ForceCtrl;/*50*/		    //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��
    //�¶����
	u8	        TempCtrlType;/*51*/	        //�¶��������� 0X00 �û�����  0XAA ϵͳ����   ϵͳ�����������û������ܸ����¶�����ֵ
	u16	        TempSetValue;/*52*/	        //�¶�����ֵ
	u8	        LimtTE_Flag;/*54*/	        //�����¶����ñ�־ 0XAAϵͳ�趨�����¶�    0XFFϵͳȡ���趨�����¶�
	u16	        LimtTE_Value;/*55*/	        //�����¶�����ֵ  ��ֵ��ͨ���ɼ�������õ��ģ��¿���ֱ�Ӹ�ֵΪ2λС�� �� 2555 = 25.55���϶�
    //�������
    u32	        Cycle_OpTim;/*57*/			//���ڿ���ʱ�� ��λ���룩
    u32	        Apportion_Energy;/*61*/     //���ڷ�̯���� ʹ������ ��λ��kwh���û���ʹ���������߷�̯����(KWH) ��2λС��
    
    u32         loop_time_59;/*65*/         //���ڹ�ůʱ�� ��λ���룩
	u32	        Remain_Heat;/*69*/	        //ʣ������ ��λ��kwh��
	u32	        Buy_Total_Heat;/*73*/ 	    //�û��������� ��λ��kwh��
    
    u16         Offect_Temp;/*77*/	        //У׼�¶ȵ�
	s16	        RakeRatioOffect_L;/*79*/	//����б��ƫ���¶�����ֵÿ0.001��
	s16	        RakeRatioOffect_H;/*81*/	//����б��ƫ���¶�����ֵ0.001��
	s16	        Offect_Input;/*83*/		    //����¶�ƫ��
	s16	        Offect_Output;/*85*/		//�����¶�ƫ��
    
    u8          DebugFlag;/*86*/            //DEBUG �򿪹رձ�־
	struct tm   sys_t;
}SysParams;

//�Զ�����ʹ�ýṹ�塣
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


#pragma pack()/* ȡ��ָ������, �ָ�ȱʡ���� */


extern SysParams g_sys_params;
extern RunParams g_run_params;
extern AutoTestStruct AutoTest;

//ϵͳ��λ
void soft_reset(void);

//�ָ���������
void sys_factory_reset(void);

//�洢������Ϣ
void save_params(save_enum params_flag);
#endif



