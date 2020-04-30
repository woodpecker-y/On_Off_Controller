/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"
#include "com.h"

//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

//断言
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)
    

/************************************************************************************************************************************************
 *                                                       FreeRTOS基础配置配置选项                                                                 *

 ************************************************************************************************************************************************/
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

// 调度模式配置 0:时间片轮流执行  1:优先级高的任务抢先执行
#define configUSE_PREEMPTION                     1

//1使能时间片调度(默认式使能的)
#define configUSE_TIME_SLICING                   1

//**********************************************************************************************************************

#define configCPU_CLOCK_HZ                      (SystemCoreClock)    //((unsigned long)72000000)        (SystemCoreClock)   // CPU时钟(主频) 单位Hz
#define configTICK_RATE_HZ                      ((TickType_t)1000)                // 系统滴答(系统心跳) 即系统每秒钟滴答的次数，可以说是系统的心跳，但需要和主频区分开来。系统滴答的值要根据CPU主频来看，一般主频越高，取值相对越大，一般在100至1000之间。

/* 某些运行FreeRTOS的硬件有两种方法选择下一个要执行的任务：
 * 通用方法和特定于硬件的方法（以下简称“特殊方法”）。
 * 
 * 通用方法：
 *      1.configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或者硬件不支持这种特殊方法。
 *      2.可以用于所有FreeRTOS支持的硬件
 *      3.完全用C实现，效率略低于特殊方法。
 *      4.不强制要求限制最大可用优先级数目
 * 特殊方法：
 *      1.必须将configUSE_PORT_OPTIMISED_TASK_SELECTION设置为1。
 *      2.依赖一个或多个特定架构的汇编指令（一般是类似计算前导零[CLZ]指令）。
 *      3.比通用方法更高效
 *      4.一般强制限定最大可用优先级数目为32
 * 一般是硬件计算前导零指令，如果所使用的，MCU没有这些硬件指令的话此宏应该设置为0！
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1

/* 置1：使能低功耗tickless模式；置0：保持系统节拍（tick）中断一直运行
 * 假设开启低功耗的话可能会导致下载出现问题，因为程序在睡眠中,可用以下办法解决
 * 
 * 下载方法：
 *      1.将开发版正常连接好
 *      2.按住复位按键，点击下载瞬间松开复位按键
 *     
 *      1.通过跳线帽将 BOOT 0 接高电平(3.3V)
 *      2.重新上电，下载
 *    
 *          1.使用FlyMcu擦除一下芯片，然后进行下载
 *          STMISP -> 清除芯片(z)
 */
#define configUSE_TICKLESS_IDLE                  0   

////启用队列
#define configUSE_QUEUE_SETS                     0   

////开启任务通知功能，默认开启
#define configUSE_TASK_NOTIFICATIONS             1   

#define configUSE_APPLICATION_TASK_TAG           0  

// 系统最大优先级值
#define configMAX_PRIORITIES                     (10)

// 空闲任务使用的堆栈大小、最小堆栈值 (在系统中，一般用于空闲、定时等一些系统任务中，当然，我们有些地方也可以使用这个定义的堆栈值)
#define configMINIMAL_STACK_SIZE                 ((unsigned short)64)
    
// 任务名称最大长度 包含'\0' (创建任务名称最大字符串长度)                           
#define configMAX_TASK_NAME_LEN                  (16)

// 是否使用16位滴答计数值 配置为0：则使用32位的滴答计数值，一般在32位处理器中都是配置为0；配置为1：则使用16位的滴答计数值，一般8位或者16位处理器中配置为1
#define configUSE_16_BIT_TICKS                   0

// 是否让空闲任务“放弃”抢占 配置为0：不放弃抢占；配置为1：放弃抢占；（空闲任务放弃CPU使用权给其他同优先级的用户任务）
#define configIDLE_SHOULD_YIELD                  1

