#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "bsp.h"
//////////////////////////��ʾ//////////////////////////
typedef enum _E_Display_Idx{
    
    SEG_VALVE_OPEN     = 0,         // "��" ��������
    SEG_VALVE_CLOSE    = 1,         // ���ء� �ط�����
    SEG_SYS_FAULT      = 2,         // ��X�� ϵͳ���ϱ�ʶ
    SEG_VALVE_STOPE    = 3,         // ��ת����

    //---- �����1�ֶ� ----
    SEG_1A             = 4,
    SEG_1F             = 5,
    SEG_1E             = 6,
    SEG_TIME           = 7,         // "ʱ��"��ʶ
    
    SEG_1B             = 8,
    SEG_1G             = 9,
    SEG_1C             = 10,
    SEG_1D             = 11,

    //---- �����2�ֶ� ----
    SEG_2A             = 12,
    SEG_2F             = 13,
    SEG_2E             = 14,
    SEG_TEMP_INLET     = 15,         // ��ڷ���
    
    SEG_2B             = 16,
    SEG_2G             = 17,
    SEG_2C             = 18,
    SEG_2D             = 19,

    //---- �����3�ֶ� ----
    SEG_3A             = 20,
    SEG_3F             = 21,
    SEG_3E             = 22,
    SEG_TEMP_RETURN    = 23,         // ���ڷ���
    
    SEG_3B             = 24,
    SEG_3G             = 25,
    SEG_3C             = 26,
    SEG_3D             = 27,

    //---- �����4�ֶ� ----
    SEG_4A             = 28,
    SEG_4F             = 29,
    SEG_4E             = 30,
    SEG_4DP_POINT      = 31,         // ������һ��С����
    
    SEG_4B             = 32,
    SEG_4G             = 33,
    SEG_4C             = 34,
    SEG_4D             = 35,

    //---- �����5�ֶ� ----
    SEG_5A             = 36,
    SEG_5F             = 37,
    SEG_5E             = 38,
    NULL_              = 39,         // ��
    
    SEG_5B             = 40,
    SEG_5G             = 41,
    SEG_5C             = 42,
    SEG_5D             = 43,

    //---- �����6�ֶ� ----
    SEG_6A             = 44,
    SEG_6F             = 45,
    SEG_6E             = 46,
    SEG_6DP_POINT      = 47,         // �����ڶ���С����
    
    
    SEG_6B             = 48,
    SEG_6G             = 49,
    SEG_6C             = 50,
    SEG_6D             = 51,

    //---- �����7�ֶ� ----
    SEG_7A             = 52,
    SEG_7F             = 53,
    SEG_7E             = 54,
    SEG_7DP_POINT      = 55,         // ����������С����
    
    SEG_7B             = 56,
    SEG_7G             = 57,
    SEG_7C             = 58,
    SEG_7D             = 59,

    //---- �����8�ֶ� ----
    SEG_8A             = 60,
    SEG_8F             = 61,
    SEG_8E             = 62,
    SEG_TEMP_DIFFERENCE= 63,         // �²����
    
    SEG_8B             = 64,
    SEG_8G             = 65,
    SEG_8C             = 66,
    SEG_8D             = 67,
    
    //---- ϵͳ��ʶ ----
    SEG_CONNECT        = 68,        // ��ͨ����
    SEG_DISCONNECT     = 69,        // �Ͽ�����
    SEG_HEAT_QUANTITY  = 70,        // ��������
    SEG_COMM_ID        = 71,        // "ͨѶ"����ʾID������

    SEG_DEGREES_CELSIUS= 72,        // ���϶ȡ����
    SEG_KWH            = 73,        // KW.h ǧ��ʱ����
    SEG_DAY            = 74,        // d��ʱ�䵥λ������
    SEG_HOUR           = 75,        // h��ʱ�䵥λ������

    SEG_SIGNAL         = 76,        // �ź�
    SEG_ANT            = 77,        // ���߷���
    SEG_ANT_FULAT      = 78,        // ͨ�Ź��� ��X��
    SEG_BATTERY        = 79         // ��ط���
    
}E_Display_Idx;

/*�ֶε���ʾ����*/
typedef enum _DispMode{
    E_DISPLAY_OFF = 0,    
    E_DISPLAY_ON
}E_DispMode;

/*��ʾ���ſ��ص�״̬*/
typedef enum _DispValveMode{
    E_DISPLAY_VALVE_OPEN = 0,    
    E_DISPLAY_VALVE_CLOSE,
    E_DISPLAY_VALVE_PLUG
}E_DispValveMode;

