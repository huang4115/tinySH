/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX������޹�˾
 *            All rights reserved.
 *
 * File name: watchdog.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : ���
 * Version: 1.00
 * Date   : 2011.1.27 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   �������Ź��������� 
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
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "watchdog.h"

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
void	WATCHDOG_enable( void )
{
	IWDG_WriteAccessCmd(0x5555);	//д��0x5555ʹ֮���Զ�PR��RLR���з���
	IWDG_SetPrescaler(6);	//6��Ƶ����������Ϊ6.4MS
	
	IWDG_ReloadCounter();
	IWDG_Enable();
}

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
void	WATCHDOG_setTimeOut( u8 time )
{
	u16	reload = (u16)time*(10000/64);	// reload = time*1000/6.4

	if(reload>0xfff)	
		return;

	IWDG_WriteAccessCmd(0x5555);	//д��0x5555ʹ֮���Զ�PR��RLR���з���
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
}

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
void	WATCHDOG_feed( void )
{
	IWDG_ReloadCounter();
}

/**
 ====================================================================================
 |  							   ��  ��  ��  ��  									|
 ====================================================================================
 */
