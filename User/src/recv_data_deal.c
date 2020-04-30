#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "app_task.h"
#include "bsp.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"
#include "recv_data_deal.h"
#include "vTask_Com_Rf_Valve.h"

Com1Handler s_com1_handler;
Com3Handler s_com3_handler;
RfHandler   s_rf_handler;

// ���ݽ��մ��� MBUS���� COM1���ݵ�����־����  RF���ݽ���
void com_recv_data_deal()
{
    u8 rv = 0;
    
    // MBUS �������ݽ���
    rv = com_recv_data(COM3, &s_com3_handler.com3_rcv_ptr, &s_com3_handler.com3_rcv_len);
    if (E_COM_OK == rv)
    {
#if DEBUG_SWITCH
        MYLOG_DEBUG_HEXDUMP("��foryon��-> COM3 RECV DATA: ", s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#endif
        rv = fy_protocol_unpack(&s_com3_handler.pkg, s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
        if(rv == FY_PROTOCOL_OK)
        {
            //����ִ�� ����
            rv = fy_protocol_func_request(&s_com3_handler.pkg, &g_run_params, &g_sys_params);
            if(rv == 0)
            {
                //���� ����
                rv = fy_protocol_func_response(&s_com3_handler.pkg, s_com3_handler.pkg.data, (u8*)&s_com3_handler.pkg_len);
                if(rv == 0)
                {
                    //Э�����
                    rv = fy_protocol_pack(&s_com3_handler.pkg, s_com3_handler.com3_rcv_ptr, (u8*)&s_com3_handler.com3_rcv_len);
                    if(rv == 0)
                    {
                        com_send_data(COM3, s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#if DEBUG_SWITCH
                        MYLOG_DEBUG_HEXDUMP("��foryon��-> COM3 SEND DATA: ", s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#endif
                    }
                }
            }
            else
            {
#if DEBUG_SWITCH
                MYLOG_DEBUG("��ERROR��-> COM3 REQUEST ERROR RV:%d\r\n", rv);
#endif
            }
            
        }
        else
        {
#if DEBUG_SWITCH
            MYLOG_DEBUG("��ERROR��-> COM3 UNPACK ERROR RV:%d\r\n", rv);
#endif
        }
        
        com_recv_init(COM3);
    }

    // RF433MHz �������ݽ���
    //�¿�������   3A 70 01 60 99 19 00 00 01 40 C8 13 AA AA 00 92 09 8B 07 89 07 80 5C 00 00 00 27 00 00 00 0A F7 AA
    rv = rf_rcv_data(&s_rf_handler.rf_rcv_ptr, &s_rf_handler.rf_rcv_len);
    if(rv == E_COM_OK)
    {
#if DEBUG_SWITCH
        MYLOG_DEBUG_HEXDUMP("��foryon��-> RF RECV DATA: ", s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#endif
        //���ݽ��
        rv = fy_protocol_unpack(&s_rf_handler.pkg, s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
        if(rv == FY_PROTOCOL_OK)
        {
            //�ж��Ƿ����¿���Э��
            if(s_rf_handler.pkg.type == 200)
            {
                //����ִ��
                rv = fy_protocol_func_request(&s_rf_handler.pkg, &g_run_params, &g_sys_params);
                if(rv == 0)
                {
                    //����
                    rv = fy_protocol_func_response(&s_rf_handler.pkg, s_rf_handler.pkg.data, &s_rf_handler.pkg_len);
                    if(rv == 0)
                    {
                        //Э�����
                        rv = fy_protocol_pack(&s_rf_handler.pkg, s_rf_handler.rf_rcv_ptr, (u8*)&s_rf_handler.rf_rcv_len);
                        if(rv == 0)
                        {
                            g_run_params.rf_comm_flag = 1;      //��ʾ��������
                            g_run_params.rf_comm_timer = 0;   //���ź��¿���ʧ����ʱ
                            
                            //���ݷ���
                            rf_snd_data(s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#if DEBUG_SWITCH
                            MYLOG_DEBUG_HEXDUMP("��foryon��-> RF SEND DATA: ", s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#endif
                        }
                    }
                }
                else
                {
#if DEBUG_SWITCH
                    MYLOG_DEBUG("��ERROR��-> RF fy_protocol_func_request ERROR RV:%d\r\n", rv);
#endif
                }
            }
            else
            {
#if DEBUG_SWITCH
                MYLOG_DEBUG("��ERROR��-> RF pkg_type ERROR NO 200\r\n");
#endif
            }
        }
        else
        {
#if DEBUG_SWITCH
            MYLOG_DEBUG("��ERROR��-> RF fy_protocol_unpack ERROR RV:%d\r\n", rv);
#endif
        }
        
        rf_rcv_init();
    }

    // ������־�������
    rv = com_recv_data(COM1, &s_com1_handler.com1_rcv_ptr, &s_com1_handler.com1_rcv_len);
    if (E_COM_OK == rv)
    {
        //com_send_str(COM1, (char *)s_com1_handler.com1_rcv_ptr);
        printf("��foryon��-> %s\r\n", (char *)s_com1_handler.com1_rcv_ptr);
        //com_send_data(COM1, rcv_buf, rcv_len);
        //MYLOG_DEBUG_HEXDUMP("COM RECV - rcv_dat: ", s_com1_handler.com1_rcv_ptr, s_com1_handler.com1_rcv_len);
        //rf_snd_data(s_com1_handler.com1_rcv_ptr, s_com1_handler.com1_rcv_len);
        if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "ps", 2) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "PS", 2) == 0)
        {
            vTaskResume( Task_System_Run_State_Handle );
#if DEBUG_SWITCH
            system_print();
#endif
        }
        else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "test", 2) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "TEST", 2) == 0)
        {
            g_run_params.auto_test_flag = 1;                //��ʼ�Զ������ԣ�����һЩ���Զ������Գ�ͻ����
            vTaskResume( Task_System_Run_State_Handle );
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "open", 4) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "OPEN", 4) == 0)
        {
            printf("��foryon��-> �򿪷���\r\n");
            valve_triggered();
            g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "close", 5) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "CLOSE", 5) == 0)
        {
            printf("��foryon��-> �رշ���\r\n");
            valve_triggered();
            g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug", strlen("debug")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG", strlen("DEBUG")) == 0)
        {
            if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug off", strlen("debug off")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG OFF", strlen("DEBUG OFF")) == 0)
            {
                printf("��foryon��-> �ѹر���־ģʽ\r\n");
                MYLOG_LEVEL(L_NONE);
                g_sys_params.DebugFlag = 0;
                //��������
                save_params(SYS_SAVE_FACTORY_PARAMS);
            }
            if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug on", strlen("debug on")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG ON", strlen("DEBUG ON")) == 0)
            {
                MYLOG_LEVEL(L_DEBUG);
                printf("��foryon��-> �Ѵ���־ģʽ\r\n");
                g_sys_params.DebugFlag = 1;
                //��������
                save_params(SYS_SAVE_FACTORY_PARAMS);
            }
            else
            {
                (g_sys_params.DebugFlag == 1) ? printf("��foryon��-> debug on\r\n"):printf("��foryon��-> debug off\r\n");
            }
        }else
        {
            printf("��ERROR��-> ����ָ��������������롣\r\n");
        }
        com_recv_init(COM1);
    }
    
}



