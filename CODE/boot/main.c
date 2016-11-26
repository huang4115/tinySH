#include "stm32f10x.h"
#include "includes.h"
#include "define.h"
#include "app_cfg.h"
#include "watchdog.h"
#include "stack.h"																   

extern	void	NVIC_Configuration( void );
extern	void	task_start( void * arg );

int main()
{
	INT8U	errno ;

	NVIC_Configuration();

#if	WATCHDOG_ENABLE > 0
	WATCHDOG_setTimeOut(15);	//如果15秒仍未启动完成，那么就复位
	WATCHDOG_enable();			//调试时要屏蔽
#endif

	OSInit();

	CREATE_TASK(task_start,NULL,TASK_PRIO_START,stack_start,STK_SIZE_START,errno);

	OSStart();
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configure the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  	/* Set the Vector Table base location at 0x20000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  	/* Set the Vector Table base location at 0x08000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  	/* Configure one bit for preemption priority */
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
