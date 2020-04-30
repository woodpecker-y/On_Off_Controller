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
*      FY_PROTOCOLЭ�����
* \param pkg[IN]                - FY_PROTOCOLЭ��֡����ṹ��
* \param packaged_data[IN]      - ���������ݰ�
* \param packaged_data_size[IN] - ���������ݰ�����
* 
* \return  
*      FY_PROTOCOL_OK                   - �ɹ�
*      FY_PROTOCOL_ERR_INVALID_STX_ETX  - ֡ͷ��֡β����
*      FY_PROTOCOL_ERR_CHECKSUM         - У��ʹ���
*      FY_PROTOCOL_ERR_LENGTH           - ���ȴ���
* \note  
* 
*/
unsigned char fy_protocol_pack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, u8 *packaged_data_size)
{
    unsigned char offset = 0;
    
    //assert_param(*packaged_data_size > pkg->msg_size+10);
    
    // ֡ͷ
    packaged_data[offset] = FY_PROTOCOL_STX;
    offset += 1;
    
    // �Ǳ�����
    packaged_data[offset] = pkg->ver;
    offset += 1;
    
    // SN �Ǳ��ַ
    //memcpy(packaged_data+offset, &(pkg->sn), sizeof(unsigned int));
    memcpy(packaged_data+offset, &g_sys_params.Device_SN, sizeof(unsigned int));
    //bytes_reverse(packaged_data+offset, sizeof(unsigned int));
    offset += sizeof(unsigned int);
    
    // DESN �����Ա���
    //packaged_data[offset] = pkg->desn;
    packaged_data[offset] = g_sys_params.Device_DESN;
    offset += 2;
    
    // GN �������
    //memcpy(packaged_data+offset, &(pkg->desn), 2);
    memcpy(packaged_data+offset, &g_sys_params.Device_GN, 2);
    //bytes_reverse(packaged_data+offset, 2);
    offset += 2;
    
    // TYPE �����Ա���
    packaged_data[offset] = pkg->type;
    offset += 1;
    
    // LENTH �����Ա���
    packaged_data[offset] = pkg->lenth;
    offset += 1;
    
    // DATA ����
    memcpy(packaged_data+offset, pkg->data, pkg->lenth);
    offset += pkg->lenth;
    
    // У���
    packaged_data[offset] = check_sum(packaged_data, offset);
    offset += 1;
    
    // ֡β
    packaged_data[offset] = FY_PROTOCOL_ETX;
    offset += 1;
    
    *packaged_data_size = offset;
    
    return 0;
}

