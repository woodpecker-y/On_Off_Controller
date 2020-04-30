#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"

#include "adf.h"
#include "sysparams.h"
#include "fy_protocol.h"
#include "bsp.h"
#include "valve_deal.h"
#include "vTask_Com_Rf_Valve.h"

int data_trim_int(unsigned char *res, int *res_len);

/*! \brief 
*      FY_PROTOCOL协议组包
* \param pkg[IN]                - FY_PROTOCOL协议帧定义结构体
* \param packaged_data[IN]      - 组包后的数据包
* \param packaged_data_size[IN] - 组包后的数据包长度
* 
* \return  
*      FY_PROTOCOL_OK                   - 成功
*      FY_PROTOCOL_ERR_INVALID_STX_ETX  - 帧头、帧尾错误
*      FY_PROTOCOL_ERR_CHECKSUM         - 校验和错误
*      FY_PROTOCOL_ERR_LENGTH           - 长度错误
* \note  
* 
*/
unsigned char fy_protocol_pack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, u8 *packaged_data_size)
{
    unsigned char offset = 0;
    
    //assert_param(*packaged_data_size > pkg->msg_size+10);
    
    // 帧头
    packaged_data[offset] = FY_PROTOCOL_STX;
    offset += 1;
    
    // 仪表类型
    packaged_data[offset] = pkg->ver;
    offset += 1;
    
    // SN 仪表地址
    //memcpy(packaged_data+offset, &(pkg->sn), sizeof(unsigned int));
    memcpy(packaged_data+offset, &g_sys_params.Device_SN, sizeof(unsigned int));
    //bytes_reverse(packaged_data+offset, sizeof(unsigned int));
    offset += sizeof(unsigned int);
    
    // DESN 网络成员编号
    //packaged_data[offset] = pkg->desn;
    packaged_data[offset] = g_sys_params.Device_DESN;
    offset += 2;
    
    // GN 网络组号
    //memcpy(packaged_data+offset, &(pkg->desn), 2);
    memcpy(packaged_data+offset, &g_sys_params.Device_GN, 2);
    //bytes_reverse(packaged_data+offset, 2);
    offset += 2;
    
    // TYPE 网络成员编号
    packaged_data[offset] = pkg->type;
    offset += 1;
    
    // LENTH 网络成员编号
    packaged_data[offset] = pkg->lenth;
    offset += 1;
    
    // DATA 数据
    memcpy(packaged_data+offset, pkg->data, pkg->lenth);
    offset += pkg->lenth;
    
    // 校验和
    packaged_data[offset] = check_sum(packaged_data, offset);
    offset += 1;
    
    // 帧尾
    packaged_data[offset] = FY_PROTOCOL_ETX;
    offset += 1;
    
    *packaged_data_size = offset;
    
    return 0;
}

/*! \brief 
*      FY_PROTOCOL协议解包
* \param pkg[IN]                    - 解包后存储数据结构
* \param packaged_data[IN]          - 原始报文
* \param packaged_data_size[IN]     - 原始报文长度
* 
* \return  
*      FY_PROTOCOL_OK                   - 成功
*      FY_PROTOCOL_ERR_INVALID_STX_ETX  - 帧头、帧尾错误
*      FY_PROTOCOL_ERR_CHECKSUM         - 校验和错误
*      FY_PROTOCOL_ERR_LENGTH           - 长度错误
* \note  
* 
*/
unsigned char fy_protocol_unpack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, int packaged_data_size)
{
    unsigned int  device_sn = 0;
    unsigned char offset = 0;
    unsigned char sum    = 0;
    
    //data_trim_int(packaged_data, &packaged_data_size);
    
    // 帧头、帧尾检查
    if (packaged_data[0]!=FY_PROTOCOL_STX || packaged_data[packaged_data_size-1]!=FY_PROTOCOL_ETX)
    {
        return FY_PROTOCOL_ERR_INVALID_PKG;
    }
    
    // 判断协议类型
    if(packaged_data[1] != 0x70)
    {
        return FY_PROTOCOL_ERR_UNKNOWN_DATA_FLAG;       //未知的数据标识
    }
    
    // 判断SN是否是此设备设备编号
    bytes_reverse(packaged_data+2, 4);
    device_sn = hex_2_dec_type(packaged_data+2, 4, ORD_MOTOR);
    if(device_sn == g_sys_params.Device_SN || device_sn == 0xAAAAAAAA)
    {
        if(device_sn == 0xAAAAAAAA)
        {
            //所有支持广播的协议命令
            if( packaged_data[10]!= 0 && packaged_data[10]!=50 && packaged_data[10]!=51 && packaged_data[10]!=52 && \
                packaged_data[10]!=53 && packaged_data[10]!=54 && packaged_data[10]!=55 && packaged_data[10]!=61 && \
                packaged_data[10]!=62 && packaged_data[10]!=63 && packaged_data[10]!=80 && packaged_data[10]!=81 && \
                packaged_data[10]!=82 && packaged_data[10]!=83 && packaged_data[10]!=85 && packaged_data[10]!=86 )
                return FY_PROTOCOL_ERR_ADDR;//地址不匹配
        }
        else if(device_sn == g_sys_params.Device_SN)
        {
            //所有不支持广播的协议命令
            if( packaged_data[10]!=0 && packaged_data[10]!=59 && packaged_data[10]!=60 && packaged_data[10]!=200)
                return FY_PROTOCOL_ERR_ADDR;//地址不匹配
        }
    }
    else
        return FY_PROTOCOL_ERR_ADDR;//地址不匹配
    
    // 校验和检查
    sum = check_sum(packaged_data, packaged_data_size-2);
    if (sum != packaged_data[packaged_data_size-2])
    {
        return FY_PROTOCOL_ERR_CHECKSUM;                //校验和错误
    }

    // 长度判断
    if (packaged_data[11] != packaged_data_size-14)
    {
        return FY_PROTOCOL_ERR_LENGTH;                  //长度错误
    }
    
    offset = 0;
    // 跳过帧头
    offset += 1;
    
    // VER 协议类型
    pkg->ver = packaged_data[offset];
    offset += 1;
    
    // SN 地址码
    //bytes_reverse(packaged_data+offset, sizeof(unsigned int));
    //pkg->sn = hex_2_dec_type(packaged_data+offset, 4, ORD_MOTOR);
    //offset += sizeof(unsigned int);
    
    // SN 地址码
    pkg->sn = device_sn;
    offset += 4;
    
    // DESN 网络成员编号
    pkg->desn = packaged_data[offset];
    offset += 2;
    
    // GN 网络组号
    //bytes_reverse(packaged_data+offset, 2);
    //pkg->gn = hex_2_dec_type(packaged_data, 2, ORD_MOTOR);
    memcpy(&pkg->gn, packaged_data+offset, 2);
    offset += 2;

    // TYPE 协议编码
    pkg->type = packaged_data[offset];
    offset += 1;

    // LENTH 协议编码
    pkg->lenth = packaged_data[offset];
    offset += 1;

    // DATA 数据负载
    memcpy(pkg->data, packaged_data+offset, pkg->lenth);
    offset += pkg->lenth;
    
    return 0;
}

