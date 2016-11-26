#ifndef    __STREAM_H__
#define    __STREAM_H__

#include <stdio.h>
#include "includes.h"
#include "type.h"
#include "spin_lock.h"

#define	FILE_OPTION_CONSOLE         0x01

struct __FILE{
	void      (*output)(char);    	//输出数据
	char	  (*input)();		    //获取数据

	OS_EVENT  *semaphore;		    //接收信号量
	slock_t   slock;				//自旋锁

	bool      isPending;			//正在等待标志
	INT16U    time_out;	   	    	//等特超时
	INT8U     option;			
   /*
    *****************************************************************
    *  选项配置:
	*    @ FILE_OPTION_CONSOLE     控制台选项，遇到'\n'，补打'\r'字符
	*****************************************************************
	*/
	int       tmp_data;				//临时传递的数据
	INT8U     errno;				//错误码
};

typedef struct __FILE FILE;

extern FILE	__stdin, __stdout, __stderr;

/***********************************************************************************
* Function: STDIO_Init;
*
* Description: 初始化标准输入输出;
*           
* Input: 	无
*
* Output:	无;
*          		
* Return:	无;
*
* Note: 	__stdin, __stdout, __stderr 都初始化为USART1	
************************************************************************************/
void	STDIO_Init( void );

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
int fputc(int ch, FILE * file);

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
int fgetc(FILE * file);

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
void fpost(FILE *file, int data);

#endif    //__STREAM_H__