/*! \brief 
*      FY_PROTOCOLЭ����
* \param pkg[IN]                    - �����洢���ݽṹ
* \param packaged_data[IN]          - ԭʼ����
* \param packaged_data_size[IN]     - ԭʼ���ĳ���
* 
* \return  
*      FY_PROTOCOL_OK                   - �ɹ�
*      FY_PROTOCOL_ERR_INVALID_STX_ETX  - ֡ͷ��֡β����
*      FY_PROTOCOL_ERR_CHECKSUM         - У��ʹ���
*      FY_PROTOCOL_ERR_LENGTH           - ���ȴ���
* \note  
* 
*/
unsigned char fy_protocol_unpack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, int packaged_data_size)
{
    unsigned int  device_sn = 0;
    unsigned char offset = 0;
    unsigned char sum    = 0;
    
    //data_trim_int(packaged_data, &packaged_data_size);
    
    // ֡ͷ��֡β���
    if (packaged_data[0]!=FY_PROTOCOL_STX || packaged_data[packaged_data_size-1]!=FY_PROTOCOL_ETX)
    {
        return FY_PROTOCOL_ERR_INVALID_PKG;
    }
    
    // �ж�Э������
    if(packaged_data[1] != 0x70)
    {
        return FY_PROTOCOL_ERR_UNKNOWN_DATA_FLAG;       //δ֪�����ݱ�ʶ
    }
    
    // �ж�SN�Ƿ��Ǵ��豸�豸���
    bytes_reverse(packaged_data+2, 4);
    device_sn = hex_2_dec_type(packaged_data+2, 4, ORD_MOTOR);
    if(device_sn == g_sys_params.Device_SN || device_sn == 0xAAAAAAAA)
    {
        if(device_sn == 0xAAAAAAAA)
        {
            //����֧�ֹ㲥��Э������
            if( packaged_data[10]!= 0 && packaged_data[10]!=50 && packaged_data[10]!=51 && packaged_data[10]!=52 && \
                packaged_data[10]!=53 && packaged_data[10]!=54 && packaged_data[10]!=55 && packaged_data[10]!=61 && \
                packaged_data[10]!=62 && packaged_data[10]!=63 && packaged_data[10]!=80 && packaged_data[10]!=81 && \
                packaged_data[10]!=82 && packaged_data[10]!=83 && packaged_data[10]!=85 && packaged_data[10]!=86 )
                return FY_PROTOCOL_ERR_ADDR;//��ַ��ƥ��
        }
        else if(device_sn == g_sys_params.Device_SN)
        {
            //���в�֧�ֹ㲥��Э������
            if( packaged_data[10]!=0 && packaged_data[10]!=59 && packaged_data[10]!=60 && packaged_data[10]!=200)
                return FY_PROTOCOL_ERR_ADDR;//��ַ��ƥ��
        }
    }
    else
        return FY_PROTOCOL_ERR_ADDR;//��ַ��ƥ��
    
    // У��ͼ��
    sum = check_sum(packaged_data, packaged_data_size-2);
    if (sum != packaged_data[packaged_data_size-2])
    {
        return FY_PROTOCOL_ERR_CHECKSUM;                //У��ʹ���
    }

    // �����ж�
    if (packaged_data[11] != packaged_data_size-14)
    {
        return FY_PROTOCOL_ERR_LENGTH;                  //���ȴ���
    }
    
    offset = 0;
    // ����֡ͷ
    offset += 1;
    
    // VER Э������
    pkg->ver = packaged_data[offset];
    offset += 1;
    
    // SN ��ַ��
    //bytes_reverse(packaged_data+offset, sizeof(unsigned int));
    //pkg->sn = hex_2_dec_type(packaged_data+offset, 4, ORD_MOTOR);
    //offset += sizeof(unsigned int);
    
    // SN ��ַ��
    pkg->sn = device_sn;
    offset += 4;
    
    // DESN �����Ա���
    pkg->desn = packaged_data[offset];
    offset += 2;
    
    // GN �������
    //bytes_reverse(packaged_data+offset, 2);
    //pkg->gn = hex_2_dec_type(packaged_data, 2, ORD_MOTOR);
    memcpy(&pkg->gn, packaged_data+offset, 2);
    offset += 2;

    // TYPE Э�����
    pkg->type = packaged_data[offset];
    offset += 1;

    // LENTH Э�����
    pkg->lenth = packaged_data[offset];
    offset += 1;

    // DATA ���ݸ���
    memcpy(pkg->data, packaged_data+offset, pkg->lenth);
    offset += pkg->lenth;
    
    return 0;
}

