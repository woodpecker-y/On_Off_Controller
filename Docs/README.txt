公司LOGO

/***
 *                                                        
 *        ,...                                            
 *      .d' ""                                            
 *      dM`                                               
 *     mMMmm,pW"Wq.`7Mb,od8 `7M'   `MF',pW"Wq.`7MMpMMMb.  
 *      MM 6W'   `Wb MM' "'   VA   ,V 6W'   `Wb MM    MM  
 *      MM 8M     M8 MM        VA ,V  8M     M8 MM    MM  
 *      MM YA.   ,A9 MM         VVV   YA.   ,A9 MM    MM  
 *    .JMML.`Ybmd9'.JMML.       ,V     `Ybmd9'.JMML  JMML.
 *                             ,V                         
 *                          OOb"                          
 */
 
 /***
 *     .o88o.                                                      
 *     888 `"                                                      
 *    o888oo   .ooooo.  oooo d8b oooo    ooo  .ooooo.  ooo. .oo.   
 *     888    d88' `88b `888""8P  `88.  .8'  d88' `88b `888P"Y88b  
 *     888    888   888  888       `88..8'   888   888  888   888  
 *     888    888   888  888        `888'    888   888  888   888  
 *    o888o   `Y8bod8P' d888b        .8'     `Y8bod8P' o888o o888o 
 *                               .o..P'                            
 *                               `Y8P'                             
 *                                                                 
 */

/***
 *                                                                                                        
 *    8 8888888888       ,o888888o.     8 888888888o. `8.`8888.      ,8'  ,o888888o.     b.             8 
 *    8 8888          . 8888     `88.   8 8888    `88. `8.`8888.    ,8'. 8888     `88.   888o.          8 
 *    8 8888         ,8 8888       `8b  8 8888     `88  `8.`8888.  ,8',8 8888       `8b  Y88888o.       8 
 *    8 8888         88 8888        `8b 8 8888     ,88   `8.`8888.,8' 88 8888        `8b .`Y888888o.    8 
 *    8 888888888888 88 8888         88 8 8888.   ,88'    `8.`88888'  88 8888         88 8o. `Y888888o. 8 
 *    8 8888         88 8888         88 8 888888888P'      `8. 8888   88 8888         88 8`Y8o. `Y88888o8 
 *    8 8888         88 8888        ,8P 8 8888`8b           `8 8888   88 8888        ,8P 8   `Y8o. `Y8888 
 *    8 8888         `8 8888       ,8P  8 8888 `8b.          8 8888   `8 8888       ,8P  8      `Y8o. `Y8 
 *    8 8888          ` 8888     ,88'   8 8888   `8b.        8 8888    ` 8888     ,88'   8         `Y8o.` 
 *    8 8888             `8888888P'     8 8888     `88.      8 8888       `8888888P'     8            `Yo 
 */
 
一、工程说明
    1、时间：    2019年08月24日
    2、开发环境：MDK-ARM V5.25a
    3、标准库：  V3.5.0
    4、操作系统：V9.0.0
    5、工程版本：V1.0.0
    6、目标芯片：STM32F10x_MD中容量芯片
	

v0.0.1.190824
    + ADF-STM32Project模板初版发布。
	
代码大小计算：
keil编译后code，RO-data,RW-data,ZI-data含义及mcu的flash实际存储数据 
这些参数的单位是Byte，几个参数分别代表含义如下： 
	Code：代码的大小 
	RO：常量所占空间 
	RW：程序中已经初始化的变量所占空间 
	ZI：未初始化的static变量和全局变量以及堆栈所占的空间 
上述参数和芯片Flash以及SRAM的对应关系是 
	Flash=Code+RO+RW 
	SRAM=RW+ZI

芯片外设情况

【1】CPU内部资源
	CPU型号        	: GD32F103CBT6 / ARM 32-bit Cortex-M3   2.293$
	主频           	: 72MHz
	内部Flash容量	: 128KB
	内部SRAM容量	: 20KB
	IO				：37
	定时器			: 3个16位定时器，每个最多4个IC/OC/PWM或脉冲计数器和
	UART			: 3个
	SPI				: 2个
	I2C				: 2个
	ADC				: 2个独立的12位ADC
	DAC				: 2路独立12位DAC
	CAN				: 1个 CAN 2.0B
	SDIO			: 1个
	USB				: USB2.0全速(从)无需外部PHY。


