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
 ***********************************************************************************
 */
void Delay_us(unsigned int times);

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
 		�ӽ��ܵĽǶ��Ͽ��ǣ���ϵͳʱ������ʱ��ϵͳ��������״̬��
 		��ϵͳʱ�Ӽ�ʱ��һ����ĳ��룬����N�����ڵ���ʱ�����ǰ�ԭ������ʱ��
 		Ҫ����ʹ�ñ�����֮ǰ��һ��Ҫ��Timer0������
 ***********************************************************************************
 */
void Delay_ms(unsigned int times);

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
 * Note:	�����ʱ����������136�ꡣ�ʣ����ص���������⡣
 ***********************************************************************************
 */
void Delay_s(unsigned long times);

/*===END_OF_FILE===*/
