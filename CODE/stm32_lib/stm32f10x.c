#include "stm32f10x.h"
#include <stdio.h>


/**
  *********************************************************************************
  * Function: assert_failed;
  *
  * Description:	打印变量验证错误提示信息
  *           
  * Input: 	file    错误文件路径
  *         line	错误行
  *
  * Output: none;
  *          		
  * Return: none;
  *
  * Note:   本函数在stm32f10x.h文件line 7836声明;
  ***********************************************************************************
  */
void assert_failed(u8* file, u32 line)
{
	printf("assert_failed : %s -- %d\n",file,line);
}
