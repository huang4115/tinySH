/************************************************************************************
 *  Copyright (c), 2009 -  , 
 *            All rights reserved.
 *
 * File name: task_button.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2011.06.09
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description:  
 *	 喂看门狗任务
 * Others: none;
 *
 * variable List: 
 *   +-------------------------+------------------+-----------------+
 *   | NAME                    | TYPE             | ATTRIBUTE       |
 *   +-------------------------+------------------+-----------------+
 *   |                         |                  |                 |
 *   +-------------------------+------------------+-----------------+
 *      	
 * Function List: 
 *	 1. void	task_feeddog( void * arg );
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
 *  NOTE: 																		
 ************************************************************************************
 */
#include "includes.h"
#include "app_cfg.h"
#include "watchdog.h"
#include "delay.h"
#include <stdio.h>

/************************************************************************************
 *  							   变  量  定  义
 *
 *  NOTE: 									
 ************************************************************************************
 */


/************************************************************************************
 *  							   函  数  定  义									
 *  NOTE: 																			
 ************************************************************************************
 */

/***********************************************************************************
 * Function: 	task_feeddog;
 *
 * Description: 喂看门狗任务
 *           
 * Input: 	arg		任务参数;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 
 **********************************************************************************
 */
void	task_feeddog( void * arg )
{
	puts("task_feeddog is running.");

	WATCHDOG_setTimeOut(10);
	while(1){
		WATCHDOG_feed();
		Delay_s(WATCHDOG_FEED_TIME);
	}
}
