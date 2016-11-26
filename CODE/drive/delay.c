/************************************************************************************
 *  Copyright (c), 2009 -  , 重庆恩菲斯软件有限公司 NFS-CQ
 *            All rights reserved.
 *
 * File name: delay.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2010.12.28
 * Email  : chunjun_cq@nfs-china.com
 * Modification: none
 * 
 * Description: 
 *   延时函数
 *
 * Others: none;
 *
 * variable List: 
 *	 +-------------------------+------------------+-----------------+
 *   | NAME                    | TYPE             | ATTRIBUTE       |
 *   +-------------------------+------------------+-----------------+
 *   |                         |                  |                 |
 *   +-------------------------+------------------+-----------------+
 *      	
 * Function List: 
 *	1. void Delay_us(unsigned int times)			
 *  2. void Delay_ms(unsigned int times)
 *	3. void Delay_s(unsigned long times)
 *   
 * History:
 *   1. Author:       none  
 *	    Version: 	  0.0
 *	    Date:         2009.*.*
 *      Modification: none
 *
 ************************************************************************************
 */

/************************************************************************************
 *  							   文  件  包  含
 *
 *  NOTE: 									
 ************************************************************************************
 */
#include "delay.h"
#include "ucos_ii.h"

#if defined ( __CC_ARM   ) 
	// MDK
	#define __NOP() __nop()
#elif (defined (__ICCARM__))
	// IAR
	#define __NOP()	{ __asm ("nop"); }
#endif

/************************************************************************************
 *  							   函  数  定  义
 *
 *  NOTE: 									
 ************************************************************************************
 */

/***********************************************************************************
 * Function: Delay_us;
 *
 * Description: 延时程序, 延时时间范围: 0~65535us;
 *
 * Input:  times, 延时时间变量;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:   延时时间最大是65535us;
 *         STM32F107   SYSCLK=72MHz ,延时单位为 1 微秒
 ***********************************************************************************
 */
void Delay_us(unsigned int times)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif  

	OS_ENTER_CRITICAL();	//在需要精确延时时，不能产生中断，所以需要关闭中断
	while(times--){
		// 65 个空指令，不作任务操作，只启延时作用。
		// 测试结果，参照示波器，实际单位周期为 0.9966us.
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
	}
	OS_EXIT_CRITICAL();
}

/***********************************************************************************
 * Function: Delay_ms;
 *
 * Description: 延时程序, 延时时间范围: 0~65535ms;
 *              
 * Input:  times, 延时时间变量;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:	延时时间最大是65.535s
 ***********************************************************************************
 */
void Delay_ms(unsigned int times)
{
	if( !OSRunning || OSIntNesting || OSLockNesting /*|| MS_TO_TICKS(times)==0*/ ){	
													// 如果在系统没有运行OR中断内OR延时时间过小
									                // 如果times太小，使用OSTimeDly不精确
		while( times -- ){
			Delay_us(1000);
		}	
	}else{
		OSTimeDly(MS_TO_TICKS(times));		//使用OS提供的延时参数实现
	}
}

/***********************************************************************************
 * Function: Delay_s;
 *
 * Description: 延时程序;
 *              
 * Input:  times, 延时时间变量;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:	这个延时函数可以延136年。所以不必担心溢出问题。
 ***********************************************************************************
 */
void Delay_s(unsigned long times)
{
	while(times--){
		Delay_ms(1000);		//使用OS提供的延时参数实现
	}
}

/*==END_OF_FILE==*/
