/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: usart.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2011.7.4 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   USART驱动 
 *
 * Others: none;
 *      	
 * Function List: 
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
#include "usart.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "includes.h"
#include "delay.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

/**
 ====================================================================================
 |  							   变  量  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
USART_TypeDef*	usart_connect_with = NULL ;		//与USART2相联的串口
USART_TypeDef*	usart_list[5] = {NULL,USART2,USART3,UART4,UART5};

/**
 ====================================================================================
 |  							   函  数  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
  *********************************************************************************
  * @Function: USART_SetBaudRate;
  *
  * @Description:	设置波特率
  *           
  * @Input:  --usart ;
  *          --baudRate   波特率	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   USARTx->BRR = Fclk / baudRate ;
  ***********************************************************************************
  */
void	USART_SetBaudRate(USART_TypeDef *usart , INT32U baudRate)
{
	RCC_ClocksTypeDef Clocks;
	INT32U	tmp = 0 ; 

	assert_param(IS_USART_ALL_PERIPH(usart));

	RCC_GetClocksFreq(&Clocks);	//获取时钟

	if(usart==USART1){
		tmp = Clocks.PCLK2_Frequency ;
	}else{
		tmp = Clocks.PCLK1_Frequency ;
	}

	tmp /= baudRate ;

	usart->BRR = tmp ;
}

/**
  *********************************************************************************
  * @Function: USART_GetBaudRate;
  *
  * @Description:	获取波特率
  *           
  * @Input:  --usart ;
  *
  * @Output: none;
  *          		
  * @Return: INT32U	波特率;
  *
  * @Note:   baudRate = Fclk / USARTx->BRR ;
  ***********************************************************************************
  */
INT32U	USART_GetBaudRate(USART_TypeDef *usart)
{
	RCC_ClocksTypeDef Clocks;
	INT32U	ret = 0 ;
	INT32U	tmp = 0 ; 

	assert_param(IS_USART_ALL_PERIPH(usart));

	tmp = usart->BRR ;
	RCC_GetClocksFreq(&Clocks);	//获取时钟

	if(usart==USART1){
		ret = Clocks.PCLK2_Frequency ;
	}else{
		ret = Clocks.PCLK1_Frequency ;
	}

	ret /= tmp ;	

	return	ret ;
}

/**
  ===================================================================================
  |																					|
  |             USART1模块  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ 全局变量 :																	|
  |		file_usart1 	USART1文件结构  											|
  | 																				|
  | @ 函数列表 :																	|
  |     void	USART1_Init()														|
  |		void	USART1_SendData( char ch )											|
  | 	char	USART1_ReceiveData( void )											|
  ===================================================================================
  */

FILE	file_usart1 = {USART1_SendData,USART1_ReceiveData};

/**
  *********************************************************************************
  * @Function: USART1_init;
  *
  * @Description:	初始化USART1串行接口，配置相关的引脚
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   TX-->PA9
  *          RX-->PA10
  ***********************************************************************************
  */
void	USART1_Init()
{	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//给USART1提供时钟
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN, ENABLE);	

#if USART1_REMAP == 0
	/**
	  * 引脚配置 	
	  * TX -> PA9  推挽输出 1011
	  * RX -> PA10 悬浮输入 0100
	  */
	//给GPIOA提供时钟
	RCC_APB2PeriphClockCmd( RCC_APB2ENR_IOPAEN, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);	//不重映射
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

#else  // if USART1_REMAP == 1
	/**
	  * 引脚配置 	
	  * TX -> PB6  推挽输出 1011
	  * RX -> PB7  悬浮输入 0100
	  */
	  
	//给GPIOB提供时钟
	RCC_APB2PeriphClockCmd( RCC_APB2ENR_IOPBEN, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	//重映射
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

#endif //USART1_REMAP == 0

#if	USART1_DR == 1
	/*
	 * R/D0 -> PA8 配置成推挽输出
	 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#endif	//USART1_DR == 1

	USART_InitStructure.USART_BaudRate            = USART1_INIT_BAUDRATE  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
	USART_Cmd(USART1, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//创建文件接收信号量
	file_usart1.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART1_SendData;
  *
  * @Description:	从USART1串行接口发送1个字节数据
  *           
  * @Input:  ch         要发送的数据	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   none;
  ***********************************************************************************
  */
void	USART1_SendData( char ch )
{
	if(usart_connect_with)
		return ;

#if	USART1_DR == 1
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
#endif	//USART1_DR == 1

	USART_SendData(USART1,ch);
	
#if	USART1_DR == 1
	while( !( USART1->SR & USART_SR_TC ) );	//等待数据发送完毕
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#endif	//USART1_DR == 1
}

/**
  *********************************************************************************
  * @Function: USART1_ReceiveData;
  *
  * @Description:	从USART1接口接收1个字节数据
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       接收到的数据;
  *
  * @Note:   none;
  ***********************************************************************************
  */
char	USART1_ReceiveData( void )
{
	return	USART_ReceiveData(USART1) ;
}

/**
  ===================================================================================
  |																					|
  |             USART2模块  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ 全局变量 :																	|
  |		file_usart2 	USART2文件结构  											|
  | 																				|
  | @ 函数列表 :																	|
  |     void	USART2_Init()														|
  |		void	USART2_SendData( char ch )											|
  | 	char	USART2_ReceiveData( void )											|
  ===================================================================================
  */

FILE	file_usart2 = {USART2_SendData,USART2_ReceiveData};

/**
  *********************************************************************************
  * @Function: USART2_Init;
  *
  * @Description:	初始化USART2串行接口，配置相关的引脚
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   TX-->PD5
  *          RX-->PD6
  ***********************************************************************************
  */
void	USART2_Init()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//给USART2与GPIOD提供时钟
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN | RCC_APB2ENR_AFIOEN, ENABLE); 	//给USART2模块提供时钟
	
