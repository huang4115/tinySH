/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX������޹�˾
 *            All rights reserved.
 *
 * File name: usart.c
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : ���
 * Version: 1.00
 * Date   : 2011.7.4 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   USART���� 
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
 |  							   ��  ��  ��  ��									|
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
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
USART_TypeDef*	usart_connect_with = NULL ;		//��USART2�����Ĵ���
USART_TypeDef*	usart_list[5] = {NULL,USART2,USART3,UART4,UART5};

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
  *********************************************************************************
  * @Function: USART_SetBaudRate;
  *
  * @Description:	���ò�����
  *           
  * @Input:  --usart ;
  *          --baudRate   ������	
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

	RCC_GetClocksFreq(&Clocks);	//��ȡʱ��

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
  * @Description:	��ȡ������
  *           
  * @Input:  --usart ;
  *
  * @Output: none;
  *          		
  * @Return: INT32U	������;
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
	RCC_GetClocksFreq(&Clocks);	//��ȡʱ��

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
  |             USART1ģ��  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ ȫ�ֱ��� :																	|
  |		file_usart1 	USART1�ļ��ṹ  											|
  | 																				|
  | @ �����б� :																	|
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
  * @Description:	��ʼ��USART1���нӿڣ�������ص�����
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

	//��USART1�ṩʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN, ENABLE);	

#if USART1_REMAP == 0
	/**
	  * �������� 	
	  * TX -> PA9  ������� 1011
	  * RX -> PA10 �������� 0100
	  */
	//��GPIOA�ṩʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2ENR_IOPAEN, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);	//����ӳ��
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

#else  // if USART1_REMAP == 1
	/**
	  * �������� 	
	  * TX -> PB6  ������� 1011
	  * RX -> PB7  �������� 0100
	  */
	  
	//��GPIOB�ṩʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2ENR_IOPBEN, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	//��ӳ��
		
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
	 * R/D0 -> PA8 ���ó��������
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

	//�����ļ������ź���
	file_usart1.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART1_SendData;
  *
  * @Description:	��USART1���нӿڷ���1���ֽ�����
  *           
  * @Input:  ch         Ҫ���͵�����	
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
	while( !( USART1->SR & USART_SR_TC ) );	//�ȴ����ݷ������
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#endif	//USART1_DR == 1
}

/**
  *********************************************************************************
  * @Function: USART1_ReceiveData;
  *
  * @Description:	��USART1�ӿڽ���1���ֽ�����
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       ���յ�������;
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
  |             USART2ģ��  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ ȫ�ֱ��� :																	|
  |		file_usart2 	USART2�ļ��ṹ  											|
  | 																				|
  | @ �����б� :																	|
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
  * @Description:	��ʼ��USART2���нӿڣ�������ص�����
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

	//��USART2��GPIOD�ṩʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN | RCC_APB2ENR_AFIOEN, ENABLE); 	//��USART2ģ���ṩʱ��
	
#if USART2_REMAP == 0
	/**
	  * �������� 	
	  * ��ӳ��
	  * TX -> PA2 ������� 1011
	  * RX -> PA3 �������� 0100
	  */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);		//��GPIOAģ���ṩʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART2,DISABLE);	//����ӳ��

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
#else
	/**
	  * �������� 	
	  * ��ӳ��
	  * TX -> PD5 ������� 1011
	  * RX -> PD6 �������� 0100
	  */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN, ENABLE);		//��GPIODģ���ṩʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);	//��ӳ��
	
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
	 * R/D1 -> PA1 ���ó��������
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

	//�����ļ������ź���
	file_usart2.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART2_SendData;
  *
  * @Description:	��USART2���нӿڷ���1���ֽ�����
  *           
  * @Input:  ch         Ҫ���͵�����	
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
	while( !( USART2->SR & USART_SR_TC ) );	//�ȴ����ݷ������
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
#endif  //USART2_DR == 1
}