// ���ݹ��ˡ���ȡ ȥ��ǰ����FFFFFF ����Ϣ
int data_trim_int(unsigned char *res, int *res_len)
{
    unsigned int i = 0;
    unsigned int j = 0;
    
    if (*res_len == 0x00)
        return -1;
    
    //while (res[i] != 0x68 && i < *res_len)    //��3A��ͷ������Э���
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
    while (res[j] != 0xAA && j > i)         //��AA��β������Э���
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



//-------------------Э�鴦��----------------------

// ����Э���� TYPE �жϹ�������
u8 fy_protocol_func_request(FY_PROTOCOLPkg *pkg, RunParams *run_params, SysParams *sys_params)
{
    //u8  sn_temp[4] = {0};
    //u8  gn_temp[2] = {0};

    struct tm t;
    
    g_sys_params.Device_DESN = pkg->desn;//�豸��ϵͳ�еı��
    
    switch (pkg->type)
    {
        case 0:     // ����������������(����ר������)  1:����
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
            
            //�����豸��������
            save_params(SYS_SAVE_DEVICE_PARAMS);
            
            rf433_init();
            
            pkg->type = 1;
        }
            break;
        case 50:    // ���㲥��ʽ�� ������ �����㲥����ʱ��У׼����
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
            
            g_run_params.heating_timer       = UNIT_DAY-2;//��⹩ů��
        }
            break;
        case 51:    // ���㲥��ʽ�� ������ �����㲥����ȫ����������
        {
            //u8   hundred     = 0;
            //u8   decade      = 0;
            //u8   unit        = 0;
            
            bytes_reverse(pkg->data, 2);
//            unit    = (sys_params->Device_SN  & 0x0000000F);
//            decade  = ((sys_params->Device_SN & 0x000000F0)>>4)*10;
//            hundred = ((sys_params->Device_SN & 0x00000F00)>>8)*100;
//            run_params->All_open_delay = (hundred + decade + unit) * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);// ���Ź㲥�����ȴ� ���䶯����ʱ��λ����
            
            // ���Ź㲥�����ȴ� ���䶯����ʱ��λ����
            run_params->All_open_delay = sys_params->Device_DESN * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            s_valve_handler.all_open_timer = run_params->All_open_delay/50;
            
            //�㲥����ָ��
            g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
            valve_triggered();
            
            //����ϵͳ��¼����
            save_params(SYS_SAVE_RECORD_PARAMS);
        }
            break;
        case 52:    // ���㲥��ʽ�� ������ �����㲥����ȫ���ط�����
        {
            //u8   hundred     = 0;
            //u8   decade      = 0;
            //u8   unit        = 0;
            
            bytes_reverse(pkg->data, 2);
//            unit    = (sys_params->Device_SN  & 0x0000000F);
//            decade  = ((sys_params->Device_SN & 0x000000F0)>>4)*10;
//            hundred = ((sys_params->Device_SN & 0x00000F00)>>8)*100;
//            run_params->All_close_delay = (hundred + decade + unit) * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);// ���Ź㲥�����ȴ� ���䶯����ʱ��λ����
            
            // ���Ź㲥�����ȴ� ���䶯����ʱ��λ����
            run_params->All_close_delay = sys_params->Device_DESN * hex_2_dec_type(pkg->data, 2, ORD_MOTOR);
            s_valve_handler.all_close_timer = run_params->All_close_delay/50;
            //�㲥�ط�ָ��
            g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
            valve_triggered();
            
            //����ϵͳ��¼����
            save_params(SYS_SAVE_RECORD_PARAMS);
        }
            break;
        case 53:    // ���㲥��ʽ�� ������ �������Ϳ������ط���ǿ�������ȶ�������㲥��ʽ��
        {
            //���ϵͳû�д�����ִ�У������ڲ�ִ��
            //if(g_run_params.DevErrState == 0x00)
            {
                //���ſ��������־ 0x0A���� 0xA0�ط�
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                }
                
                if( pkg->data[1]==0x55 || pkg->data[1]==0xAA || pkg->data[1]==0xFF)
                {
                    sys_params->ForceCtrl = pkg->data[1];//����ǿ�ƿ��Ʊ�־  0xAAǿ�ƿ���   0x55ǿ�ƹط�
                }
                
                valve_triggered();
                
                //����ϵͳ��¼����
                save_params(SYS_SAVE_RECORD_PARAMS);
            }
        }
            break;
        case 54:    //  ��֧�ֹ㲥��ʽ �� IDУ��ģʽ���������ط���ǿ�������ȶ�������          128:����
        {
            //ǿ�ƿ��� ��Ҫ����� �¿����ģ�Э���Ƕ����Կ��Ʒ��ſ��صģ�����ǿ�ƿ���صĻ��¿����Ͳ��ܿ��Ʒ����ˡ�
            if(pkg->data[1]==0xAA)//ǿ��
            {
                //���ſ��������־ 0x0A���� 0xA0�ط�
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                    if(sys_params->ValveCtrlDemandFlg ==0x0A)
                        sys_params->ForceCtrl = 0x55;       //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��
                    else if(sys_params->ValveCtrlDemandFlg ==0xA0)
                        sys_params->ForceCtrl = 0xAA;       //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��
                }
            }
            else
            {
                //���ſ��������־ 0x0A���� 0xA0�ط�
                if( pkg->data[0]==0x0A || pkg->data[0]==0xA0)
                {
                    sys_params->ValveCtrlDemandFlg = pkg->data[0];
                }
                
                sys_params->ForceCtrl = 0xFF;
            }
            
            //��������
            save_params(SYS_SAVE_RECORD_PARAMS);
            
            valve_triggered();
            
            pkg->type = 128;
        }
            
            break;
        case 55:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �ն�ʱ��У׼����                       129:����
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
            
            g_run_params.heating_timer       = UNIT_DAY-2;//��⹩ů��
            
            pkg->type = 129;
        }
            break;
        case 59:    // ���ݲɼ�����                           132:����
        {
            //u8 data_temp[] = {0xAA, 0xAA};
            
            //if(memcmp(pkg->data, data_temp, 2) == 0)
            {
                //���ڹ�ůʱ�䣨�ɼ����������㣩
                bytes_reverse(pkg->data+2, 4);
                sys_params->loop_time_59 = hex_2_dec_type(pkg->data+2, 4, ORD_MOTOR);
                
                //�û���ʹ���������߷�̯����(KWH) ��2λС��
                bytes_reverse(pkg->data+6, 4);
                sys_params->Apportion_Energy = hex_2_dec_type(pkg->data+6, 4, ORD_MOTOR);
                
                //ʣ������(KWH)��2λС��
                bytes_reverse(pkg->data+10, 4);
                sys_params->Remain_Heat = hex_2_dec_type(pkg->data+10, 4, ORD_MOTOR);
                
                //������ �û�����������2λС��
                bytes_reverse(pkg->data+14, 4);
                sys_params->Buy_Total_Heat = hex_2_dec_type(pkg->data+14, 4, ORD_MOTOR);
                
                //�ѿ�����
                if(pkg->data[18]==0 || pkg->data[18]==0xA0 || pkg->data[18]==0xAA)
                {
                    sys_params->ControlType = pkg->data[18];
                }
                
                //ǿ�Ʒ��ű�־   //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��
                if(pkg->data[19]==0x55 || pkg->data[19]==0xAA || pkg->data[19]==0xFF)
                {
                    //ǿ�ƿ���ģʽ
                    sys_params->ForceCtrl = pkg->data[19];
                    //���Ŷ��� //���ſ��������־ 0x0A���� 0xA0�ط�                  
                    if(sys_params->ForceCtrl == 0x55)
                    {
                        if(run_params->ValveState != 0x0A)//������ڷ��Ų��Ǵ򿪵�����²�ִ�п�������
                        {
                            sys_params->ValveCtrlDemandFlg = 0x0A;
                            valve_triggered();
                        }
                    }
                    else if(sys_params->ForceCtrl == 0xAA)
                    {
                        if(run_params->ValveState != 0xA0)//������ڷ��Ų��ǹرյ�����²�ִ�п�������
                        {
                            sys_params->ValveCtrlDemandFlg = 0xA0;
                            valve_triggered();
                        }
                    }
                    //else if(sys_params->ForceCtrl == 0xFF)
                    //{
                        ////ǿ��ȡ��0xFF ���Ų�����
                    //}
                    
                }
                
                //ϵͳ�趨�±�־  0x00 �û��趨����     0xAA ϵͳ�趨����
                if(pkg->data[20]==0x00 || pkg->data[20]==0xAA )
                {
                    sys_params->TempCtrlType =  pkg->data[20];
                }
                
                //�����ϵͳ�������͵�ʱ�� �Ż���趨�¶��趨���û��趨���Ͳ�����
                if(sys_params->TempCtrlType == 0xAA)
                {
                    //ϵͳ�趨�¶�
                    bytes_reverse(pkg->data+21, 2);
                    sys_params->TempSetValue = hex_2_dec_type(pkg->data+21, 2, ORD_MOTOR);
                }
                
                //�����¶����ñ�־   0XAAϵͳ�趨����    0XFFϵͳȡ���趨
                if(pkg->data[23]==0xAA || pkg->data[23]==0xFF) 
                {
                    sys_params->LimtTE_Flag =  pkg->data[23];
                }
                
                //ϵͳ�趨�����¶���Чʱ���趨�����¶ȣ�0XFF������
                if(sys_params->LimtTE_Flag == 0xAA)
                {
                    //�����¶�����ֵ
                    bytes_reverse(pkg->data+24, 2);
                    sys_params->LimtTE_Value = hex_2_dec_type(pkg->data+24, 2, ORD_MOTOR);
                }
                
                //��ů��ʼʱ��
                bytes_reverse(pkg->data+26, 2);
                sys_params->Start_Time.Year = hex_2_dec_type(pkg->data+26, 2, ORD_MOTOR);
                sys_params->Start_Time.Month = pkg->data[28];
                sys_params->Start_Time.Day = pkg->data[29];
                sys_params->Start_Time.Hour = pkg->data[30];
                sys_params->Start_Time.Minute = pkg->data[31];
                sys_params->Start_Time.Second = pkg->data[32];
                
                //��ů����ʱ��
                bytes_reverse(pkg->data+33, 2);
                sys_params->Start_Time.Year = hex_2_dec_type(pkg->data+33, 2, ORD_MOTOR);
                sys_params->Start_Time.Month = pkg->data[35];
                sys_params->Start_Time.Day = pkg->data[36];
                sys_params->Start_Time.Hour = pkg->data[37];
                sys_params->Start_Time.Minute = pkg->data[38];
                sys_params->Start_Time.Second = pkg->data[39];
                
                //¥����Ϣ
                sys_params->Buld =  pkg->data[40];
                sys_params->Unit =  pkg->data[41];
                sys_params->Floor =  pkg->data[42];
                sys_params->Cell =  pkg->data[43];
                bytes_reverse(pkg->data+44, 2);
                sys_params->CellArea = hex_2_dec_type(pkg->data+44, 2, ORD_MOTOR);
            }
            
            //valve_triggered();
            
            //��������
            save_params(SYS_SAVE_SHARE_PARAMS);
            //save_params(SYS_SAVE_RECORD_PARAMS);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//��⹩ů��
            
            pkg->type = 132;
        }
            break;
        case 60:    // ͨ�Ų�������                           133:����
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
        case 61:    // ���㲥��ʽ����ůʱ�������
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
            
            //�����һ����ů�����óɹ���־
            //run_params->TimeSetFlag = 0x0A;
            
            //��������
            save_params(SYS_SAVE_SHARE_PARAMS);
        }
            break;
        case 62:    // ���㲥��ʽ����һ��ů�������������
        {
            if(pkg->data[0]==0xAA && pkg->data[1]==0xAA)
            {
                //g_sys_params.ControlType         = 0x00;                //�û��ѿ����� 0X00δ����Ԥ����  0XA0 ������Ԥ����û��Ƿ��  0XAA�û��Ѿ�Ƿ��
                //g_sys_params.ArrearFlg           = 0x00;                //Ƿ�ѱ�־ 0x00û��Ƿ��   0xAAǷ��
                g_sys_params.Cycle_OpTim         = 0;                   //���ڿ���ʱ�� ��λ���룩
                g_sys_params.loop_time_59        = 0;                   //���ڹ�ůʱ�� ��λ���룩
                g_sys_params.Apportion_Energy    = 0;                   //���ڷ�̯���� ʹ������ ��λ��kwh��
                //ʣ�������͹�������
                g_sys_params.Remain_Heat         = 0;                   //ʣ������ ��λ��kwh��
                g_sys_params.Buy_Total_Heat      = 0;                   //�û��������� ��λ��kwh��
                
                //��������
                save_params(SYS_SAVE_RECORD_PARAMS);
            }
        }
            break;
        case 63:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� ���÷������������ŵ�
        {
            sys_params->Buld = pkg->data[0];
            sys_params->Unit = pkg->data[1];
            sys_params->Floor = pkg->data[2];
            sys_params->Cell = pkg->data[3];
            
            bytes_reverse(pkg->data+4, 2);
            sys_params->CellArea = hex_2_dec_type(pkg->data+4, 2, ORD_MOTOR);
            
            //��������
            save_params(SYS_SAVE_SHARE_PARAMS);
            pkg->type = 135;
        }
            break;
        case 80:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �ն����ݶ�ȡ����  (����ר������)        132:����
        {
            pkg->type = 132;
        }
            break;
        case 81:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �ն˲�����ȡ����  (����ר������)        135:����
        {
            pkg->type = 135;
        }
            break;
        case 82:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �ն˻ָ�������������  (����ר������)    135:����
        {
            //�ն˻ָ���������
            sys_factory_reset();
            valve_triggered();
            pkg->type = 135;
        }
            break;
        case 83:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �ն˼����������� (����ר������)         84:����
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
            
            //��������
            save_params(SYS_SAVE_SHARE_PARAMS);
            
            g_run_params.heating_timer       = UNIT_DAY-2;//��⹩ů��
            
            pkg->type = 84;
        }
            break;
        case 85:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� ͨ�Ͽ������¶�У׼���� (����ר������)   87:����
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

            //��������
            save_params(SYS_SAVE_RECORD_PARAMS);
            
            pkg->type = 87;
        }
            break;
        case 86:    // ��֧�ֹ㲥��ʽ �� IDУ��ģʽ�� �¶�У׼������ȡ����  (����ר������)    87:����
        {
            pkg->type = 87;
        }
            break;
        case 200:   // �¿���巢��ͨ����������               201:����
        {
            u8 temp[2] = {0xAA, 0xAA};
            
            if(pkg->sn == sys_params->Device_SN)
            {
                if(memcmp(pkg->data, temp, 2) == 0)
                {
                    //�¶��趨����
                    //sys_params->TempCtrlType = sys_params->TempCtrlType;
                    
                    //�¶�����ֵ
                    if(sys_params->TempCtrlType == 0x00)   //AA��ϵͳ���� 00���û����� �趨�¶ȿ������� 
                    {
                        bytes_reverse(pkg->data+3, 2);
                        sys_params->TempSetValue = hex_2_dec_type(pkg->data+3, 2, ORD_MOTOR); 
                    }
                    //MYLOG_DEBUG("��foryon��-> ���¶�--����ֵ���� TempSetValue = %d\r\n", g_sys_params.TempSetValue);
                    
                    //��ǰ�����¶�
                    bytes_reverse(pkg->data+5, 2);
                    run_params->Cur_Temp = hex_2_dec_type(pkg->data+5, 2, ORD_MOTOR); 
                    //MYLOG_DEBUG("��foryon��-> ����ǰ�����¶ȡ��� Run_Cur_Temp = %d\r\n", run_params->Cur_Temp);
                    
                    //����ƽ���¶�
                    bytes_reverse(pkg->data+7, 2);
                    run_params->Ave_Temp = hex_2_dec_type(pkg->data+7, 2, ORD_MOTOR); 
                    //MYLOG_DEBUG("��foryon��-> ������ƽ���¶ȡ��� Run_Ave_Temp = %d\r\n", run_params->Ave_Temp);
                    
                    //������״̬
                    run_params->Face_State = pkg->data[9]; 
                    //MYLOG_DEBUG("��foryon��-> ���¿���--���ϡ��� Run_Face_State = %d\r\n", run_params->Face_State);
                    
                    //ͨ�ųɹ�����
                    bytes_reverse(pkg->data+10, 4);
                    run_params->Com_OK = hex_2_dec_type(pkg->data+10, 4, ORD_MOTOR); 
                    //MYLOG_DEBUG("��foryon��-> ��ͨ�ųɹ��������� Run_Com_OK = %d\r\n", run_params->Com_OK);
                    
                    //ͨ���ܴ���
                    bytes_reverse(pkg->data+14, 4);
                    run_params->Com_Total = hex_2_dec_type(pkg->data+14, 4, ORD_MOTOR); 
                    //MYLOG_DEBUG("��foryon��-> ��ͨ����--�������� Run_Com_Total = %d\r\n", run_params->Com_Total);
                    
                    //ǿ�ƿ����¿������ܿ��Ʒ���
                    if(sys_params->ForceCtrl == 0xFF)   //����ǿ�ƿ��Ʊ�־  0x55 ǿ�ƿ���  0xAAǿ�ƹط�   0xFFǿ��ȡ��
                    {
                        //���Ŷ�������//���ſ��������־ 0x0A���� 0xA0�ط�
                        if(pkg->data[18] == 0x0A)       //����
                        {
                            if(g_sys_params.ControlType != 0xAA)
                            {
                                if(run_params->ValveState != 0x0A)//�����������෴��ִ�з��Ŷ���
                                {
                                    sys_params->ValveCtrlDemandFlg = 0x0A; 
                                    valve_triggered();
                                }
                            }
                        }
                        else if(pkg->data[18] == 0xA0)  //�ط�
                        {
                            if(g_sys_params.ControlType != 0xAA)
                            {
                                if(run_params->ValveState != 0xA0)//�����������෴��ִ�з��Ŷ���
                                {
                                    sys_params->ValveCtrlDemandFlg = 0xA0; 
                                    valve_triggered();
                                }
                            }
                        }
                        //valve_triggered();
                    }

                    //MYLOG_DEBUG("��foryon��-> ���¶��������͡��� TempCtrlType          = 0x%02X\r\n",      g_sys_params.TempCtrlType);
                    //MYLOG_DEBUG("��foryon��-> ������----���ơ��� ValveCtrlDemandFlg    = 0x%02X\r\n\r\n",  g_sys_params.ValveCtrlDemandFlg);
                    
                    //��������
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
        case 1:     // ����������������(����ר������)  1:����
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
        
            //ִ�б�־  0xAA ִ�гɹ�    0x00ִ��ʧ��
            packaged_data[offset++] = 0xAA;
            
            *packaged_data_size = offset;
            break;
        case 84:    // 
            //packaged_data[offset++] = 18;//pkg->lenth
            pkg->lenth = 18;
        
            //��ů�ڿ�ʼʱ��
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Start_Time.Year), g_sys_params.Start_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Start_Time.Month;
            packaged_data[offset++] =g_sys_params.Start_Time.Day;
            packaged_data[offset++] =g_sys_params.Start_Time.Hour;
            packaged_data[offset++] =g_sys_params.Start_Time.Minute;
            packaged_data[offset++] =g_sys_params.Start_Time.Second;
        
            //��ů�ڽ���ʱ��
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Over_Time.Year), g_sys_params.Over_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Over_Time.Month;
            packaged_data[offset++] =g_sys_params.Over_Time.Day;
            packaged_data[offset++] =g_sys_params.Over_Time.Hour;
            packaged_data[offset++] =g_sys_params.Over_Time.Minute;
            packaged_data[offset++] =g_sys_params.Over_Time.Second;
        
            //����ʱ��
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
            
//            if(g_run_params.DevErrState == 0x00)//���û�д����־�򷵻�����
//            {
                 packaged_data[offset++] = 0xAA; //����ɹ�  CtrlFlg
//            }
//            else
//            {
//                 packaged_data[offset++] = 0x00; //����ʧ�� CtrlFlg
//            }
            *packaged_data_size = offset;
            break;
        case 129:    // 
        {
            struct tm t;
            
            rtc_read(&t);
            
            //packaged_data[offset++] = 10;//pkg->lenth
            pkg->lenth = 10;
            
            //ϵͳʵʱʱ��
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
            
            //�趨�¶ȵĿ�������
            packaged_data[offset++] = g_sys_params.TempCtrlType;
            
            //�����趨�¶�
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //��嵱ǰ�¶�
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Cur_Temp, ORD_MOTOR);  //���ƽ���¶�
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //���ƽ���¶�
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Ave_Temp, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //���Ž�ˮ�¶�
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Input_Temp*10, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //���Ż�ˮ�¶�
            dec_2_hex_type(packaged_data+offset, 2, g_run_params.Output_Temp*10, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //ͨ�Ϸ�����λ��
            packaged_data[offset++] = g_run_params.ValveState;
            
            //���ڿ���ʱ��
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //���ڿ�������
            dec_2_hex_type(packaged_data+offset, 2, 0, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //�ۼƿ���ʱ��
            dec_2_hex_type(packaged_data+offset, 4, 0, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //������״̬
            packaged_data[offset++] = g_run_params.Face_State; //0x01:��ص�����  0x80��ͨ�Ź���    0x00������
            
            //ͨ�Ͽ��Ʒ�����״̬
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
        
            //¥����
            packaged_data[offset++] = g_sys_params.Buld;
                
            //��Ԫ��
            packaged_data[offset++] = g_sys_params.Unit;
                
            //¥����
            packaged_data[offset++] = g_sys_params.Floor;
               
            //�����
            packaged_data[offset++] = g_sys_params.Cell;
            
            //�������
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.CellArea, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //�豸����
            packaged_data[offset++] = g_sys_params.DeviceType;
            //�¶���������
            packaged_data[offset++] = g_sys_params.TempCtrlType;
        
            //�¶�����ֵ
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
        
            //�����¶����ñ�־ 0xAA ϵͳǿ���趨�¶ȣ� 0xFF ϵͳȡ��ǿ���趨�¶�
            packaged_data[offset++] = g_sys_params.LimtTE_Flag;
        
            //�����¶�����ֵ
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.LimtTE_Value, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //������״̬
            packaged_data[offset++] = g_run_params.Face_State;
            
            //���Ź���״̬
            packaged_data[offset++] = g_run_params.DevErrState;
            
            //�û����
            packaged_data[offset++] = g_sys_params.Device_DESN;
            
            //����ǿ�ƿ��Ʊ�־
            packaged_data[offset++] = g_sys_params.ForceCtrl;
            
            //�û��ѿ����� 0X00δ����Ԥ����  0XA0 ������Ԥ����û��Ƿ��  0XAA�û��Ѿ�Ƿ��
            packaged_data[offset++] = g_sys_params.ControlType;
            
            //��ů�ڿ�ʼʱ��
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Start_Time.Year), g_sys_params.Start_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Start_Time.Month;
            packaged_data[offset++] =g_sys_params.Start_Time.Day;
            packaged_data[offset++] =g_sys_params.Start_Time.Hour;
            packaged_data[offset++] =g_sys_params.Start_Time.Minute;
            packaged_data[offset++] =g_sys_params.Start_Time.Second;
        
            //��ů�ڽ���ʱ��
            dec_2_hex_type(packaged_data+offset, sizeof(g_sys_params.Over_Time.Year), g_sys_params.Over_Time.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.Over_Time.Month;
            packaged_data[offset++] =g_sys_params.Over_Time.Day;
            packaged_data[offset++] =g_sys_params.Over_Time.Hour;
            packaged_data[offset++] =g_sys_params.Over_Time.Minute;
            packaged_data[offset++] =g_sys_params.Over_Time.Second;
            
            //��ů����־��¼
            packaged_data[offset++] = g_run_params.WorkFlg;
            
            //���Ŷ���ʱ���
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.RecordTime.Year, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] =g_sys_params.RecordTime.Month;
            packaged_data[offset++] =g_sys_params.RecordTime.Day;
            packaged_data[offset++] =g_sys_params.RecordTime.Hour;
            packaged_data[offset++] =g_sys_params.RecordTime.Minute;
            packaged_data[offset++] =g_sys_params.RecordTime.Second;
            
            //ͨ�Ϸ�����λ��
            packaged_data[offset++] = g_sys_params.ValveCtrlDemandFlg;
            
            //�ۼƿ���ʱ��
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
            
            //ͨ�Ϸ��� ����λ��
            if(g_sys_params.ValveCtrlDemandFlg==0x55 || g_sys_params.ValveCtrlDemandFlg==0x0A || g_sys_params.ValveCtrlDemandFlg== 0XA0 || g_sys_params.ValveCtrlDemandFlg==0X00)
                packaged_data[offset++] = g_sys_params.ValveCtrlDemandFlg;
            
            //����״̬
            if(g_run_params.ValvEerror == 0xFF || g_run_params.ValvEerror==0x00)        //���Ź���״̬  0xFF(��ת������΢������ͬʱ�պϡ�����оƬ��)     0x00����
                packaged_data[offset++] = g_run_params.ValvEerror;
            
            //���Ŷ�ת״̬
            if(g_run_params.Plug_valve_state==0x05 || g_run_params.Plug_valve_state==0x50 || g_run_params.Plug_valve_state==0x55 || g_run_params.Plug_valve_state==0x00 )
                packaged_data[offset++] =  g_run_params.Plug_valve_state;               //0x05������ת  0x50�ط���ת   0x55ȫ����ת   0x00�޶�ת

            //�ۼƿ���ʱ��
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Cycle_OpTim, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
        
            //��̯����
            dec_2_hex_type(packaged_data+offset, 4, g_sys_params.Apportion_Energy, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 4);
            offset += 4;
            
            //ϵͳʵʱʱ��
            dec_2_hex_type(packaged_data+offset, 2, t.tm_year+1900, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            packaged_data[offset++] = t.tm_mon+1;
            packaged_data[offset++] = t.tm_mday;
            packaged_data[offset++] = t.tm_hour;
            packaged_data[offset++] = t.tm_min;
            packaged_data[offset++] = t.tm_sec;
            packaged_data[offset++] = t.tm_wday;
            
            //��ů����־
            if(g_run_params.WorkFlg==0xAA || g_run_params.WorkFlg==0x00)            //��ů����־��¼  0XAA �ڹ�ů��   0X00 ���ڹ�ů��
                packaged_data[offset++] = g_run_params.WorkFlg;
            
            //�¶��������� 0X00 �û�����  0XAA ϵͳ����   ϵͳ�����������û������ܸ����¶�����ֵ
            if(g_sys_params.TempCtrlType==0x00 || g_sys_params.TempCtrlType==0xAA)
                packaged_data[offset++] = g_sys_params.TempCtrlType;
            
            //�趨�¶�
            dec_2_hex_type(packaged_data+offset, 2, g_sys_params.TempSetValue, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
            
            //Ƿ��״̬
            packaged_data[offset++] = g_sys_params.ControlType;
            
            //�����¶��趨�¶�
            packaged_data[offset++] = g_sys_params.LimtTE_Flag;
            
            //�����¶�����ֵ
            dec_2_hex_type(packaged_data+offset, 2,g_sys_params.LimtTE_Value, ORD_MOTOR);
            bytes_reverse(packaged_data+offset, 2);
            offset += 2;
        
            *packaged_data_size = offset;
        }
            break;
        default:
            return 1;//û�д˷������� ���޷���
            //break;
    }
    
    return 0;
}