【2】FSMC存储器地址分配
   	//CPU内部 FLASH  【0x0800 0000 - 0x080F FFFF】,容量512K(0x80000)字节
	//CPU内部 SRAM1  【0x2000 0000 - 0x2000 FFFF】,容量64K(0x10000)字节   
   	CPU内部 FLASH  【0x0800 0000 - 0x0802 0000】,容量128K(0x20000)字节      0x20000 = 131072/1024 = 128k
	CPU内部 SRAM1  【0x2000 0000 - 0x2000 5000】,容量20K(0x10000)字节       0x5000 = 2048/1024 = 20k
	
	//外部 NOR Flash 【0x6400 0000 - 0x64FF FFFF】,容量16M(0x1000000)字节
	//外部 SRAM      【0x6800 0000 - 0x681F FFFF】,容量1M(0x100000)字节
	//DM9000A芯片地址【0x6C10 0000,  0x6C10 0008】, 仅占用2个端口地址
	//TFT LCD地址    【0x6C00 0000,  0x6C00 0002】, 仅占用2个端口地址
	//OLED地址       【0x6C20 0000,  0x6C20 0002】, 仅占用2个端口地址
	//外部 NAND Flash【0x7000 0000】, 容量128M(0xC800000)字节

【3】I2C总线外设地址分配
  序号	器件                      地址
	1	串行EEPROM  AT24C128 	  0xA0
	2	音频CODEC芯片WM8978	      0x34
	3	FM收音机Si4704            0x22
	4	陀螺仪 MPU-6050           0xD0
	5	光照度传感器 BH1750FVI    0x46
	6	大气压 BMP180             0xEE

	7	磁力计HMC5883L            0x3C  【外置的模块，选配】

【4】开发工具
    【仿真器】 --- 非标配，单独选购
	虽然STM32支持串口ISP下载程序，但是不支持在线调试。使用串口下载，开发效率极低，因此强烈推荐购买一个仿真器，提高开发效率。
	- ST-LINK V2 仿真器: ST公司开发的仿真器，可以下载程序和调试跟踪程序。支持STM8和STM32。一般用来开发STM8S。如果开发
	  STM32的话， 还是建议购买J-Link仿真器
	- J-LINK V8,V9仿真器 : Segger公司开发的仿真器，可以下载程序和调试跟踪程序。支持所有ARM系列。

    【USB转RS232串口线】--- 开发板配送
	- 现在大多数PC机都没有硬件串口了。但是对于单片机串口还是很有用的。安富莱的很多例程都需要通过串口打印调试信息。
	- 开发板配送了一根HL-340型号的USB转RS232线。支持WIN XP, WIN7， WIN8 系统。

    【开发软件】
	- 编译环境 IAR EWARMv6 和 KEIL 的 MDK uV4。请按需选择，如都不熟悉，推荐使用KEIL MDK (uV4)。
	- SecureCRT : 一个优秀的功能强大的超级终端工具，主要用来显示开发板从串口打印出来的信息
	- UltraEdit : 一个优秀的源代码编辑工具
	- SourceInsight : 一个优秀的源代码浏览工具

【5】编译和调试方法
  说明：安富莱的例程均提供 KEIL 和 IAR两种工程，每个工程中设置了2个Target，一个是在Flash巡行的，一个是在CpuRAM运行的。请按需要选择。
  如果需要不修改Flash中的程序调试新的例程，可以选择在CPU RAM运行。
  >>>> 有些程序（比如出厂测试程序）体积很大，无法在有限的RAM空间执行。

	【Keil MDK】V4.54
		- 执行菜单 Project -> Open project                 打开 \Project\MDK-ARM(uV4)\project.uvproj （这是工程文件）
 		- 执行菜单 Project -> Rebuild all target files     重新编译所有的文件:
 		- 执行菜单 Debug->Start/Stop Debug Session (Ctrl+F5)  启动调试:

 		【备注1】选择在CPU RAM运行时，不能使用Load按钮下载程序，请直接Start Debug即可。因为Load按钮是专门用于下载到Flash的。
 		【备注2】对于Keil MDK，为了避免中文字符串告警，需要在C/C++编译选项中增加 --diag_suppress=870 参数。
		【备注3】为了提高编译效率。我们缺省关闭了工程设置 Output - Browse Information。因此无法使用鼠标右键查找函数定义。
				  如果你需要这个功能，请打开这个选项开关。

	【IAR EWARM】V6.30
		- 执行菜单 File -> Open Workspace          (打开工程文件: Project\EWARMv6\Project.www)
 		- 执行菜单 Project -> Rebuild All          (重新编译整个工程)
 		- 执行菜单 Project -> Download and Debug(Ctrl+D)   (装载程序并启动调试)

