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

// 数据接收处理 MBUS接收 COM1数据调试日志接收  RF数据接收
void com_recv_data_deal()
{
    u8 rv = 0;
    
    // MBUS 总线数据接收
    rv = com_recv_data(COM3, &s_com3_handler.com3_rcv_ptr, &s_com3_handler.com3_rcv_len);
    if (E_COM_OK == rv)
    {
#if DEBUG_SWITCH
        MYLOG_DEBUG_HEXDUMP("【foryon】-> COM3 RECV DATA: ", s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#endif
        rv = fy_protocol_unpack(&s_com3_handler.pkg, s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
        if(rv == FY_PROTOCOL_OK)
        {
            //数据执行 处理
            rv = fy_protocol_func_request(&s_com3_handler.pkg, &g_run_params, &g_sys_params);
            if(rv == 0)
            {
                //反馈 数据
                rv = fy_protocol_func_response(&s_com3_handler.pkg, s_com3_handler.pkg.data, (u8*)&s_com3_handler.pkg_len);
                if(rv == 0)
                {
                    //协议组包
                    rv = fy_protocol_pack(&s_com3_handler.pkg, s_com3_handler.com3_rcv_ptr, (u8*)&s_com3_handler.com3_rcv_len);
                    if(rv == 0)
                    {
                        com_send_data(COM3, s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#if DEBUG_SWITCH
                        MYLOG_DEBUG_HEXDUMP("【foryon】-> COM3 SEND DATA: ", s_com3_handler.com3_rcv_ptr, s_com3_handler.com3_rcv_len);
#endif
                    }
                }
            }
            else
            {
#if DEBUG_SWITCH
                MYLOG_DEBUG("【ERROR】-> COM3 REQUEST ERROR RV:%d\r\n", rv);
#endif
            }
            
        }
        else
        {
#if DEBUG_SWITCH
            MYLOG_DEBUG("【ERROR】-> COM3 UNPACK ERROR RV:%d\r\n", rv);
#endif
        }
        
        com_recv_init(COM3);
    }

    // RF433MHz 无线数据接收
    //温控器发送   3A 70 01 60 99 19 00 00 01 40 C8 13 AA AA 00 92 09 8B 07 89 07 80 5C 00 00 00 27 00 00 00 0A F7 AA
    rv = rf_rcv_data(&s_rf_handler.rf_rcv_ptr, &s_rf_handler.rf_rcv_len);
    if(rv == E_COM_OK)
    {
#if DEBUG_SWITCH
        MYLOG_DEBUG_HEXDUMP("【foryon】-> RF RECV DATA: ", s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#endif
        //数据解包
        rv = fy_protocol_unpack(&s_rf_handler.pkg, s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
        if(rv == FY_PROTOCOL_OK)
        {
            //判断是否是温控器协议
            if(s_rf_handler.pkg.type == 200)
            {
                //数据执行
                rv = fy_protocol_func_request(&s_rf_handler.pkg, &g_run_params, &g_sys_params);
                if(rv == 0)
                {
                    //反馈
                    rv = fy_protocol_func_response(&s_rf_handler.pkg, s_rf_handler.pkg.data, &s_rf_handler.pkg_len);
                    if(rv == 0)
                    {
                        //协议组包
                        rv = fy_protocol_pack(&s_rf_handler.pkg, s_rf_handler.rf_rcv_ptr, (u8*)&s_rf_handler.rf_rcv_len);
                        if(rv == 0)
                        {
                            g_run_params.rf_comm_flag = 1;      //表示建立连接
                            g_run_params.rf_comm_timer = 0;   //阀门和温控器失联计时
                            
                            //数据发送
                            rf_snd_data(s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#if DEBUG_SWITCH
                            MYLOG_DEBUG_HEXDUMP("【foryon】-> RF SEND DATA: ", s_rf_handler.rf_rcv_ptr, s_rf_handler.rf_rcv_len);
#endif
                        }
                    }
                }
                else
                {
#if DEBUG_SWITCH
                    MYLOG_DEBUG("【ERROR】-> RF fy_protocol_func_request ERROR RV:%d\r\n", rv);
#endif
                }
            }
            else
            {
#if DEBUG_SWITCH
                MYLOG_DEBUG("【ERROR】-> RF pkg_type ERROR NO 200\r\n");
#endif
            }
        }
        else
        {
#if DEBUG_SWITCH
            MYLOG_DEBUG("【ERROR】-> RF fy_protocol_unpack ERROR RV:%d\r\n", rv);
#endif
        }
        
        rf_rcv_init();
    }

    // 调试日志接收输出
    rv = com_recv_data(COM1, &s_com1_handler.com1_rcv_ptr, &s_com1_handler.com1_rcv_len);
    if (E_COM_OK == rv)
    {
        //com_send_str(COM1, (char *)s_com1_handler.com1_rcv_ptr);
        printf("【foryon】-> %s\r\n", (char *)s_com1_handler.com1_rcv_ptr);
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
            g_run_params.auto_test_flag = 1;                //开始自动化测试，屏蔽一些和自动化测试冲突任务
            vTaskResume( Task_System_Run_State_Handle );
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "open", 4) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "OPEN", 4) == 0)
        {
            printf("【foryon】-> 打开阀门\r\n");
            valve_triggered();
            g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "close", 5) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "CLOSE", 5) == 0)
        {
            printf("【foryon】-> 关闭阀门\r\n");
            valve_triggered();
            g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
        }else if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug", strlen("debug")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG", strlen("DEBUG")) == 0)
        {
            if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug off", strlen("debug off")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG OFF", strlen("DEBUG OFF")) == 0)
            {
                printf("【foryon】-> 已关闭日志模式\r\n");
                MYLOG_LEVEL(L_NONE);
                g_sys_params.DebugFlag = 0;
                //保存数据
                save_params(SYS_SAVE_FACTORY_PARAMS);
            }
            if(strncmp((char*)s_com1_handler.com1_rcv_ptr, "debug on", strlen("debug on")) == 0 || strncmp((char*)s_com1_handler.com1_rcv_ptr, "DEBUG ON", strlen("DEBUG ON")) == 0)
            {
                MYLOG_LEVEL(L_DEBUG);
                printf("【foryon】-> 已打开日志模式\r\n");
                g_sys_params.DebugFlag = 1;
                //保存数据
                save_params(SYS_SAVE_FACTORY_PARAMS);
            }
            else
            {
                (g_sys_params.DebugFlag == 1) ? printf("【foryon】-> debug on\r\n"):printf("【foryon】-> debug off\r\n");
            }
        }else
        {
            printf("【ERROR】-> 输入指令错误！请重新输入。\r\n");
        }
        com_recv_init(COM1);
    }
    
}



