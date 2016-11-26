#include <stdio.h>
#include "stream.h"
#include "usart.h"
#include "spin_lock.h"

FILE	__stdin, __stdout, __stderr;

/***********************************************************************************
* Function: STDIO_init;
*
* Description: ��ʼ����׼�������;
*           
* Input: 	��
*
* Output:	��;
*          		
* Return:	��;
*
* Note: 	__stdin, __stdout, __stderr ����ʼ��ΪUSART2	
************************************************************************************/
void	STDIO_Init()
{
	/*
	 *			KEIL ֧�ֽṹ�帳ֵ
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
  * @Description:	��׼�ļ�����ӿ�
  *           
  * @Input:  ch         Ҫ���������	
  *          file		����˿�
  *
  * @Output: none;
  *          		
  * @Return: int    ch;
  *
  * @Note:   ���ļ�ֻ�Ըú������ж��壬���������stdio.h��;
  ***********************************************************************************
  */
int fputc(int ch, FILE * file)
{
	if( file && file->output ){		//У��file�ĺϷ���
		Spin_Lock(&file->slock,1,0);
		if( (FILE_OPTION_CONSOLE & file->option) && '\n' == ch){	//ֻ�����ڿ���̨��ʹ�øù���
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
  * @Description:	�ӱ�׼�ļ���ȡ����
  *           
  * @Input:  file		����˿�
  *
  * @Output: none;
  *          		
  * @Return: int    ��ȡ������;
  *
  * @Note:   ���ļ�ֻ�Ըú������ж��壬���������stdio.h��;
  ***********************************************************************************
  */
int fgetc(FILE * file)
{
	if( file && file->input ){		//У��file�ĺϷ���
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
  * @Description:	���ļ���������
  *           
  * @Input:  file		�ļ�
  *          data		����
  *
  * @Output: none;
  *          		
  * @Return: none;
  *
  * @Note:   ͨ�����ж��е��á�����������ֱ�ӻ��ӵ�����fgetc()�����ڵȴ��豸������
  *          ���ź�ʱ�����ж��е��øú��������ڵȴ������������ݡ�
  ***********************************************************************************
  */
void fpost(FILE *file, int data)
{
	if( file && file->isPending ){
		file->isPending = false;
		file->tmp_data = data;	//��������
		OSSemPost(file->semaphore);
	}
}
