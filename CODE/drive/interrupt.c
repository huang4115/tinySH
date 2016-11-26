/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "includes.h"
#include "global.h"
#include "usart.h"
#include <stdio.h>
#include "define.h"
#include "utility.h"
#include "delay.h"
#include "type.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

uint32_t __get_sp(void);
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	uint32_t	r_sp ;
	
	r_sp = __get_sp();	//获取SP的值
	
	while (1){
		PERROR(ERROR,Memory Access Error!);
		Panic(r_sp);
		Delay_us(1000);
	} 
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void StateLampControl( BYTE Data );

#include <stdio.h>		//__stdin

/**
  * @brief  This function handles USART1 exception.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint16_t	State = USART1->SR;
	uint16_t	RxData = USART1->DR;

	OSIntEnter();
	if( State & USART_SR_RXNE ){
		
		if(usart_connect_with){
			USART_SendData(usart_connect_with,RxData);
			if(Listen(&g_listen_exit_ucon,RxData)){
				usart_connect_with = NULL;	
			}
		}else{
			fpost(stdin,RxData);
		}
	}
	OSIntExit();
}

/**
  * @brief  This function handles USART2 interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	uint16_t	State = USART2->SR;
	uint16_t	RxData = USART2->DR;

	OSIntEnter();
	if( State & USART_SR_RXNE ){

		if(usart_connect_with==USART2){
			USART_SendData(USART1,RxData);
		}else{
			fpost(&file_usart2,RxData);
		}
	}
	OSIntExit();
}

/**
  * @brief  This function handles USART3 interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	uint16_t	State = USART3->SR;
	uint16_t	RxData = USART3->DR;

	OSIntEnter();
	if( State & USART_SR_RXNE ){

		if(usart_connect_with==USART3){
			USART_SendData(USART1,RxData);
		}else{
			fpost(&file_usart3,RxData);
		}
	}
	OSIntExit();
}

#ifdef STM32F10X_CL

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
	uint16_t	State = UART4->SR;
	uint16_t	RxData = UART4->DR;

	OSIntEnter();
	if( State & USART_SR_RXNE ){

		if(usart_connect_with==UART4){
			USART_SendData(USART1,RxData);
		}else{
			if(file_uart4.isPending){
				file_uart4.isPending = false;
				OSSemPost(file_UART4.semaphore);
			}
		}
	}
	OSIntExit();
}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
	uint16_t	State = UART5->SR;
	uint16_t	RxData = UART5->DR;

	OSIntEnter();
	if( State & USART_SR_RXNE ){

		if(usart_connect_with==UART5){
			USART_SendData(USART1,RxData);
		}else{
			if(file_uart5.isPending){
				file_uart5.isPending = false;
				OSSemPost(file_UART5.semaphore);
			}
		}
	}
	OSIntExit();
}

#endif //STM32F10X_CL

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