/*��ʾ�˵���ѯö��*/
typedef enum _MenuIndex{
    E_MENU_TIME = 0,        // ��ʾ ϵͳʱ��
    E_MENU_TEMP_INLET,      // ��ʾ ����¶�
    E_MENU_TEMP_RETURN,     // ��ʾ �����¶�
    E_MENU_TEMP_DIFF,       // ��ʾ �²�
    E_MENU_CONNECT_TIME,    // ��ʾ ��ͨ�ۼ�ʱ��
    E_MENU_FAULT,           // ��ʾ ����״̬��ʾ��
    E_MENU_ID_LEFT,         // ��ʾ ID���
    E_MENU_ID_RIGHT,        // ��ʾ ID�ұ�
    E_MENU_HEAT_QUANTITY    // ��ʾ ���ۼ�����
}E_MenuIndex;

/*! \brief   ˢ����ʾ*/
void disp_update(void);

/*! \brief   ���������ʾ*/
void disp_clear(void);

/*! \brief   ȫ��ʾ*/
void disp_full(void);

/*! \brief   ��ѯ��ʾ*/
void disp_poll(void);

/*! \brief   ��ʾ����*/
void disp_digit(u8 idx, u8 val, u8 disp_sts);

/*! \brief   ��ʾ��ֵ*/
void disp_number(u8 start, u8 end, u32 val);

/*! \brief   ��ʾ�ַ�*/
void disp_char(u8 start, u8 ch);

/*! \brief   ��ʾ�ַ���*/
void disp_str(u8 start, u8 end, const char *str);

/*! \brief ��ʾϵͳʱ�� ������*/
void disp_systime_year_mon_day(void);

/*! \brief  ��ʾϵͳʱ�� ʱ����  */
void disp_systime_hour_min_sec(void);

/*! \brief   ��ʾ��ˮ�¶�*/
void disp_temp_inlet(long temp_inlet);

/*! \brief  ��ʾ��ˮ�¶�*/
void disp_temp_return(long temp_return);

/*! \brief  ��ʾ�²�*/
void disp_temp_difference(long temp_dif, u8 flag);

/*! \brief  ��ʾ�����ۼ�ʱ��*/
void disp_connect_sum_time(u32 connect_time);

/*! \brief  ��ʾϵͳ����״̬��־ */
void disp_sys_fault_flag(u8 code);

/*! \brief  ��ʾϵͳ����״̬*/
void disp_sys_fault(u8 code);

/*! \brief  ��ʾ�豸���루ID��*/
void disp_id(const unsigned char *id, u8 len);

/*! \brief  ��ʾͨѶ���루GN��*/
void disp_gn_id(const unsigned char *id, u8 len);

/*! \brief  ��ʾ������*/
void disp_heat_quantity(u32 heat_quantity);
    
/*! \brief   ��ʾ����״̬*/
void disp_valve_state(E_DispValveMode sts);

/*! \brief   ��ʾ�ź�*/
void disp_signal_state(E_DispMode sts);

/*! \brief   ��ʾ���*/
void disp_battery_state(E_DispMode sts);
    
void disp_sys_version(u8 version_h, u8 version_s);
#endif



//    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_OFF);       //ʱ��
//    lcd_write_seg(disp_buffer, SEG_TEMP_INLET, E_DISPLAY_OFF);       //���
//    lcd_write_seg(disp_buffer, SEG_TEMP_RETURN, E_DISPLAY_OFF);       //����
//    lcd_write_seg(disp_buffer, SEG_CONNECT, E_DISPLAY_OFF);       //��ͨ
//    lcd_write_seg(disp_buffer, SEG_DISCONNECT, E_DISPLAY_OFF);       //�Ͽ�
//    lcd_write_seg(disp_buffer, SEG_HEAT_QUANTITY, E_DISPLAY_OFF);       //����
//    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_OFF);       //ͨѶ
//    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_OFF);       //���϶ȡ����

//    lcd_write_seg(disp_buffer, SEG_KWH, E_DISPLAY_OFF);       //KW.h ǧ��ʱ����
//    lcd_write_seg(disp_buffer, SEG_DAY, E_DISPLAY_OFF);       //����
//    lcd_write_seg(disp_buffer, SEG_HOUR, E_DISPLAY_OFF);       //ͨѶ
//    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_OFF);       //���϶ȡ����
