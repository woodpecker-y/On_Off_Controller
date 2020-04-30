#include "error.h"
#include "sysparams.h"

//typedef enum _ErrorSet{
//    SYS_ERROR_RECORD_PARAMS     = 0X01,  // ��¼��������
//    SYS_ERROR_SHARE_PARAMS      = 0X02,  // ��̯��������
//    SYS_ERROR_DEVICE_PARAMS     = 0X04,  // �豸��������
//    SYS_ERROR_FACTORY_PARAMS    = 0X08,  // ������������
//    SYS_ERROR_VALVE             = 0X10,  // �����������ϡ�
//    SYS_ERROR_OUTPUT_TEMP       = 0X20,  // �����¶ȹ��ϡ�
//    SYS_ERROR_INPUT_TEMP        = 0X40,  // ��ڴ��������ϡ�
//    SYS_ERROR_RTC               = 0X80,  // RTC���ϡ�
//}ErrorSet;


//д����
void error_set(u8 code)
{
    g_run_params.DevErrState |= code;
}

//�������
void error_clr(u8 code)
{
    g_run_params.DevErrState &= ~code;
}


