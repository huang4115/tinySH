#ifndef	__APP_CFG_H__
#define	__APP_CFG_H__

//栈空间大小
/**
 *  任务栈实际大小ByteSize = ConfigSize*4 
 *  因为OS_STK的类型为unsigned int , 32Bit
 */
#define	STK_SIZE_START			512 
#define	STK_SIZE_FEEDDOG		256

/**
  **********************************************************************************
  *	 在 OS_CFG.H 文件中已明确规定最低优先级为63
  *  #define OS_LOWEST_PRIO           63
  **********************************************************************************
  */

//优先级
enum{
	TASK_PRIO_START = 0,
	TASK_PRIO_FEEDDOG,
};

#define	WATCHDOG_ENABLE			1		//看门狗使能，（调试时要关闭）
#define	WATCHDOG_FEED_TIME		5		//喂IWDG周期，秒（小于10S） 


/**
 ====================================================================================
 |                        LOOK FOR MISSING #define CONSTANTS  						|
 |																					|
 |																					|
 | NOTE :																			|
 |																					|
 | This section is used to generate ERROR messages at compile time if certain   	|
 | #define constants are MISSING in OS_CFG.H.  This allows you to quickly 			|
 | determine the source of the error.												|
 |																					|
 | You SHOULD NOT change this section UNLESS you would like to add more comments 	|
 | as to the source of the compile time error.										|
 ====================================================================================
 */

#endif	//__APP_CFG_H__
