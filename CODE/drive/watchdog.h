/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: watchdog.h
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2011.XX.XX 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   看门狗模块函数声明 
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

#ifndef	__WATCHDOG_H__
#define	__WATCHDOG_H__

/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "stm32f10x_iwdg.h"

/**
 ====================================================================================
 |  							   函  数  声  明									|
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
void	WATCHDOG_enable( void );

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
void	WATCHDOG_setTimeOut( u8 time );

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
void	WATCHDOG_feed( void );

#endif	//__WATCHDOG_H__