#if USART2_REMAP == 0
	/**
	  * 引脚配置 	
	  * 无映射
	  * TX -> PA2 推挽输出 1011
	  * RX -> PA3 悬浮输入 0100
	  */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);		//给GPIOA模块提供时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART2,DISABLE);	//不重映射

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#else
	/**
	  * 引脚配置 	
	  * 重映射
	  * TX -> PD5 推挽输出 1011
	  * RX -> PD6 悬浮输入 0100
	  */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN, ENABLE);		//给GPIOD模块提供时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);	//重映射
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
			
#endif //USART2_REMAP == 0

#if	USART2_DR == 1
	/*
	 * R/D1 -> PA1 配置成推挽输出
	 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
#endif  //USART2_DR == 1

	USART_InitStructure.USART_BaudRate            = USART2_INIT_BAUDRATE  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
	USART_Cmd(USART2, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//创建文件接收信号量
	file_usart2.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART2_SendData;
  *
  * @Description:	从USART2串行接口发送1个字节数据
  *           
  * @Input:  ch         要发送的数据	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   none;
  ***********************************************************************************
  */
void	USART2_SendData( char ch )
{
	if(usart_connect_with==USART2)
		return ;

#if	USART2_DR == 1
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
#endif  //USART2_DR == 1
	
	USART_SendData(USART2,ch);

#if	USART2_DR == 1
	while( !( USART2->SR & USART_SR_TC ) );	//等待数据发送完毕
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
#endif  //USART2_DR == 1
}

/**
  *********************************************************************************
  * @Function: USART2_ReceiveData;
  *
  * @Description:	从USART2接口接收1个字节数据
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       接收到的数据;
  *
  * @Note:   none;
  ***********************************************************************************
  */
char	USART2_ReceiveData( void )
{
	return	USART_ReceiveData(USART2) ;
}

/**
  ===================================================================================
  |																					|
  |             USART3模块  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ 全局变量 :																	|
  |		file_usart3 	USART3文件结构  											|
  | 																				|
  | @ 函数列表 :																	|
  |     void	USART3_Init()														|
  |		void	USART3_SendData( char ch )											|
  | 	char	USART3_ReceiveData( void )											|
  ===================================================================================
  */

FILE	file_usart3 = {USART3_SendData,USART3_ReceiveData};

/**
  *********************************************************************************
  * @Function: USART3_Init;
  *
  * @Description:	初始化USART3串行接口，配置相关的引脚
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   TX-->PD5
  *          RX-->PD6
  ***********************************************************************************
  */
void	USART3_Init()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//给USART3提供时钟
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART3EN | RCC_APB2ENR_AFIOEN, ENABLE); 	//给USART3模块提供时钟

#if USART3_REMAP == 0

	RCC_APB2PeriphClockCmd(	RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPDEN, ENABLE);		//给GPIOB,GPIOD模块提供时钟
	/**
	  * 引脚配置 	
	  * 无重映射
	  * TX -> PB10 推挽输出 1011
	  * RX -> PD11 悬浮输入 0100
	  */
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);		

#elif USART3_REMAP == 1

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//给GPIOD模块提供时钟
	/**
	  * 引脚配置 	
	  * 部分重映射
	  * TX -> PC10 推挽输出 1011
	  * RX -> PC11 悬浮输入 0100
	  */
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);	//重映射
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);		

#elif USART3_REMAP == 2

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//给GPIOD模块提供时钟
	/**
	  * 引脚配置 	
	  * 重映射
	  * TX -> PD8 推挽输出 1011
	  * RX -> PD9 悬浮输入 0100
	  */
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);	//重映射
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);		