// 数据过滤、截取 去掉前导码FFFFFF 等信息
int data_trim_int(unsigned char *res, int *res_len)
{
    unsigned int i = 0;
    unsigned int j = 0;
    
    if (*res_len == 0x00)
        return -1;
    
    //while (res[i] != 0x68 && i < *res_len)    //以3A开头的数据协议包
    while (res[i] != 0x3A && i < *res_len)
    {
        ++i;
    }
    
    if (i == *res_len)
    {
        return -2;
    }
    
    j = *res_len-1;
    
    //while (res[j] != 0x16 && j > i)
    while (res[j] != 0xAA && j > i)         //以AA结尾的数据协议包
    {
        --j;
    }
    
    if (j == i)
    {
        return -3;
    }
    
    memcpy(res, res+i, j - i + 1);
    *res_len = j - i + 1;
    
    return 0;
}



//-------------------协议处理----------------------

// 根据协议中 TYPE 判断工作内容
u8 fy_protocol_func_request(FY_PROTOCOLPkg *pkg, RunParams *run_params, SysParams *sys_params)
{
    //u8  sn_temp[4] = {0};
    //u8  gn_temp[2] = {0};

    struct tm t;
    
    g_sys_params.Device_DESN = pkg->desn;//设备在系统中的编号
    
    switch (pkg->type)
    {
        case 0:     // 出厂参数配置命令(调试专用命令)  1:返回
        {
            //memcpy(sn_temp, pkg->data, 4);
            // bytes_reverse(sn_temp, 4);
            //g_sys_params.Device_SN = hex_2_dec_type(sn_temp,   sizeof(sn_temp), ORD_MOTOR);
            bytes_reverse(pkg->data, 4);
            sys_params->Device_SN = hex_2_dec_type(pkg->data, 4, ORD_MOTOR);
        
            //memcpy(gn_temp, pkg->data+4, 2);
            //bytes_reverse(gn_temp, 2);
            //g_sys_params.Device_GN = hex_2_dec_type(gn_temp, sizeof(gn_temp), ORD_MOTOR);
            bytes_reverse(pkg->data+4, 2);
            sys_params->Device_GN = hex_2_dec_type(pkg->data+4, 2, ORD_MOTOR);
        
            sys_params->Version_H = pkg->data[6];
            sys_params->Version_S = pkg->data[7];
            
            //保存设备参数数据
            save_params(SYS_SAVE_DEVICE_PARAMS);
            
            rf433_init();
            
            pkg->type = 1;
        }
            break;
        case 50:    // （广播方式） 不反馈 主机广播发送时钟校准命令
        {
            bytes_reverse(pkg->data, 2);
            t.tm_year   = hex_2_dec_type(pkg->data, 2, ORD_MOTOR)-1900;
            t.tm_mon    = pkg->data[2]-1;
            t.tm_mday   = pkg->data[3];
            t.tm_hour   = pkg->data[4];
            t.tm_min    = pkg->data[5];
            t.tm_sec    = pkg->data[6];
            t.tm_wday   = pkg->data[7];
            rtc_write(t);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//检测供暖期
        }
            break;
        case 51:    // （广播方式） 不反馈 主机广播发送全部开阀命令
        {
            //u8   hundred     = 0;
            //u8   decade      = 0;
            //u8   unit        = 0;
            
            bytes_reverse(pkg->data, 2);
//            unit    = (sys_params->Device_SN  & 0x0000000F);
//            decade  = ((sys_params->Device_SN & 0x000000F0)>>4)*10;
//            hundred = ((sys_params->Device_SN & 0x00000F00)>>8)*100;
//            run_params->All_open_delay = (hundred + decade + unit) * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);// 阀门广播开阀等待 阀间动作延时单位毫秒
            
            // 阀门广播开阀等待 阀间动作延时单位毫秒
            run_params->All_open_delay = sys_params->Device_DESN * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            s_valve_handler.all_open_timer = run_params->All_open_delay/50;
            
            //广播开阀指令
            g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
            valve_triggered();
            
            //保存系统记录参数
            save_params(SYS_SAVE_RECORD_PARAMS);
        }
            break;
        case 52:    // （广播方式） 不反馈 主机广播发送全部关阀命令
        {
            //u8   hundred     = 0;
            //u8   decade      = 0;
            //u8   unit        = 0;
            
            bytes_reverse(pkg->data, 2);
//            unit    = (sys_params->Device_SN  & 0x0000000F);
//            decade  = ((sys_params->Device_SN & 0x000000F0)>>4)*10;
//            hundred = ((sys_params->Device_SN & 0x00000F00)>>8)*100;
//            run_params->All_close_delay = (hundred + decade + unit) * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);// 阀门广播开阀等待 阀间动作延时单位毫秒
            
            // 阀门广播开阀等待 阀间动作延时单位毫秒
            run_params->All_close_delay = sys_params->Device_DESN * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            s_valve_handler.all_close_timer = run_params->All_close_delay/50;
            //广播关阀指令
            g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
            valve_triggered();
            
            //保存系统记录参数
            save_params(SYS_SAVE_RECORD_PARAMS);
        }
            break;
        case 53:    // （广播方式） 不反馈 主机发送开阀、关阀、强制锁定等动作命令（广播方式）
        {
            //如果系统没有错误则执行，否则在不执行
            //if(g_run_params.DevErrState == 0x00)
            {
                //阀门控制命令标志 0x0A开阀 0xA0关阀
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                }
                
                if( pkg->data[1]==0x55 || pkg->data[1]==0xAA || pkg->data[1]==0xFF)
                {
                    sys_params->ForceCtrl = pkg->data[1];//阀门强制控制标志  0xAA强制开阀   0x55强制关阀
                }
                
                valve_triggered();
                
                //保存系统记录参数
                save_params(SYS_SAVE_RECORD_PARAMS);
            }
        }
            break;
        case 54:    //  （支持广播方式 和 ID校验模式）开阀、关阀、强制锁定等动作命令          128:返回
        {
            //强制控制 主要是相对 温控器的，协议是都可以控制阀门开关的，但是强制开或关的话温控器就不能控制阀门了。
            if(pkg->data[1]==0xAA)//强制
            {
                //阀门控制命令标志 0x0A开阀 0xA0关阀
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                    if(sys_params->ValveCtrlDemandFlg ==0x0A)
                        sys_params->ForceCtrl = 0x55;       //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消
                    else if(sys_params->ValveCtrlDemandFlg ==0xA0)
                        sys_params->ForceCtrl = 0xAA;       //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消
                }
            }
            else
            {
                //阀门控制命令标志 0x0A开阀 0xA0关阀
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                }
                
                sys_params->ForceCtrl = 0xFF;
            }
            
            //保存数据
            save_params(SYS_SAVE_RECORD_PARAMS);
            
            valve_triggered();
            
            pkg->type = 128;
        }
            
            break;
        case 55:    // （支持广播方式 和 ID校验模式） 终端时间校准命令                       129:返回
        {
            bytes_reverse(pkg->data, 2);
            t.tm_year   = hex_2_dec_type(pkg->data, 2, ORD_MOTOR)-1900;
            t.tm_mon    = pkg->data[2]-1;
            t.tm_mday   = pkg->data[3];
            t.tm_hour   = pkg->data[4];
            t.tm_min    = pkg->data[5];
            t.tm_sec    = pkg->data[6];
            t.tm_wday   = pkg->data[7];
            rtc_write(t);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//检测供暖期
            
            pkg->type = 129;
        }
            break;
        case 59:    // 数据采集命令                           132:返回
        {
            //u8 data_temp[] = {0xAA, 0xAA};
            
            //if(memcmp(pkg->data, data_temp, 2) == 0)
            {
                //周期供暖时间（采集器自行运算）
                bytes_reverse(pkg->data+2, 4);
                sys_params->loop_time_59 = hex_2_dec_type(pkg->data+2, 4, ORD_MOTOR);
                
                //用户的使用热量或者分摊热量(KWH) 含2位小数
                bytes_reverse(pkg->data+6, 4);
                sys_params->Apportion_Energy = hex_2_dec_type(pkg->data+6, 4, ORD_MOTOR);
                
                //剩余热量(KWH)含2位小数
                bytes_reverse(pkg->data+10, 4);
                sys_params->Remain_Heat = hex_2_dec_type(pkg->data+10, 4, ORD_MOTOR);
                
                //总热量 用户购买热量含2位小数
                bytes_reverse(pkg->data+14, 4);
                sys_params->Buy_Total_Heat = hex_2_dec_type(pkg->data+14, 4, ORD_MOTOR);
                
                //费控类型
                if(pkg->data[18]==0 || pkg->data[18]==0xA0 || pkg->data[18]==0xAA)
                {
                    sys_params->ControlType = pkg->data[18];
                }
                
                //强制阀门标志   //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消
                if(pkg->data[19]==0x55 || pkg->data[19]==0xAA || pkg->data[19]==0xFF)
                {
                    //强制控制模式
                    sys_params->ForceCtrl = pkg->data[19];
                    //阀门动作 //阀门控制命令标志 0x0A开阀 0xA0关阀                  
                    if(sys_params->ForceCtrl == 0x55)
                    {
                        if(run_params->ValveState != 0x0A)//如果现在阀门不是打开的情况下才执行开阀动作
                        {
                            sys_params->ValveCtrlDemandFlg = 0x0A;
                            valve_triggered();
                        }
                    }
                    else if(sys_params->ForceCtrl == 0xAA)
                    {
                        if(run_params->ValveState != 0xA0)//如果现在阀门不是关闭的情况下才执行开阀动作
                        {
                            sys_params->ValveCtrlDemandFlg = 0xA0;
                            valve_triggered();
                        }
                    }
                    //else if(sys_params->ForceCtrl == 0xFF)
                    //{
                        ////强制取消0xFF 则阀门不动作
                    //}
                    
                }
                
                //系统设定温标志  0x00 用户设定类型     0xAA 系统设定类型
                if(pkg->data[20]==0x00 || pkg->data[20]==0xAA )
                {
                    sys_params->TempCtrlType =  pkg->data[20];
                }
                
                //如果是系统设置类型的时候 才会给设定温度设定，用户设定类型不操作
                if(sys_params->TempCtrlType == 0xAA)
                {
                    //系统设定温度
                    bytes_reverse(pkg->data+21, 2);
                    sys_params->TempSetValue = hex_2_dec_type(pkg->data+21, 2, ORD_MOTOR);
                }
                
                //极限温度设置标志   0XAA系统设定极限    0XFF系统取消设定
                if(pkg->data[23]==0xAA || pkg->data[23]==0xFF) 
                {
                    sys_params->LimtTE_Flag =  pkg->data[23];
                }
                
                //系统设定极限温度有效时才设定极限温度，0XFF不操作
                if(sys_params->LimtTE_Flag == 0xAA)
                {
                    //极限温度设置值
                    bytes_reverse(pkg->data+24, 2);
                    sys_params->LimtTE_Value = hex_2_dec_type(pkg->data+24, 2, ORD_MOTOR);
                }
                
                //供暖开始时间
                bytes_reverse(pkg->data+26, 2);
                sys_params->Start_Time.Year = hex_2_dec_type(pkg->data+26, 2, ORD_MOTOR);
                sys_params->Start_Time.Month = pkg->data[28];
                sys_params->Start_Time.Day = pkg->data[29];
                sys_params->Start_Time.Hour = pkg->data[30];
                sys_params->Start_Time.Minute = pkg->data[31];
                sys_params->Start_Time.Second = pkg->data[32];
                
                //供暖结束时间
                bytes_reverse(pkg->data+33, 2);
                sys_params->Start_Time.Year = hex_2_dec_type(pkg->data+33, 2, ORD_MOTOR);
                sys_params->Start_Time.Month = pkg->data[35];
                sys_params->Start_Time.Day = pkg->data[36];
                sys_params->Start_Time.Hour = pkg->data[37];
                sys_params->Start_Time.Minute = pkg->data[38];
                sys_params->Start_Time.Second = pkg->data[39];
                
                //楼栋信息
                sys_params->Buld =  pkg->data[40];
                sys_params->Unit =  pkg->data[41];
                sys_params->Floor =  pkg->data[42];
                sys_params->Cell =  pkg->data[43];
                bytes_reverse(pkg->data+44, 2);
                sys_params->CellArea = hex_2_dec_type(pkg->data+44, 2, ORD_MOTOR);
            }
            
            //valve_triggered();
            
            //保存数据
            save_params(SYS_SAVE_SHARE_PARAMS);
            //save_params(SYS_SAVE_RECORD_PARAMS);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//检测供暖期
            
            pkg->type = 132;
        }
            break;
        case 60:    // 通信测试命令                           133:返回
        {
            bytes_reverse(pkg->data, 2);
            t.tm_year   = hex_2_dec_type(pkg->data, 2, ORD_MOTOR) - 1900;
            t.tm_mon    = pkg->data[2]-1;
            t.tm_mday   = pkg->data[3];
            t.tm_hour   = pkg->data[4];
            t.tm_min    = pkg->data[5];
            t.tm_sec    = pkg->data[6];
            t.tm_wday   = pkg->data[7];
            
            rtc_write(t);
            
            pkg->type = 133;
        }
            break;
        case 61:    // （广播方式）供暖时间段命令
        {
            bytes_reverse(pkg->data, 2);
            sys_params->Start_Time.Year     = hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            sys_params->Start_Time.Month    = pkg->data[2];
            sys_params->Start_Time.Day      = pkg->data[3];
            sys_params->Start_Time.Hour     = pkg->data[4];
            sys_params->Start_Time.Minute   = pkg->data[5];
            sys_params->Start_Time.Second   = pkg->data[6];
            
            bytes_reverse(pkg->data+7, 2);
            sys_params->Over_Time.Year      = hex_2_dec_type(pkg->data+7, 2, ORD_MOTOR);
            sys_params->Over_Time.Month     = pkg->data[9];
            sys_params->Over_Time.Day       = pkg->data[10];
            sys_params->Over_Time.Hour      = pkg->data[11];
            sys_params->Over_Time.Minute    = pkg->data[12];
            sys_params->Over_Time.Second    = pkg->data[13];
            
            //再添加一个供暖期设置成功标志
            //run_params->TimeSetFlag = 0x0A;
            
            //保存数据
            save_params(SYS_SAVE_SHARE_PARAMS);
        }
            break;
        case 62:    // （广播方式）上一供暖季数据清除命令
        {
            if(pkg->data[0]==0xAA && pkg->data[1]==0xAA)
            {
                //g_sys_params.ControlType         = 0x00;                //用户费控类型 0X00未启用预付费  0XA0 已启用预付费没有欠费  0XAA用户已经欠费
                //g_sys_params.ArrearFlg           = 0x00;                //欠费标志 0x00没有欠费   0xAA欠费
                g_sys_params.Cycle_OpTim         = 0;                   //周期开阀时间 单位（秒）
                g_sys_params.loop_time_59        = 0;                   //周期供暖时间 单位（秒）
                g_sys_params.Apportion_Energy    = 0;                   //周期分摊热量 使用热量 单位（kwh）
                //剩余热量和购买热量
                g_sys_params.Remain_Heat         = 0;                   //剩余热量 单位（kwh）
                g_sys_params.Buy_Total_Heat      = 0;                   //用户购入热量 单位（kwh）
                
                //保存数据
                save_params(SYS_SAVE_RECORD_PARAMS);
            }
        }
            break;
        case 63:    // （支持广播方式 和 ID校验模式） 设置房间面积，房间号等
        {
            sys_params->Buld = pkg->data[0];
            sys_params->Unit = pkg->data[1];
            sys_params->Floor = pkg->data[2];
            sys_params->Cell = pkg->data[3];
            
            bytes_reverse(pkg->data+4, 2);
            sys_params->CellArea = hex_2_dec_type(pkg->data+4, 2, ORD_MOTOR);
            
            //保存数据
            save_params(SYS_SAVE_SHARE_PARAMS);
            pkg->type = 135;
        }
            break;
        case 80:    // （支持广播方式 和 ID校验模式） 终端数据读取命令  (调试专用命令)        132:返回
        {
            pkg->type = 132;
        }
            break;
        case 81:    // （支持广播方式 和 ID校验模式） 终端参数读取命令  (调试专用命令)        135:返回
        {
            pkg->type = 135;
        }
            break;
        case 82:    // （支持广播方式 和 ID校验模式） 终端恢复出厂设置命令  (调试专用命令)    135:返回
        {
            //终端恢复出厂设置
            sys_factory_reset();
            valve_triggered();
            pkg->type = 135;
        }
            break;
        case 83:    // （支持广播方式 和 ID校验模式） 终端计量数据命令 (调试专用命令)         84:返回
        {
            bytes_reverse(pkg->data, 2);
            sys_params->Start_Time.Year     = hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            sys_params->Start_Time.Month    = pkg->data[2];
            sys_params->Start_Time.Day      = pkg->data[3];
            sys_params->Start_Time.Hour     = pkg->data[4];
            sys_params->Start_Time.Minute   = pkg->data[5];
            sys_params->Start_Time.Second   = pkg->data[6];
            
            bytes_reverse(pkg->data+7, 2);
            sys_params->Over_Time.Year      = hex_2_dec_type(pkg->data+7, 2, ORD_MOTOR);
            sys_params->Over_Time.Month     = pkg->data[9];
            sys_params->Over_Time.Day       = pkg->data[10];
            sys_params->Over_Time.Hour      = pkg->data[11];
            sys_params->Over_Time.Minute    = pkg->data[12];
            sys_params->Over_Time.Second    = pkg->data[13];
            
            bytes_reverse(pkg->data+14, 4);
            sys_params->Cycle_OpTim = hex_2_dec_type(pkg->data+14, 4, ORD_MOTOR); 
            
            //保存数据
            save_params(SYS_SAVE_SHARE_PARAMS);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//检测供暖期
            
            pkg->type = 84;
        }
            break;
        case 85:    // （支持广播方式 和 ID校验模式） 通断控制器温度校准命令 (调试专用命令)   87:返回
        {
            s16 data_temp_L = 0, data_temp_H = 0;

            bytes_reverse(pkg->data+8, 2);
            sys_params->Offect_Temp = hex_2_dec_type(pkg->data+8, 2, ORD_MOTOR); 
            if(sys_params->Offect_Temp == 0xC350)
            {
                data_temp_H = pkg->data[0];
                data_temp_L = pkg->data[1];
                sys_params->RakeRatioOffect_L = (s16)((data_temp_L&0X00FF)<<8) | (data_temp_H&0x00FF>>8);
                
                data_temp_H = pkg->data[2];
                data_temp_L = pkg->data[3];
                sys_params->RakeRatioOffect_H = (s16)((data_temp_L&0X00FF)<<8) | (data_temp_H&0x00FF>>8);
                
                data_temp_H = pkg->data[4];
                data_temp_L = pkg->data[5];
                sys_params->Offect_Input = (s16)((data_temp_L&0X00FF)<<8) | (data_temp_H&0x00FF>>8);
                
                data_temp_H = pkg->data[6];
                data_temp_L = pkg->data[7];
                sys_params->Offect_Output = (s16)((data_temp_L&0X00FF)<<8) | (data_temp_H&0x00FF>>8);
            }

            //保存数据
            save_params(SYS_SAVE_RECORD_PARAMS);
            
            pkg->type = 87;
        }
            break;
        case 86:    // （支持广播方式 和 ID校验模式） 温度校准参数读取命令  (调试专用命令)    87:返回
        {
            pkg->type = 87;
        }
            break;
        case 200:   // 温控面板发送通信请求命令               201:返回
        {
            u8 temp[2] = {0xAA, 0xAA};
            
            if(pkg->sn == sys_params->Device_SN)
            {
                if(memcmp(pkg->data, temp, 2) == 0)
                {
                    //温度设定类型
                    //sys_params->TempCtrlType = sys_params->TempCtrlType;
                    
                    //温度设置值
                    if(sys_params->TempCtrlType == 0x00)   //AA：系统控制 00：用户控制 设定温度控制类型 
                    {
                        bytes_reverse(pkg->data+3, 2);
                        sys_params->TempSetValue = hex_2_dec_type(pkg->data+3, 2, ORD_MOTOR); 
                    }
                    //MYLOG_DEBUG("【foryon】-> 【温度--设置值】： TempSetValue = %d\r\n", g_sys_params.TempSetValue);
                    
                    //当前室内温度
                    bytes_reverse(pkg->data+5, 2);
                    run_params->Cur_Temp = hex_2_dec_type(pkg->data+5, 2, ORD_MOTOR); 
                    //MYLOG_DEBUG("【foryon】-> 【当前室内温度】： Run_Cur_Temp = %d\r\n", run_params->Cur_Temp);
                    
                    //室内平均温度
                    bytes_reverse(pkg->data+7, 2);
                    run_params->Ave_Temp = hex_2_dec_type(pkg->data+7, 2, ORD_MOTOR); 
                    //MYLOG_DEBUG("【foryon】-> 【室内平均温度】： Run_Ave_Temp = %d\r\n", run_params->Ave_Temp);
                    
                    //面板故障状态
                    run_params->Face_State = pkg->data[9]; 
                    //MYLOG_DEBUG("【foryon】-> 【温控器--故障】： Run_Face_State = %d\r\n", run_params->Face_State);
                    
                    //通信成功次数
                    bytes_reverse(pkg->data+10, 4);
                    run_params->Com_OK = hex_2_dec_type(pkg->data+10, 4, ORD_MOTOR); 
                    //MYLOG_DEBUG("【foryon】-> 【通信成功次数】： Run_Com_OK = %d\r\n", run_params->Com_OK);
                    
                    //通信总次数
                    bytes_reverse(pkg->data+14, 4);
                    run_params->Com_Total = hex_2_dec_type(pkg->data+14, 4, ORD_MOTOR); 
                    //MYLOG_DEBUG("【foryon】-> 【通信总--次数】： Run_Com_Total = %d\r\n", run_params->Com_Total);
                    
                    //强制控制温控器不能控制阀门
                    if(sys_params->ForceCtrl == 0xFF)   //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消
                    {
                        //阀门动作请求//阀门控制命令标志 0x0A开阀 0xA0关阀
                        if(pkg->data[18] == 0x0A)       //开阀
                        {
                            if(g_sys_params.ControlType != 0xAA)
                            {
                                if(run_params->ValveState != 0x0A)//动作和现在相反才执行阀门动作
                                {
                                    sys_params->ValveCtrlDemandFlg = 0x0A; 
                                    valve_triggered();
                                }
                            }
                        }
                        else if(pkg->data[18] == 0xA0)  //关阀
                        {
                            if(g_sys_params.ControlType != 0xAA)
                            {
                                if(run_params->ValveState != 0xA0)//动作和现在相反才执行阀门动作
                                {
                                    sys_params->ValveCtrlDemandFlg = 0xA0; 
                                    valve_triggered();
                                }
                            }
                        }
                        //valve_triggered();
                    }

                    //MYLOG_DEBUG("【foryon】-> 【温度设置类型】： TempCtrlType          = 0x%02X\r\n",      g_sys_params.TempCtrlType);
                    //MYLOG_DEBUG("【foryon】-> 【阀门----控制】： ValveCtrlDemandFlg    = 0x%02X\r\n\r\n",  g_sys_params.ValveCtrlDemandFlg);
                    
                    //保存数据
                    save_params(SYS_SAVE_RECORD_PARAMS);
                    
                    pkg->type = 201;
                }
                
            }
            
        }
            break;
        default:
            
            break;
    }
    
    return 0;
}

