#include <stm32f10x.h>

//获取寄存器 SP 的值

#if defined ( __CC_ARM   )

__asm uint32_t __get_sp(void)
{
  mov r0, r13
  bx lr
}

#elif defined ( __ICCARM__ )
#pragma diag_suppress=Pe940   //要有这一行，否则会有“无返回值”的告警

uint32_t __get_sp(void)
{
  __asm("mov r0, r13");
  __asm("bx lr");
}

#endif 


