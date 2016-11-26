#ifndef    __STREAM_H__
#define    __STREAM_H__

#include <stdio.h>
#include "includes.h"
#include "type.h"
#include "spin_lock.h"

#define	FILE_OPTION_CONSOLE         0x01

struct __FILE{
	void      (*output)(char);    	//�������
	char	  (*input)();		    //��ȡ����

	OS_EVENT  *semaphore;		    //�����ź���
	slock_t   slock;				//������

	bool      isPending;			//���ڵȴ���־
	INT16U    time_out;	   	    	//���س�ʱ
	INT8U     option;			
   /*
    *****************************************************************
    *  ѡ������:
	*    @ FILE_OPTION_CONSOLE     ����̨ѡ�����'\n'������'\r'�ַ�
	*****************************************************************
	*/
	int       tmp_data;				//��ʱ���ݵ�����
	INT8U     errno;				//������
};

typedef struct __FILE FILE;

extern FILE	__stdin, __stdout, __stderr;

/***********************************************************************************
* Function: STDIO_Init;
*
* Description: ��ʼ����׼�������;
*           
* Input: 	��
*
* Output:	��;
*          		
* Return:	��;
*
* Note: 	__stdin, __stdout, __stderr ����ʼ��ΪUSART1	
************************************************************************************/
void	STDIO_Init( void );

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
int fputc(int ch, FILE * file);

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
int fgetc(FILE * file);

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
void fpost(FILE *file, int data);

#endif    //__STREAM_H__