/*是否用MUTEXES（使用互斥信号量）。MUTEXES是任务间通讯的一种方式，特别是用于任务共享资源的应用，比如打印机，任务A用的时候就排斥别的任务应用，用完了别的任务才可以应用。 */
#define configUSE_MUTEXES                        0

// 确定是否用递归式的MUTEXES（使用递归互斥信号量 ）
#define configUSE_RECURSIVE_MUTEXES              0

//（为1时使用计数信号量）是否用计数式的SEMAPHORES，SEMAPHORES也是任务间通讯的一种方式
#define configUSE_COUNTING_SEMAPHORES            0

/*是否应用可切换式的API。freeRTOS 同一功能API有多个，有全功能但是需求资源和时间较多的，此项使能后就可以用较简单的API， 节省资源和时间，但是应用限制较多 */
//#define configUSE_ALTERNATIVE_API                0

/*此项用于DEBUG，来看是否有栈溢出，需要你自己编相应检查函数void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ) */
/**/ //#define configCHECK_FOR_STACK_OVERFLOW         0

/* 设置可以注册的信号量和消息队列个数 用于DEBUG，登记SEMAPHORESQ和QUEUE的最大个数，需要在任务用应用函数vQueueAddToRegistry()和vQueueUnregisterQueue() */
#define configQUEUE_REGISTRY_SIZE                10


/************************************************************************************************************************************************
 *                                                       FreeRTOS与内存申请有关配置选项                                                            *
 ************************************************************************************************************************************************/
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION         1
//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION          0
//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE                    ((size_t)(16*1024))

/************************************************************************************************************************************************
 *                                                       FreeRTOS与钩子函数有关的配置选项                                                          *
 ************************************************************************************************************************************************/
/* 置1：使用空闲钩子（Idle Hook类似于回调函数）；置0：忽略空闲钩子
 * 
 * 空闲任务钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationIdleHook(void )，
 * 这个函数在每个空闲任务周期都会被调用
 * 对于已经删除的RTOS任务，空闲任务可以释放分配给它们的堆栈内存。
 * 因此必须保证空闲任务可以被CPU执行
 * 使用空闲钩子函数设置CPU进入省电模式是很常见的
 * 不可以调用会引起空闲任务阻塞的API函数
 */
#define configUSE_IDLE_HOOK                      0      

/* 置1：使用时间片钩子（Tick Hook）；置0：忽略时间片钩子
 * 
 * 
 * 时间片钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationTickHook(void )
 * 时间片中断可以周期性的调用
 * 函数必须非常短小，不能大量使用堆栈，
 * 不能调用以”FromISR" 或 "FROM_ISR”结尾的API函数
 */
 /*xTaskIncrementTick函数是在xPortSysTickHandler中断函数中被调用的。因此，vApplicationTickHook()函数执行的时间必须很短才行*/
#define configUSE_TICK_HOOK                      0

//使用内存申请失败钩子函数
#define configUSE_MALLOC_FAILED_HOOK             0

/*
 * 大于0时启用堆栈溢出检测功能，如果使用此功能 
 * 用户必须提供一个栈溢出钩子函数，如果使用的话
 * 此值可以为1或者2，因为有两种栈溢出检测方法 */
#define configCHECK_FOR_STACK_OVERFLOW           0

/************************************************************************************************************************************************
 *                                                       FreeRTOS与运行时间和任务状态收集有关的配置选项                                             *
 ************************************************************************************************************************************************/
//启用运行时间统计功能
#define configGENERATE_RUN_TIME_STATS            1
 //启用可视化跟踪调试
#define configUSE_TRACE_FACILITY                 1                          // 1 使能TRACE功能 0禁止   使用要三思  RAM占用很多
/* 与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
 * prvWriteNameToBuffer()
 * vTaskList(),
 * vTaskGetRunTimeStats()
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS     1

extern volatile uint32_t CPU_RunTime;

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()     (CPU_RunTime = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE()             CPU_RunTime   

/************************************************************************************************************************************************
 *                                                       FreeRTOS与协程有关的配置选项                                                             *
 ************************************************************************************************************************************************/
