#ifndef _BOARD_H
#define _BOARD_H

#include <stdio.h>
#include "adf.h"
#include "bsp.h"

#define TEST_FILE_SYSINF				(0x0000) 
#define TEST_FILE_SYSINF_SIZE			(0x2000/sizeof(SysParams)*sizeof(SysParams))
    

#define AUTO_TEST_LOG			(0x2000)
#define AUTO_TEST_LOG_SIZE	   	(0x2000/sizeof(AutoTestStruct)*sizeof(AutoTestStruct))



//#define TEST_FILE_RECHARGE_LOG			(0x2000)
//#define TEST_FILE_RECHARGE_LOG_SIZE	   	(0x2000/sizeof(RechargeLog)*sizeof(RechargeLog))


extern void DELAY_MS(int ms);
    
//////////////////////////////// COM 配置 ////////////////////////////
extern uint8_t com_global_buffer[500];
extern uint8_t rf_global_buffer[80];
extern COMConfig com_cfg[3];

//////////////////////////////// LED /////////////////////////////////
typedef enum _LED_NAME{
    CLOSE_VALVE_LED = 0,
    OPEN_VALVE_LED  = 1,
    RUN_LED   = 2
}LED_NAME;
extern PortTypeDef led_config[3];

//////////////////////////////// LCD //////////////////////////////////
extern HT1621Config lcd_config;

////////////////////////////////外部中断///////////////////////////////
extern EXTIConfig exti_config[1];

///////////////////////////////FLASH//////////////////////////////////
extern FlashConfig flash_config;

////////////////////////////si4432(RF)////////////////////////////////
extern RFConfig rf_config;

////////////////////////// ADC配置 //////////////////////////////////
typedef enum _E_ADC_NUM_Idx{
	E_ADC_TEMP_INLET = 0, 								// 进水温度
	E_ADC_TEMP_RETURN = 1, 								// 回水温度
}E_ADC_NUM_Idx;
extern ADCConfig adc_config[2];

///////////////////////// Motor配置 ////////////////////////////////
extern MotorConfig motor_config;


void system_print(void);
#endif
