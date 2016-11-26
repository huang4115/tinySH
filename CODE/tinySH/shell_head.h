/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: XXXX.h
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
 *   XXXX 
 *
 * Others: none;
 *      	
 * Function List: 
 *	 1. int		XXXX(int tmp);
 *   
 * History:
 *   1. Author:       none  
 *	    Version: 	  0.0
 *	    Date:         2011.XX.XX
 *      Modification: none
 *
 ************************************************************************************
 */

#ifndef	__SHELL_HEAD_H__
#define	__SHELL_HEAD_H__

/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "shell.h"

/**
 ====================================================================================
 |  							   宏    定    义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
// 可配置宏
#define INPUT_BUFFER_SIZE   100  
#define NODE_NAME_LEN       10
#define	PASSWORD_LEN        12

// 不可配置宏

#define ASCII_ENTER      0x0D
#define ASCII_TAB        '\t' 
#define ASCII_BACKSPACE  0x08
#define ASCII_SPACE      ' '
#define ASCII_ESC        27 

/**
 ====================================================================================
 |  							  变 量 类 型 定 义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
typedef enum {
    FALSE = 0 , TRUE ,
}BOOL;

/**
 ====================================================================================
 |  							   变  量  声  明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
extern    BOOL is_shell_run ;

#endif  //__SHELL_HEAD_H__