/**
  *********************************************************************************
  * @Function: USART2_ReceiveData;
  *
  * @Description:	��USART2�ӿڽ���1���ֽ�����
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       ���յ�������;
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
  |             USART3ģ��  														|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ ȫ�ֱ��� :																	|
  |		file_usart3 	USART3�ļ��ṹ  											|
  | 																				|
  | @ �����б� :																	|
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
  * @Description:	��ʼ��USART3���нӿڣ�������ص�����
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

	//��USART3�ṩʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART3EN | RCC_APB2ENR_AFIOEN, ENABLE); 	//��USART3ģ���ṩʱ��

#if USART3_REMAP == 0

	RCC_APB2PeriphClockCmd(	RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPDEN, ENABLE);		//��GPIOB,GPIODģ���ṩʱ��
	/**
	  * �������� 	
	  * ����ӳ��
	  * TX -> PB10 ������� 1011
	  * RX -> PD11 �������� 0100
	  */
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);		

#elif USART3_REMAP == 1

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//��GPIODģ���ṩʱ��
	/**
	  * �������� 	
	  * ������ӳ��
	  * TX -> PC10 ������� 1011
	  * RX -> PC11 �������� 0100
	  */
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);	//��ӳ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU ;
	GPIO_Init(GPIOC,&GPIO_InitStructure);		

#elif USART3_REMAP == 2

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//��GPIODģ���ṩʱ��
	/**
	  * �������� 	
	  * ��ӳ��
	  * TX -> PD8 ������� 1011
	  * RX -> PD9 �������� 0100
	  */
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);	//��ӳ��
	
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

	//�����ļ������ź���
	file_usart3.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: USART3_SendData;
  *
  * @Description:	��USART3���нӿڷ���1���ֽ�����
  *           
  * @Input:  ch         Ҫ���͵�����	
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
  * @Description:	��USART3�ӿڽ���1���ֽ�����
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       ���յ�������;
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
  |             UART4ģ��  															|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ ȫ�ֱ��� :																	|
  |		file_uart4	UART4�ļ� 												|
  | 																				|
  | @ �����б� :																	|
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
  * @Description:	��ʼ��USART1���нӿڣ�������ص�����
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

	//��UART4��GPIOD�ṩʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART4EN,ENABLE); 	//��UART4ģ���ṩʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//��GPIOCģ���ṩʱ��
	/**
	  * �������� 	
	  * ��ӳ��
	  * TX -> PC10 �������
	  * RX -> PC11 ��������
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

	//�����ļ������ź���
	file_uart4.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: UART4_SendData;
  *
  * @Description:	��USART1���нӿڷ���1���ֽ�����
  *           
  * @Input:  ch         Ҫ���͵�����	
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
  * @Description:	��UART4�ӿڽ���1���ֽ�����
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       ���յ�������;
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
  |             UART5ģ��  															|
  |																					|
  |---------------------------------------------------------------------------------|
  | @ ȫ�ֱ��� :																	|
  |		file_uart5	UART5�ļ� 												|
  | 																				|
  | @ �����б� :																	|
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
  * @Description:	��ʼ��USART1���нӿڣ�������ص�����
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

	//��UART5��GPIOD�ṩʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART5EN,ENABLE); 	//��UART5ģ���ṩʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN ,ENABLE);		//��GPIOC,GPIODģ���ṩʱ��
	/**
	  * �������� 	
	  * ��ӳ��
	  * TX -> PC12 �������
	  * RX -> PD2  ��������
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

	//�����ļ������ź���
	file_uart5.semaphore = OSSemCreate(0);
}

/**
  *********************************************************************************
  * @Function: UART5_SendData;
  *
  * @Description:	��USART1���нӿڷ���1���ֽ�����
  *           
  * @Input:  ch         Ҫ���͵�����	
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
  * @Description:	��UART5�ӿڽ���1���ֽ�����
  *           
  * @Input:  none;	
  *
  * @Output: none;
  *          		
  * @Return: char       ���յ�������;
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
