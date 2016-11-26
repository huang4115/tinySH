
/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
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
 |  							   ��    ��    ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ====================================================================================
 |  							 �� �� �� �� �� ��									|
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
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

void	task_start( void * arg )
{
	OS_CPU_SysTickInit();	//����ʱ��

	OSStatInit();			//��ʼ��ͳ������

	Delay_ms(10);			//��ʱ���ȴ������豸����

	DEVICE_Init();	
	
	CreateTask();

	Delay_ms(100);

	//����tinySH
	puts("tinySH is running.");

	while(1){
		puts("press any key to enter tinySH ...");
		getchar(); 		//�ȴ��ӱ�׼����IO���������� 
		puts("====[ Welcome to tinySH ]====");
		ShellEntry();		//�������̨
		puts("====[ Exit tinySH ]====");
	}
}

void	CreateTask( void )
{
	INT8U	errno = 0 ;

#if	WATCHDOG_ENABLE>0
	//�������Ź�ι������
	CREATE_TASK(task_feeddog, NULL, TASK_PRIO_FEEDDOG, stack_feeddog, STK_SIZE_FEEDDOG, errno);
	if(errno){
		printf("Create task [task_feeddog] fail , errno=%d\n",errno);
	}
#endif
}
										   	
