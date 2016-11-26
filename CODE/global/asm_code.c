#include <stm32f10x.h>

//��ȡ�Ĵ��� SP ��ֵ

#if defined ( __CC_ARM   )

__asm uint32_t __get_sp(void)
{
  mov r0, r13
  bx lr
}

#elif defined ( __ICCARM__ )
#pragma diag_suppress=Pe940   //Ҫ����һ�У�������С��޷���ֵ���ĸ澯

uint32_t __get_sp(void)
{
  __asm("mov r0, r13");
  __asm("bx lr");
}

#endif 


