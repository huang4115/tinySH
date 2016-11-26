/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX������޹�˾
 *            All rights reserved.
 *
 * File name: watchdog.h
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : ���
 * Version: 1.00
 * Date   : 2011.XX.XX 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   ���Ź�ģ�麯������ 
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
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "stm32f10x_iwdg.h"

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ***********************************************************************************
 * Function: WATCHDOG_enable;
 *
 * Description: ʹ�ܶ������Ź�;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note:    ���Ź���λ��������Ϊ10S
 ***********************************************************************************
 */
void	WATCHDOG_enable( void );

/**
 ***********************************************************************************
 * Function: WATCHDOG_setTimeOut;
 *
 * Description: ���ÿ��Ź���ʱ���ڣ������¼�����;
 *           
 * Input: 	--time	��ʱʱ�䣬��;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note:    ����ܳ���26�룬����ʧЧ
 ***********************************************************************************
 */
void	WATCHDOG_setTimeOut( u8 time );

/**
 ***********************************************************************************
 * Function: WATCHDOG_feed;
 *
 * Description: ι���Ź�;
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