u8 fy_protocol_func_response(FY_PROTOCOLPkg *pkg,  unsigned char *packaged_data, u8 *packaged_data_size)
{
    u8 offset = 0;

//    packaged_data[offset++] = FY_PROTOCOL_STX;                                  //0x3A
//    packaged_data[offset++] = pkg->ver;                                         //0x70
//    
//    dec_2_hex_type(packaged_data+offset, sizeof(pkg->sn), pkg->sn, ORD_MOTOR);  //SN
//    bytes_reverse(packaged_data+offset, 4);
//    offset += sizeof(pkg->sn);
//    
//    memset(packaged_data, 0x00, sizeof(pkg->desn));                             //DESN
//    offset += sizeof(pkg->desn);
//    
//    dec_2_hex_type(packaged_data+offset, sizeof(pkg->gn), pkg->gn, ORD_MOTOR);  //GN
//    bytes_reverse(packaged_data+offset, 2);
//    offset += sizeof(pkg->gn);
    
//    packaged_data[offset++] = pkg->type;                                        //TYPE
    
    switch (pkg->type)
    {
        case 1:     // 出厂参数配置命令(调试专用命令)  1:返回
            //packaged_data[offset++] = 8;//pkg->lenth
            pkg->lenth = 9;
        
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Device_SN), g_sys_params.Device_SN, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += sizeof(g_sys_params.Device_SN);
        
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Device_GN), g_sys_params.Device_GN, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.Device_GN);
        
            packaged_data[offset++] = g_sys_params.Version_H;
            packaged_data[offset++] = g_sys_params.Version_S;
        
            //执行标志  0xAA 执行成功    0x00执行失败
            packaged_data[offset++] = 0xAA;
            
            *packaged_data_size = offset;
            break;
        case 84:    // 
            //packaged_data[offset++] = 18;//pkg->lenth
            pkg->lenth = 18;
        
            //供暖期开始时间
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Start_Time.Year), g_sys_params.Start_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Start_Time.Month;
            packaged_data[offset++] =g_sys_params.Start_Time.Day;
            packaged_data[offset++] =g_sys_params.Start_Time.Hour;
            packaged_data[offset++] =g_sys_params.Start_Time.Minute;
            packaged_data[offset++] =g_sys_params.Start_Time.Second;
        
            //供暖期结束时间
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Over_Time.Year), g_sys_params.Over_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Over_Time.Month;
            packaged_data[offset++] =g_sys_params.Over_Time.Day;
            packaged_data[offset++] =g_sys_params.Over_Time.Hour;
            packaged_data[offset++] =g_sys_params.Over_Time.Minute;
            packaged_data[offset++] =g_sys_params.Over_Time.Second;
        
            //开阀时间
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Cycle_OpTim), g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += sizeof(g_sys_params.Cycle_OpTim);
            
            *packaged_data_size = offset;
            break;
        case 87:    // 
            //packaged_data[offset++] = 12;   //pkg->lenth
            pkg->lenth = 12;
        
            packaged_data[offset++] = 0xAA; //CtrlFlg
            packaged_data[offset++] = 0xAA;
            
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.RakeRatioOffect_L), g_sys_params.RakeRatioOffect_L, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.RakeRatioOffect_L);
            
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.RakeRatioOffect_H), g_sys_params.RakeRatioOffect_H, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.RakeRatioOffect_H);
            
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Offect_Input), g_sys_params.Offect_Input, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.Offect_Input);
        
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Offect_Output), g_sys_params.Offect_Output, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.Offect_Output);
        
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Offect_Temp), 0X1388, ORD_MOTOR);      //5000==0x1388
            bytes_reverse(packaged_data+offset, 2);
            offset += sizeof(g_sys_params.Offect_Temp);
        
            *packaged_data_size = offset;
            break;
        case 128:    // 
            //packaged_data[offset++] = 1;    //pkg->lenth
            pkg->lenth = 1;
            