【6】例程文件夹说明
├─Libraries :  存放第3方提供的库或其源代码，这些代码一般是成熟的并经过验证的代码。
│  ├─CMSIS :  CMSIS是ARM公司与多家不同的芯片和软件供应商一起紧密合作定义的，提供了内核与外设、实时操作系统和中间设备之间的通用接口。
│  ├─STM32F10x_StdPeriph_Driver : STM32F10X系列MCU的标准固件库源代码
│  └─STM32_USB-FS-Device_Driver : USB Device Library
│
├─User      :  这里存放用户自己编写的源代码
│  ├─bsp    :  板级支持包（Board Surport Packet），也就是硬件底层驱动程序. 该层介于固件库和应用程序之间。
│  └─fonts  :  存放点阵字库常量数组
│
└─project   :  这里存放各类开发工具的工程文件，编译过程中的临时文件和最终HEX文件都放在此文件夹
    ├─MDK-ARM(uV4) : KEIL公司MDK uVision 4工程文件夹
    └─EWARMv6 : IAR公司的EWARM 工程文件夹

【7】工程中源代码分组说明
└─Flash               : Target名称，Flash 或 CpuRAM
   ├─USER             : 存放 main.c 以及用户用用程序
   ├─BSP              : 板级支持包（Board Support Packet）,存放硬件底层驱动文件
   ├─CMSIS            : 仅存放CMSISI接口文件 system_stm32f4xx.c
   ├─StdPeriph_Drivers: 存放STM328S系列MCU的固件库源代码
   ├─MDK-ARM          : 启动文件（汇编源程序）,对于IAR是 EWARMv6
   └─Doc              : 存放一些文档，仅限txt文件

【8】公共源代码文件说明
	- main.c          : 用户主程序，存放main()函数的文件
	- stm32f1xx_it.c  : 集中存放中断服务程序 【除了几个异常中断外，我们建议其他的ISR放到各自的模块中】
	- stm32f10x_assert.c : 存放断言函数   （一般无需更改）
	- bsp.c           : 底层硬件驱动程序的主程序（包含硬件初始化入口函数）
	- 清除目标文件(用于打包备份).bat : 执行这个程序会删除OBJ,LIST文件，方便对源代码进行压缩打包。

【9】C编译器中的预定义宏 （在工程设置中更改）
	USE_STDPERIPH_DRIVER  - 增加这个符号表示使用ST公司的标准外设库
	VECT_TAB_SRAM         - 增加这个符号表示中断向量表定位在CPU内部RAM （针对在CPU内部RAM运行的工程才需要添加）

【10】调整堆和栈的容量
	对于KEIL MDK, 设置堆和栈的大小是通过修改启动文件start_stm32f10x_hd.s 文件实现的，比如：
		Stack_Size      EQU     0x00008000
		Heap_Size       EQU     0x00000400

	对于IAR EWARM, 直接在工程设置中进行修改即可
		- 菜单 project -> options -> 选择Linker -> Config页 -> 点Edit按钮 -> 切换到CSTACK/HEAP
		比如：
			CSTACK = 0x8000
			HEAP   = 0x400		

【11】输出目标文件
	在 project 下面，有编译好的 hex 文件，用户可以用仿真器直接下载 output(flash).hex 到CPU内部flash	
	\Project\output(mdk).hex - 采用KEIL MDK编译好的文件，定位在CPU 内部Flash	
	\Project\output(iar).hex - 采用IAR编译好的文件，定位在CPU 内部Flash	
	\Project\MDK-ARM(uV4)\Flash\List\output.map  - 这是编译链接文件，文本格式，可以查看每个函数、变量的地址和空间分配情况。



*                2. 任务运行状态的定义如下，跟上面串口打印字母B, R, D, S对应：
*                    #define tskBLOCKED_CHAR		( 'B' )  阻塞
*                    #define tskREADY_CHAR		    ( 'R' )  就绪
*                    #define tskDELETED_CHAR		( 'D' )  删除
*                    #define tskSUSPENDED_CHAR	    ( 'S' )  挂起


