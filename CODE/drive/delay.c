/************************************************************************************
 *  Copyright (c), 2009 -  , �������˹������޹�˾ NFS-CQ
 *            All rights reserved.
 *
 * File name: delay.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : ���
 * Version: 1.00
 * Date   : 2010.12.28
 * Email  : chunjun_cq@nfs-china.com
 * Modification: none
 * 
 * Description: 
 *   ��ʱ����
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
 *  							   ��  ��  ��  ��
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
 *  							   ��  ��  ��  ��
 *
 *  NOTE: 									
 ************************************************************************************
 */

/***********************************************************************************
 * Function: Delay_us;
 *
 * Description: ��ʱ����, ��ʱʱ�䷶Χ: 0~65535us;
 *
 * Input:  times, ��ʱʱ�����;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:   ��ʱʱ�������65535us;
 *         STM32F107   SYSCLK=72MHz ,��ʱ��λΪ 1 ΢��
 ***********************************************************************************
 */
void Delay_us(unsigned int times)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif  

	OS_ENTER_CRITICAL();	//����Ҫ��ȷ��ʱʱ�����ܲ����жϣ�������Ҫ�ر��ж�
	while(times--){
		// 65 ����ָ��������������ֻ����ʱ���á�
		// ���Խ��������ʾ������ʵ�ʵ�λ����Ϊ 0.9966us.
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
 * Description: ��ʱ����, ��ʱʱ�䷶Χ: 0~65535ms;
 *              
 * Input:  times, ��ʱʱ�����;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:	��ʱʱ�������65.535s
 ***********************************************************************************
 */
void Delay_ms(unsigned int times)
{
	if( !OSRunning || OSIntNesting || OSLockNesting /*|| MS_TO_TICKS(times)==0*/ ){	
													// �����ϵͳû������OR�ж���OR��ʱʱ���С
									                // ���times̫С��ʹ��OSTimeDly����ȷ
		while( times -- ){
			Delay_us(1000);
		}	
	}else{
		OSTimeDly(MS_TO_TICKS(times));		//ʹ��OS�ṩ����ʱ����ʵ��
	}
}

/***********************************************************************************
 * Function: Delay_s;
 *
 * Description: ��ʱ����;
 *              
 * Input:  times, ��ʱʱ�����;
 *
 * Output: none;
 *
 * Return: none;
 *
 * Note:	�����ʱ����������136�ꡣ���Բ��ص���������⡣
 ***********************************************************************************
 */
void Delay_s(unsigned long times)
{
	while(times--){
		Delay_ms(1000);		//ʹ��OS�ṩ����ʱ����ʵ��
	}
}

/*==END_OF_FILE==*/
