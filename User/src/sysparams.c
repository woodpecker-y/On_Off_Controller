
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "sysparams.h"

const char *foryon = \
"------------------- ��ӭʹ�� ��Դ�ǿ�ͨ�Ͽ����� -----------------------\r\n\
\r\n\
        888888b  .d88b.  88888b. db    db  .d88b.  d8b   88 \r\n\
        88`     .8P  Y8. 88` `8D `8b  d8` .8P  Y8. 888o  88 \r\n\
        88ooo   88    88 88oobY`  `8bd8`  88    88 88V8o 88 \r\n\
        88999   88    88 88`8b      88    88    88 88 V8o88 \r\n\
        88      `8b  d8` 88 `88.    88    `8b  d8` 88  V888 \r\n\
        88       `Y88P`  88   YD    88     `Y88P`  88   V8P \r\n\
\r\n\
----------------------------------------------- www.foryon.com --------\r\n\
\r\n";

//const char *foryon_log = 
//"            .::\r\n\
//-----------.:----------------------------------------------------------\r\n\
//        .:.:..:   .::    .: .:::.::   .::   .::    .:: .::  \r\n\
//          .::   .::  .::  .::    .:: .::  .::  .::  .::  .::\r\n\
//          .::  .::    .:: .::      .:::  .::    .:: .::  .::\r\n\
//          .::   .::  .::  .::       .::   .::  .::  .::  .::\r\n\
//          .::     .::    .:::      .::      .::    .:::  .::\r\n\
//----------------------------------.::---------- www.foryon.com --------\r\n";

SysParams g_sys_params;
RunParams g_run_params;
AutoTestStruct AutoTest;

//ϵͳ��λ
void soft_reset()
{
    __set_PRIMASK(1); //�����ֹȫ���ж�(����NMI��HardFault)������ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�
    NVIC_SystemReset();//ϵͳ��λ
}

void sys_factory_reset(void)
{
    struct tm t;
    
    rtc_read(&t);
    
    g_sys_params.Device_DESN         = 0x0000;
    g_sys_params.Version_H           = HARDWARE_VERSION;
    g_sys_params.Version_S           = SOFTWARE_VERSION;
    g_sys_params.DeviceType          = 0;                   //�豸����

    //������Ϣ
    g_sys_params.Buld                = 1;                   //¥����
    g_sys_params.Unit                = 1;                   //��Ԫ��
    g_sys_params.Floor               = 1;                   //¥���
    g_sys_params.Cell                = 1;                   //�����
    g_sys_params.CellArea            = 10000;               //100.00 �������,2λС����

    //�¶���Ϣ
    g_sys_params.TempCtrlType        = 0x00;                //�¶��������� 0X00 �û�����  0XAA ϵͳ����
    g_sys_params.TempSetValue        = 2550;                //25.50�� �¶�����ֵ

    g_sys_params.LimtTE_Flag         = 0xFF;                //�����¶����ñ�־ 0xAA ϵͳǿ���趨�¶ȣ� 0xFF ϵͳȡ��ǿ���趨�¶�
    g_sys_params.LimtTE_Value        = 3000;                //30.00�� �����¶�����ֵ

    g_sys_params.Offect_Temp         = 0;                   //У׼�¶ȵ�
    g_sys_params.RakeRatioOffect_L   = 0;                   //����б��ƫ���¶�����ֵÿ0.001��
    g_sys_params.RakeRatioOffect_H   = 0;                   //����б��ƫ���¶�����ֵ0.001��
    g_sys_params.Offect_Input        = 0;                   //����¶�ƫ��
    g_sys_params.Offect_Output       = 0;                   //�����¶�ƫ��

    //������Ϣ
    g_sys_params.ControlType         = 0x00;                //�û��ѿ����� ��4λΪ�ѿط�ʽ  ��4λΪ�ѿ�״̬  0X00:δ����Ԥ����,δǷ��  0XA0:������Ԥ���ѣ�û��Ƿ��  0XAA:������Ԥ���ѣ��û��Ѿ�Ƿ��
    g_sys_params.Cycle_OpTim         = 0;                   //���ڿ���ʱ�� ��λ���룩
    g_sys_params.loop_time_59        = 0;                   //���ڹ�ůʱ�� ��λ���룩
    g_sys_params.Apportion_Energy    = 0;                   //���ڷ�̯���� ʹ������ ��λ��kwh��
    g_sys_params.Remain_Heat         = 0;                   //ʣ������ ��λ��kwh��
    g_sys_params.Buy_Total_Heat      = 0;                   //�û��������� ��λ��kwh��

    //���ſ��ض������
    g_sys_params.ValveCtrlDemandFlg  = VALVE_OPEN;          //���ſ��������־ 0x0A���� 0xA0�ط�
    g_run_params.ValveState          = VALVE_CLOSE;

    g_sys_params.ForceCtrl           = 0xFF;                //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��

    //��ů�ڿ�ʼʱ��
    g_sys_params.Start_Time.Year     = 2019;
    g_sys_params.Start_Time.Month    = 11;
    g_sys_params.Start_Time.Day      = 15;
    g_sys_params.Start_Time.Hour     = 8;
    g_sys_params.Start_Time.Minute   = 0;
    g_sys_params.Start_Time.Second   = 0;
    g_sys_params.Start_Time.Week     = 5;
    //��ů�ڽ���ʱ��
    g_sys_params.Over_Time.Year      = 2020;
    g_sys_params.Over_Time.Month     = 3;
    g_sys_params.Over_Time.Day       = 15;
    g_sys_params.Over_Time.Hour      = 8;
    g_sys_params.Over_Time.Minute    = 0;
    g_sys_params.Over_Time.Second    = 0;
    g_sys_params.Over_Time.Week      = 7;
    //���������ʱ��
    g_sys_params.RecordTime.Year      = t.tm_year + 1900;
    g_sys_params.RecordTime.Month     = t.tm_mon+1;
    g_sys_params.RecordTime.Day       = t.tm_mday;
    g_sys_params.RecordTime.Hour      = t.tm_hour;
    g_sys_params.RecordTime.Minute    = t.tm_min;
    g_sys_params.RecordTime.Second    = t.tm_sec;
    g_sys_params.RecordTime.Week      = t.tm_wday+1;

    g_sys_params.DebugFlag = 0;     //�ر���־ģʽ
    
    if(g_sys_params.factory_flag != 1)
    {
        g_sys_params.sys_t.tm_year    = 2019 - 1900;
        g_sys_params.sys_t.tm_mon     = 10;
        g_sys_params.sys_t.tm_mday    = 15;
        g_sys_params.sys_t.tm_hour    = 8;
        g_sys_params.sys_t.tm_min     = 0;
        g_sys_params.sys_t.tm_sec     = 0;
        g_sys_params.sys_t.tm_wday    = 5;
    }
    
    //������־
    g_sys_params.factory_flag = 1;
    
    //��������
    save_params(SYS_SAVE_FACTORY_PARAMS);
}



