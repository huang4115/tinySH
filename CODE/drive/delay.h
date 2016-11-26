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
 ***********************************************************************************
 */
void Delay_us(unsigned int times);

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
 		从节能的角度上考虑，由系统时钟来计时，系统处于休眠状态，
 		但系统时钟计时有一毫秒的出入，故在N秒以内的延时，还是按原方法延时。
 		要求在使用本函数之前，一定要将Timer0开启。
 ***********************************************************************************
 */
void Delay_ms(unsigned int times);

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
 * Note:	这个延时函数可以延136年。故，不必担心溢出问题。
 ***********************************************************************************
 */
void Delay_s(unsigned long times);

/*===END_OF_FILE===*/
