#include "includes.h"

void App_TaskCreateHook(OS_TCB  *ptcb)
{

}

void App_TaskDelHook(OS_TCB *ptcb)
{

}

void App_TaskIdleHook(void)
{

}

void App_TaskReturnHook(OS_TCB *ptcb)
{

}

void App_TaskStatHook(void)
{

}

#if OS_TASK_SW_HOOK_EN > 0u
void App_TaskSwHook(void)
{

}
#endif

void App_TCBInitHook(OS_TCB *ptcb)
{

}

#if OS_TIME_TICK_HOOK_EN > 0u
void App_TimeTickHook(void)
{

}
#endif
