#include "stm32f10x.h"
#include <stdio.h>


/**
  *********************************************************************************
  * Function: assert_failed;
  *
  * Description:	��ӡ������֤������ʾ��Ϣ
  *           
  * Input: 	file    �����ļ�·��
  *         line	������
  *
  * Output: none;
  *          		
  * Return: none;
  *
  * Note:   ��������stm32f10x.h�ļ�line 7836����;
  ***********************************************************************************
  */
void assert_failed(u8* file, u32 line)
{
	printf("assert_failed : %s -- %d\n",file,line);
}
