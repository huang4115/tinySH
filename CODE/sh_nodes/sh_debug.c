#include "shell.h"
#include <stdio.h>
#include "includes.h"
#include "global.h"
#include "app_cfg.h"


NODE_FUN_NAMED(sh_debug_os,"os")
{
	printf("Task number  : %d\n",OSTaskCtr);
	printf("CPU usage    : %d %%\n",OSCPUUsage);
	printf("OS Version   : %d\n",OS_VERSION);
	printf("OSTime       : %d\n",OSTime);
	printf("OSRdyGrp     : %02xh\n",OSRdyGrp);
	printf("OSRdyTbl[]   : %08x-%08xh\n",*(U32*)(OSRdyTbl+4),*(U32*)OSRdyTbl);
	printf("OSCtxSwCtr   : %d\n",OSCtxSwCtr);
	printf("OSIntNesting : %d\n",OSIntNesting);
	printf("OSLockNesting: %d\n",OSLockNesting);
	printf("OSPrioCur    : %d\n",OSPrioCur);
	printf("OSTCBCur->OSTCBPrio: %d\n",OSTCBCur->OSTCBPrio);
	printf("OSTCBCur->OSTCBTaskName: %s\n",OSTCBCur->OSTCBTaskName);
	return	0 ;
}

#define	PrintValue(macro)	printf(#macro " = %d\n",macro)

NODE_FUN_NAMED(sh_debug_cfg,"cfg")
{
	puts("==== APP_CFG ====");
	PrintValue(WATCHDOG_ENABLE);
	PrintValue(WATCHDOG_FEED_TIME);

	puts("press any key ...");
	getchar();

	puts("==== OS_CFG ====");
	PrintValue(OS_MAX_EVENTS);
	PrintValue(OS_MAX_MEM_PART);
	PrintValue(OS_MAX_QS);
	PrintValue(OS_MAX_TASKS);
	PrintValue(OS_LOWEST_PRIO);
	PrintValue(OS_TICKS_PER_SEC);
	PrintValue(OS_TMR_CFG_MAX);
	PrintValue(OS_TMR_CFG_WHEEL_SIZE);
	PrintValue(OS_TMR_CFG_TICKS_PER_SEC);
	//PrintValue(OS_MEM_FLOAT_PTR_EN);

	return	0;
}

NODE_DIR_DEF(sh_debug,"dbg",0,0)
{	
	&sh_debug_os,
	&sh_debug_cfg,
	0,
};
