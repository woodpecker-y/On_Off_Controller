SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;D:\Program Files (x86)\;C:\Program Files (x86)\STMicroelectronics\st_toolset\asm;C:\Program Files\Microsoft MPI\Bin\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\dotnet\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Android;C:\Windows\System32;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\IDM Computer Solutions\UltraEdit;C:\Program Files\IDM Computer Solutions\UltraCompare;C:\Program Files (x86)\IDM Computer Solutions\UltraFinder;C:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\Java\jdk1.8.0_161\bin;C:\Program Files (x86)\Java\jdk1.8.0_161\jre\bin;C:\yjh\apache-maven-3.6.3-bin\apache-maven-3.6.3\bin;D:\Program Files\Microsoft VS Code\bin;C:\Program Files\Git\cmd;C:\Program Files\nodejs\;C:\Users\Administrator\AppData\Local\Programs\Python\Python37\Scripts\;C:\Users\Administrator\AppData\Local\Programs\Python\Python37\;C:\Program Files (x86)\Atmel\sam-ba_2.14\drv\;C:\Program Files (x86)\Atmel\sam-ba_2.14;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\Library\;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;C:\Users\Administrator\AppData\Local\Programs\Microsoft VS Code\bin;C:\Users\Administrator\AppData\Local\GitHubDesktop\bin;C:\Users\Administrator\AppData\Roaming\npm
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103CB
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=tongDuanKongZhiQi
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\board.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\sysparams.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\display.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\fy_protocol.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\bsp_timbase.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\valve_deal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\recv_data_deal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\error.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\app_task.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\vtask_com_rf_valve.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\vtask_power_display_temp_runled.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\vtask_calculate.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\vtaskruntimestats.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\croutine.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\event_groups.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\list.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\queue.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\tasks.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\timers.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\port.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\app\obj\heap_4.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\App\Obj\yi_ti_fa_app.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\App\Obj\yi_ti_fa_app.axf" --i32combined --output ".\App\Obj\yi_ti_fa_app.hex"
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o ./../../Bin/App.bin ./App/Obj/App.axf
