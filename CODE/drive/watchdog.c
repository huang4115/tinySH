/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: watchdog.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2011.1.27 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   独立看门狗驱动程序 
 *
 * Others: none;
 *      	
 * Function List: 
 *	 1. void	WATCHDOG_enable( void )
 *   2. void	WATCHDOG_setTimeOut( u8 time )
 *   3. void	WATCHDOG_feed( void )
 *   
 * History:
 *   1. Author:       none  
 *	    Version: 	  0.0
 *	    Date:         2011.XX.XX
 *      Modification: none
 *
 ************************************************************************************
 */

/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "watchdog.h"

/**
 ====================================================================================
 |  							   函  数  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ***********************************************************************************
 * Function: WATCHDOG_enable;
 *
 * Description: 使能独立看门狗;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note:    看门狗复位触发周期为10S
 ***********************************************************************************
 */
void	WATCHDOG_enable( void )
{
	IWDG_WriteAccessCmd(0x5555);	//写入0x5555使之可以对PR与RLR进行访问
	IWDG_SetPrescaler(6);	//6分频，计数周期为6.4MS
	
	IWDG_ReloadCounter();
	IWDG_Enable();
}

/**
 ***********************************************************************************
 * Function: WATCHDOG_setTimeOut;
 *
 * Description: 设置看门狗超时周期，并更新计数器;
 *           
 * Input: 	--time	超时时间，秒;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note:    最大不能超过26秒，否则失效
 ***********************************************************************************
 */
void	WATCHDOG_setTimeOut( u8 time )
{
	u16	reload = (u16)time*(10000/64);	// reload = time*1000/6.4

	if(reload>0xfff)	
		return;

	IWDG_WriteAccessCmd(0x5555);	//写入0x5555使之可以对PR与RLR进行访问
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
}

/**
 ***********************************************************************************
 * Function: WATCHDOG_feed;
 *
 * Description: 喂看门狗;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note:   
 ***********************************************************************************
 */
void	WATCHDOG_feed( void )
{
	IWDG_ReloadCounter();
}

/**
 ====================================================================================
 |  							   文  件  结  束  									|
 ====================================================================================
 */