//启用协程，启用协程以后必须添加文件croutine.c
#define configUSE_CO_ROUTINES                    0                 
//协程的有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )                   

/************************************************************************************************************************************************
 *                                                       FreeRTOS与软件定时器有关的配置选项                                                        *
 ************************************************************************************************************************************************/
 //启用软件定时器
#define configUSE_TIMERS                         0                              
//软件定时器优先级
#define configTIMER_TASK_PRIORITY                (configMAX_PRIORITIES-1)        
//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH                 10                               
//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH             (configMINIMAL_STACK_SIZE*2)    

/************************************************************************************************************************************************
 *                                                       FreeRTOS可选函数配置选项                                                                 *
 ************************************************************************************************************************************************/
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/* 选择需要参加编译的功能 删除不需要的功能  节约RAM与ROM */
#define INCLUDE_xTaskGetSchedulerState           1   //设置取得当前任务分配器的状态
#define INCLUDE_vTaskPrioritySet                 0   //设定可以改变任务优先度
#define INCLUDE_uxTaskPriorityGet                1   //设定可以查询任务优先度
#define INCLUDE_vTaskDelete                      1   //设定可以删除任务
#define INCLUDE_vTaskCleanUpResources            1   //据说是可以回收删除任务后的资源(RAM等)
#define INCLUDE_vTaskSuspend                     1   //设置可以把任务挂起
#define INCLUDE_vResumeFromISR                   0   //设置可以从中断恢复(比如系统睡眠，由中断唤醒)
#define INCLUDE_vTaskDelayUntil                  1   //设置任务延迟的绝对时间， 比如现在4：30，延迟到5：00。时间都是绝对时间
#define INCLUDE_vTaskDelay                       1   //设置任务延时， 比如延迟30分钟，相对的时间，现在什么时间，不需要知道
#define INCLUDE_uxTaskGetStackHighWaterMark      0   //是否使能这一函数，函数的目的是返回任务执行后任务堆栈的最小未用数量，同样是为防止堆栈溢出
#define INCLUDE_eTaskGetState                    1   //查询某个任务的运行状态，比如:运行态，阻塞态，挂起态，就绪态等,返回值是个枚举类型.
#define INCLUDE_xTimerPendFunctionCall           0
#define INCLUDE_xTaskGetIdleTaskHandle           0

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
//#define configKERNEL_INTERRUPT_PRIORITY          255                                     //决定了FreeRTOS内核使用的优先级

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
//#define configMAX_SYSCALL_INTERRUPT_PRIORITY     191 /* equivalent to 0xb0, or priority 11. */ //决定了可以调用API函数的中断的最高优先级   高于这个值的中断处理函数不能调用过任何的API函数


/************************************************************************************************************************************************
                                                        FreeRTOS与中断有关的配置选项                                                 
************************************************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		         4                  
#endif


//中断最低优先级
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
/* 这是ST库允许使用的值，ST库允许使用16优先级值，0到15。这必须对应于configKERNEL_INTERRUPT_PRIORITY设置。这里15是最小的NVIC值255. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )   /* 80 */


/************************************************************************************************************************************************
                                                        FreeRTOS与中断服务函数有关的配置选项                         
************************************************************************************************************************************************/
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler           SVC_Handler
#define xPortPendSVHandler        PendSV_Handler
//#define xPortSysTickHandler       SysTick_Handler


/* 以下为使用Percepio Tracealyzer需要的东西，不需要时将 configUSE_TRACE_FACILITY 定义为 0 */
#if ( configUSE_TRACE_FACILITY == 1 )
//#include "trcRecorder.h"
//#define INCLUDE_xTaskGetCurrentTaskHandle               1   // 启用一个可选函数（该函数被 Trace源码使用，默认该值为0 表示不用） //设置当前任务是由哪个任务开启的
#endif



#endif /* FREERTOS_CONFIG_H */




