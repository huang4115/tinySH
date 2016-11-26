
/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "includes.h"
#include "app_cfg.h"
#include "global.h"
#include "define.h"
#include "usart.h"
#include "delay.h"
#include "shell.h"
#include "stack.h"
#include <stdio.h>

/**
 ====================================================================================
 |  							   宏    定    义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ====================================================================================
 |  							 引 用 函 数 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
void	SOURCE_Init( void );
void	DEVICE_Init( void );
void	CreateTask( void );

void	task_feeddog( void * arg );

/**
 ====================================================================================
 |  							   变  量  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

void	task_start( void * arg )
{
	OS_CPU_SysTickInit();	//启动时钟

	OSStatInit();			//初始化统计任务

	Delay_ms(10);			//延时，等待其它设备就绪

	DEVICE_Init();	
	
	CreateTask();

	Delay_ms(100);

	//进入tinySH
	puts("tinySH is running.");

	while(1){
		puts("press any key to enter tinySH ...");
		getchar(); 		//等待从标准数据IO口输入数据 
		puts("====[ Welcome to tinySH ]====");
		ShellEntry();		//进入控制台
		puts("====[ Exit tinySH ]====");
	}
}

void	CreateTask( void )
{
	INT8U	errno = 0 ;

#if	WATCHDOG_ENABLE>0
	//创建看门狗喂狗任务
	CREATE_TASK(task_feeddog, NULL, TASK_PRIO_FEEDDOG, stack_feeddog, STK_SIZE_FEEDDOG, errno);
	if(errno){
		printf("Create task [task_feeddog] fail , errno=%d\n",errno);
	}
#endif
}
										   	