#endif

	USART_InitStructure.USART_BaudRate            = USART3_INIT_BAUDRATE  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
	USART_Cmd(USART3, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//创建文件接收信号量
	file_usart3.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART3_SendData;
  *
  * @Description:	从USART3串行接口发送1个字节数据
  *           
  * @Input:  ch         要发送的数据	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   none;
  ***********************************************************************************
  */
void	USART3_SendData( char ch )
{
	if(usart_connect_with==USART3)
		return ;

	USART_SendData(USART3,ch);
}

/**
  *********************************************************************************
  * @Function: USART3_ReceiveData;
  *
  * @Description:	从USART3接口接收1个字节数据
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       接收到的数据;
  *
  * @Note:   none;
  ***********************************************************************************
  */
char	USART3_ReceiveData( void )
{
	return	USART_ReceiveData(USART3) ;
}

#ifdef STM32F10X_CL

/**
  ===================================================================================
  |																					|
  |             UART4模块  															|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ 全局变量 :																	|
  |		file_uart4	UART4文件 												|
  | 																				|
  | @ 函数列表 :																	|
  |     void	UART4_Init()														|
  |		void	UART4_SendData( char ch )											|
  | 	char	UART4_ReceiveData( void )											|
  ===================================================================================
  */

FILE	file_uart4 = {UART4_SendData,UART4_ReceiveData};

/**
  *********************************************************************************
  * @Function: UART4_Init;
  *
  * @Description:	初始化USART1串行接口，配置相关的引脚
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   TX-->PC10
  *          RX-->PC11
  ***********************************************************************************
  */
void	UART4_Init()
{	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//给UART4与GPIOD提供时钟
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART4EN,ENABLE); 	//给UART4模块提供时钟
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//给GPIOC模块提供时钟
	/**
	  * 引脚配置 	
	  * 重映射
	  * TX -> PC10 推挽输出
	  * RX -> PC11 悬浮输入
	  */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);		

	USART_InitStructure.USART_BaudRate            = UART4_INIT_BAUDRATE  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE , ENABLE);
	USART_Cmd(UART4, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//创建文件接收信号量
	file_uart4.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: UART4_SendData;
  *
  * @Description:	从USART1串行接口发送1个字节数据
  *           
  * @Input:  ch         要发送的数据	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   none;
  ***********************************************************************************
  */
void	UART4_SendData( char ch )
{
	if(usart_connect_with==UART4)
		return ;

	USART_SendData(UART4,ch);
}

/**
  *********************************************************************************
  * @Function: UART4_ReceiveData;
  *
  * @Description:	从UART4接口接收1个字节数据
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       接收到的数据;
  *
  * @Note:   none;
  ***********************************************************************************
  */
char	UART4_ReceiveData( void )
{
	Delay_us(20);
	return	USART_ReceiveData(UART4) ;
}

/**
  ===================================================================================
  |																					|
  |             UART5模块  															|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ 全局变量 :																	|
  |		file_uart5	UART5文件 												|
  | 																				|
  | @ 函数列表 :																	|
  |     void	UART5_Init()														|
  |		void	UART5_SendData( char ch )											|
  | 	char	UART5_ReceiveData( void )											|
  ===================================================================================
  */

FILE	file_uart5 = {UART5_SendData,UART5_ReceiveData};

/**
  *********************************************************************************
  * @Function: UART5_Init;
  *
  * @Description:	初始化USART1串行接口，配置相关的引脚
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   TX-->PC12
  *          RX-->PD2
  ***********************************************************************************
  */
void	UART5_Init()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//给UART5与GPIOD提供时钟
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART5EN,ENABLE); 	//给UART5模块提供时钟
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//给GPIOC,GPIOD模块提供时钟
	/**
	  * 引脚配置 	
	  * 重映射
	  * TX -> PC12 推挽输出
	  * RX -> PD2  悬浮输入
	  */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);		

	USART_InitStructure.USART_BaudRate            = UART5_INIT_BAUDRATE  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART5, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//创建文件接收信号量
	file_uart5.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: UART5_SendData;
  *
  * @Description:	从USART1串行接口发送1个字节数据
  *           
  * @Input:  ch         要发送的数据	
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   none;
  ***********************************************************************************
  */
void	UART5_SendData( char ch )
{
	if(usart_connect_with==UART5)
		return ;

	USART_SendData(UART5,ch);
}

/**
  *********************************************************************************
  * @Function: UART5_ReceiveData;
  *
  * @Description:	从UART5接口接收1个字节数据
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       接收到的数据;
  *
  * @Note:   none;
  ***********************************************************************************
  */
char	UART5_ReceiveData( void )
{	
	Delay_us(20);
	return	USART_ReceiveData(UART5) ;
}

#endif //STM32F10X_CL