void save_params(save_enum params_flag)
{
    u8 rv = 0;
    //u8* save_ptr;
    //u8 sizeof_params = 0;
    ErrorSet error_enum;
    
    //����ڲ��Խ����в����д洢����
    if(g_run_params.auto_test_flag != 0)
        return;
    
    switch(params_flag)
    {
        case SYS_SAVE_FACTORY_PARAMS:
            error_enum = SYS_ERROR_FACTORY_PARAMS;
//            save_ptr = (u8*)&g_sys_params;
//            sizeof_params = sizeof(SysParams);
            break;
        case SYS_SAVE_DEVICE_PARAMS:
            error_enum = SYS_ERROR_DEVICE_PARAMS;
//            save_ptr = (u8*)&g_sys_params.Device_SN;
//            sizeof_params = 11;
            break;
        case SYS_SAVE_SHARE_PARAMS:
            error_enum = SYS_ERROR_SHARE_PARAMS;
//            save_ptr = (u8*)&g_sys_params.Buld;
//            sizeof_params = 36;
            break;
        case SYS_SAVE_RECORD_PARAMS:
            error_enum = SYS_ERROR_RECORD_PARAMS;
//            save_ptr = (u8*)&g_sys_params.ControlType;
//            sizeof_params = 40;
            break;
        default:
           return;
           // break;
    }
    // �ļ�ϵͳ������
    //typedef enum _eFSRet{
    //	E_FS_OK = 0,							  		// �ɹ�
    //	E_FS_ERR_FILE_NUM_EXCEED_MAX,			   		// �ļ����������
    //	E_FS_ERR_HARDWARE,						   		// �ײ�Ӳ���ӿ�
    //	E_FS_ERR_NOT_EXSIT,		  						// �ļ�������
    //	E_FS_ERR_READ_FAIL,					   			// ��ʧ��
    //	E_FS_ERR_WRITE_FAIL,							// дʧ��
    //	E_FS_ERR_NO_RECORD,				   				// �޼�¼
    //	E_FS_ERR_EXCEED_FILE_SIZE,						// �����ļ���С
    //	E_FS_ERR_INVALID_PARAMS,						// �Ƿ��Ĳ���
    //	E_FS_ERR_ALREADY_WRITED,						// �ļ���д��
    //	E_FS_ERR_NOT_MATCH_READ_AFTER_WRITE,			// ��ȡ��д������ݺ�ƥ��
    //}eFSRet;
    //��������
    rv = fs_write(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params, sizeof(SysParams));
    if (rv != E_FS_OK){
        //������������
        error_set(error_enum);
        MYLOG_DEBUG("fs_write[E_FILE_TYPE_SYSINF] failed!Return Code[%d]\r\n", rv);
    }
    else{
        error_clr(error_enum);
    }
    
    return;
}

