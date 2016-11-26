#include <stdio.h>
#include "stream.h"
#include "usart.h"
#include "spin_lock.h"

FILE	__stdin, __stdout, __stderr;

/***********************************************************************************
* Function: STDIO_init;
*
* Description: 初始化标准输入输出;
*           
* Input: 	无
*
* Output:	无;
*          		
* Return:	无;
*
* Note: 	__stdin, __stdout, __stderr 都初始化为USART2	
************************************************************************************/
void	STDIO_Init()
{
	/*
	 *			KEIL 支持结构体赋值
	 */
	__stdin   = file_usart1 ;
	__stdout  = file_usart1 ;
	__stderr  = file_usart1 ;
	
	
	__stdin.option     |= FILE_OPTION_CONSOLE ;
	__stdout.option    |= FILE_OPTION_CONSOLE ;
	__stderr.option    |= FILE_OPTION_CONSOLE ;
}

/**
  *********************************************************************************
  * @Function: fputc;
  *
  * @Description:	标准文件输出接口
  *           
  * @Input:  ch         要输出的数据	
  *          file		输出端口
  *
  * @Output: none;
  *          		
  * @Return: int    ch;
  *
  * @Note:   本文件只对该函数进行定义，相关申声在stdio.h中;
  ***********************************************************************************
  */
int fputc(int ch, FILE * file)
{
	if( file && file->output ){		//校验file的合法性
		Spin_Lock(&file->slock,1,0);
		if( (FILE_OPTION_CONSOLE & file->option) && '\n' == ch){	//只允许在控制台下使用该功能
			file->output('\r');			
		}
	   	file->output(ch);
		Spin_Unlock(&file->slock,1); 
	}
	return	ch ;
}

/**
  *********************************************************************************
  * @Function: fgetc;
  *
  * @Description:	从标准文件获取数据
  *           
  * @Input:  file		输入端口
  *
  * @Output: none;
  *          		
  * @Return: int    获取的数据;
  *
  * @Note:   本文件只对该函数进行定义，相关申声在stdio.h中;
  ***********************************************************************************
  */
int fgetc(FILE * file)
{
	if( file && file->input ){		//校验file的合法性
		file->isPending = true ;
		OSSemPend(file->semaphore,file->time_out,&file->errno) ;
		return	file->tmp_data;
	}else{
		return	0;
	}
}

/**
  *********************************************************************************
  * @Function: fpost;
  *
  * @Description:	向文件发送数据
  *           
  * @Input:  file		文件
  *          data		数据
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   通常在中断中调用。当后面任务直接或间接调用了fgetc()并处于等待设备数据输
  *          入信号时。在中断中调用该函数向正在等待的任务发送数据。
  ***********************************************************************************
  */
void fpost(FILE *file, int data)
{
	if( file && file->isPending ){
		file->isPending = false;
		file->tmp_data = data;	//保存数据
		OSSemPost(file->semaphore);
	}
}