//            if(g_run_params.DevErrState == 0x00)//如果没有错误标志则返回正常
//            {
                 packaged_data[offset++] = 0xAA; //请求成功  CtrlFlg
//            }
//            else
//            {
//                 packaged_data[offset++] = 0x00; //请求失败 CtrlFlg
//            }
            *packaged_data_size = offset;
            break;
        case 129:    // 
        {
            struct tm t;
            
            rtc_read(&t);
            
            //packaged_data[offset++] = 10;//pkg->lenth
            pkg->lenth = 10;
            
            //系统实时时间
            dec_2_hex_type(packaged_data+offset, 2, t.tm_year+1900, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] = t.tm_mon+1;
            packaged_data[offset++] = t.tm_mday;
            packaged_data[offset++] = t.tm_hour;
            packaged_data[offset++] = t.tm_min;
            packaged_data[offset++] = t.tm_sec;
            packaged_data[offset++] = t.tm_wday;
            //CtrlFlg
            packaged_data[offset++] = 0xAA;
            packaged_data[offset++] = 0xAA;

            *packaged_data_size = offset;
        }
            break;
        case 130:    // 
            //packaged_data[offset++] = 6;    //pkg->lenth
            pkg->lenth = 6;
        
            packaged_data[offset++] = g_sys_params.Buld;
            packaged_data[offset++] = g_sys_params.Unit;
            packaged_data[offset++] = g_sys_params.Floor;
            packaged_data[offset++] = g_sys_params.Cell;
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.CellArea, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
        
            *packaged_data_size = offset;
            break;
        case 131:    // 
            //packaged_data[offset++] = 2;   //pkg->lenth
            pkg->lenth = 2;
        
            packaged_data[offset++] = 0xAA;
            packaged_data[offset++] = 0xAA;
        
            *packaged_data_size = offset;
            break;
        case 132:    // 
        {
            struct tm t;
            
            rtc_read(&t);
            
            //packaged_data[offset++] = 34;//pkg->lenth
            pkg->lenth = 34;
        
            packaged_data[offset++] = t.tm_mon+1;
            packaged_data[offset++] = t.tm_mday; //  tm_yday
            packaged_data[offset++] = t.tm_hour;
            packaged_data[offset++] = t.tm_min;
            packaged_data[offset++] = g_sys_params.Buld;
            packaged_data[offset++] = g_sys_params.Unit;
            packaged_data[offset++] = g_sys_params.Floor;
            packaged_data[offset++] = g_sys_params.Cell;
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.CellArea, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //设定温度的控制类型
            packaged_data[offset++] = g_sys_params.TempCtrlType;
            
            //面板的设定温度
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //面板当前温度
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Cur_Temp, ORD_MOTOR);  //面板平均温度
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //面板平均温度
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Ave_Temp, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //阀门进水温度
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Input_Temp*10, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //阀门回水温度
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Output_Temp*10, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //通断阀阀门位置
            packaged_data[offset++] = g_run_params.ValveState;
            
            //周期开阀时间
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //周期开阀比例
            dec_2_hex_type(packaged_data+offset, 2, 0, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //累计开阀时间
            dec_2_hex_type(packaged_data+offset, 4, 0, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //面板故障状态
            packaged_data[offset++] = g_run_params.Face_State; //0x01:电池电量低  0x80：通信故障    0x00：正常
            
            //通断控制阀故障状态
            packaged_data[offset++] = g_run_params.DevErrState;
            
            *packaged_data_size = offset;
        }
            break;
        case 133:    // 
            //packaged_data[offset++] = 2;//pkg->lenth
            pkg->lenth = 2;
        
            packaged_data[offset++] = 0xAA;
            packaged_data[offset++] = 0xAA;
        
            *packaged_data_size = offset;
            break;
        case 135:    // 
            //packaged_data[offset++] = 26;//pkg->lenth
            pkg->lenth = 49;
        
            packaged_data[offset++] = 0xAA;
            packaged_data[offset++] = 0xAA;
        
            //楼栋号
            packaged_data[offset++] = g_sys_params.Buld;
                
            //单元号
            packaged_data[offset++] = g_sys_params.Unit;
                
            //楼栋号
            packaged_data[offset++] = g_sys_params.Floor;
               
            //房间号
            packaged_data[offset++] = g_sys_params.Cell;
            
            //房间面积
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.CellArea, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //设备类型
            packaged_data[offset++] = g_sys_params.DeviceType;
            //温度设置类型
            packaged_data[offset++] = g_sys_params.TempCtrlType;
        
            //温度设置值
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
        
            //极限温度设置标志 0xAA 系统强制设定温度， 0xFF 系统取消强制设定温度
            packaged_data[offset++] = g_sys_params.LimtTE_Flag;
        
            //极限温度设置值
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.LimtTE_Value, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //面板故障状态
            packaged_data[offset++] = g_run_params.Face_State;
            
            //阀门故障状态
            packaged_data[offset++] = g_run_params.DevErrState;
            
            //用户编号
            packaged_data[offset++] = g_sys_params.Device_DESN;
            
            //阀门强制控制标志
            packaged_data[offset++] = g_sys_params.ForceCtrl;
            
            //用户费控类型 0X00未启用预付费  0XA0 已启用预付费没有欠费  0XAA用户已经欠费
            packaged_data[offset++] = g_sys_params.ControlType;
            
            //供暖期开始时间
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Start_Time.Year), g_sys_params.Start_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Start_Time.Month;
            packaged_data[offset++] =g_sys_params.Start_Time.Day;
            packaged_data[offset++] =g_sys_params.Start_Time.Hour;
            packaged_data[offset++] =g_sys_params.Start_Time.Minute;
            packaged_data[offset++] =g_sys_params.Start_Time.Second;
        
            //供暖期结束时间
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Over_Time.Year), g_sys_params.Over_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Over_Time.Month;
            packaged_data[offset++] =g_sys_params.Over_Time.Day;
            packaged_data[offset++] =g_sys_params.Over_Time.Hour;
            packaged_data[offset++] =g_sys_params.Over_Time.Minute;
            packaged_data[offset++] =g_sys_params.Over_Time.Second;
            
            //供暖季标志记录
            packaged_data[offset++] = g_run_params.WorkFlg;
            
            //阀门动作时间点
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.RecordTime.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.RecordTime.Month;
            packaged_data[offset++] =g_sys_params.RecordTime.Day;
            packaged_data[offset++] =g_sys_params.RecordTime.Hour;
            packaged_data[offset++] =g_sys_params.RecordTime.Minute;
            packaged_data[offset++] =g_sys_params.RecordTime.Second;
            
            //通断阀阀门位置
            packaged_data[offset++] = g_sys_params.ValveCtrlDemandFlg;
            
            //累计开阀时间
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            packaged_data[offset++] = g_sys_params.Version_H;
            packaged_data[offset++] = g_sys_params.Version_S;
            
            *packaged_data_size = offset;
            break;
        case 201:    // 
        {
            struct tm t;
            
            rtc_read(&t);
            
            //packaged_data[offset++] = 27;//pkg->lenth
            pkg->lenth = 27;
            
            //通断阀门 阀门位置
            if(g_sys_params.ValveCtrlDemandFlg==0x55 || g_sys_params.ValveCtrlDemandFlg==0x0A || g_sys_params.ValveCtrlDemandFlg== 0XA0 || g_sys_params.ValveCtrlDemandFlg==0X00)
                packaged_data[offset++] = g_sys_params.ValveCtrlDemandFlg;
            
            //阀门状态
            if(g_run_params.ValvEerror == 0xFF || g_run_params.ValvEerror==0x00)        //阀门故障状态  0xFF(堵转、两个微动开关同时闭合、驱动芯片坏)     0x00正常
                packaged_data[offset++] = g_run_params.ValvEerror;
            
            //阀门堵转状态
            if(g_run_params.Plug_valve_state==0x05 || g_run_params.Plug_valve_state==0x50 || g_run_params.Plug_valve_state==0x55 || g_run_params.Plug_valve_state==0x00 )
                packaged_data[offset++] =  g_run_params.Plug_valve_state;               //0x05开阀堵转  0x50关阀堵转   0x55全部堵转   0x00无堵转

            //累计开阀时间
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
        
            //分摊热量
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Apportion_Energy, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //系统实时时间
            dec_2_hex_type(packaged_data+offset, 2, t.tm_year+1900, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] = t.tm_mon+1;
            packaged_data[offset++] = t.tm_mday;
            packaged_data[offset++] = t.tm_hour;
            packaged_data[offset++] = t.tm_min;
            packaged_data[offset++] = t.tm_sec;
            packaged_data[offset++] = t.tm_wday;
            
            //供暖季标志
            if(g_run_params.WorkFlg==0xAA || g_run_params.WorkFlg==0x00)            //供暖季标志记录  0XAA 在供暖季   0X00 不在供暖季
                packaged_data[offset++] = g_run_params.WorkFlg;
            
            //温度设置类型 0X00 用户设置  0XAA 系统设置   系统设置条件下用户将不能更改温度设置值
            if(g_sys_params.TempCtrlType==0x00 || g_sys_params.TempCtrlType==0xAA)
                packaged_data[offset++] = g_sys_params.TempCtrlType;
            
            //设定温度
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //欠费状态
            packaged_data[offset++] = g_sys_params.ControlType;
            
            //极限温度设定温度
            packaged_data[offset++] = g_sys_params.LimtTE_Flag;
            
            //极限温度设置值
            dec_2_hex_type(packaged_data+offset, 2,g_sys_params.LimtTE_Value, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
        
            *packaged_data_size = offset;
        }
            break;
        default:
            return 1;//没有此反馈命令 ，无返回
            //break;
    }
    
    return 0;
}

