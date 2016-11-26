#include "shell.h"
#include <stdio.h>
#include "includes.h"
#include "utility.h"

NODE_FUN_NAMED(sh_reset,"reset")
{
	if(YesOrNo("Reset System",false)){
		ResetSystem();
	}
	return 0;
}

NODE_FUN_NAMED(sh_halt,"halt")
{
	if(YesOrNo("Halt",false)){
		Halt();
	}
	return 0;
}

NODE_FUN_NAMED(sh_ps,"ps")
{
	const OS_TCB*	ptcb = NULL ;
	OS_STK_DATA	stk;
	int	i ;

	puts("=======================================================================");
	puts("|prio|       name       |status|  delay  | StkSize | StkUsed |StkUsage|");
	puts("+----+------------------+------+---------+---------+---------+--------|");
	for( i=0 ; i<=OS_LOWEST_PRIO ; i++ ){
		ptcb = OSTCBPrioTbl[i];
		if(ptcb==NULL||ptcb==(void*)1)
			continue ;
		OSTaskStkChk(i,&stk);	//获取任务栈信息
		printf("| %2d |%18s|   %1d  | %7d | %7d | %7d |  %3d%%  |\n",
			i,ptcb->OSTCBTaskName,ptcb->OSTCBStat,ptcb->OSTCBDly,
			(stk.OSUsed+stk.OSFree),stk.OSUsed,stk.OSUsed*100/(stk.OSUsed+stk.OSFree));
	}
	puts("+----+------------------+------+---------+---------+---------+--------+");
	return 0;
}

extern	path_struct    dir_path;
NODE_DEC(sh_debug);

NODE_FUN_DEF(sh_debug_entry,"dbg",0,"查看调试信息")
{
	if(dir_path.level[dir_path.deep]==&sh_debug || dir_path.deep>=DIR_MAX_DEEP){
		//防止目录重复叠加
		return	1;		
	}
	//将sh_debug目录挂载到当前目录下
	dir_path.level[++dir_path.deep] = &sh_debug;
	return	0;
}

NODE_DIR(sbin)
{
	&sh_reset,
	&sh_halt,
	&sh_ps,
	&sh_debug_entry,
	0,
};

