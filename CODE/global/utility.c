
/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "utility.h"
#include "global.h"
#include "app_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "includes.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>


/**
 ====================================================================================
 |  							   宏    定    义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#define	SECONDS_PER_MINUTE		60UL
#define	SECONDS_PER_HOUR		(60UL*SECONDS_PER_MINUTE)
#define	SECONDS_PER_DAY			(24UL*SECONDS_PER_HOUR)		//86400 > 65535

/**
 ====================================================================================
 |  							  变 量 类 型 定 义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ====================================================================================
 |  							   变  量  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
bool	g_isChangeEndian = false;					//是否需要数据存储模式转换

/**
 ====================================================================================
 |  							   函  数  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ***********************************************************************************
 * Function: GetRunTime;
 *
 * Description: 获取运行时间;
 *           
 * Input: 	--ticks   运行嘀嗒;
 *
 * Output:	--ptime   时间;
 *          		
 * Return:	int       返回值;
 *
 * Note: 
 ***********************************************************************************
 */
void	GetRunTime( U32 ticks , run_time * ptime )
{
	U32	tmp = ticks / OS_TICKS_PER_SEC ;	//算出总的秒数

	ptime->days = tmp/SECONDS_PER_DAY;	//天数
	tmp %= SECONDS_PER_DAY ;
	ptime->hours = tmp / SECONDS_PER_HOUR ;	//小时
	tmp %= SECONDS_PER_HOUR ;
	ptime->minutes = tmp / SECONDS_PER_MINUTE ;
	ptime->seconds = tmp % SECONDS_PER_MINUTE ;
}

/***********************************************************************************
* Function: CheckCPUEndian;
*
* Description: 检查CPU的数据存储模式;
*           
* Input: 	none;
*
* Output:	g_isChangeEndian    是否需要存储模式切换;
*          		
* Return:	无
*
* Note:  	使用union实现
************************************************************************************/
Endian	CPU_Endian ;

void	CheckCPUEndian()
{
	union{
		U16	i;
		U8	c[2];
	}test = {0x00ff} ;

	if (test.c[0] == 0x00){
		CPU_Endian = ENDIAN_LITTLE;	//小端
	}else{
		CPU_Endian = ENDIAN_BIG;	//大端
	}
}
/***********************************************************************************
* Function: ExchangeEndian;
*
* Description: 转换数据存储模式;
*           
* Input: 	p		要转换的数据指针
*			size	数据所占的存储空间大小
*
* Output:	无;
*          		
* Return:	无
*
* Note:  	由于上位机是采用小端存储模式，而本机可能采用大端存储模式。故在信息交互时需要
*			转换。
************************************************************************************/
void	ExchangeEndian( void* p , U8 size )
{
	BYTE	temp ;
	U8		i ;
	if(g_isChangeEndian){	//检查是否需要模式转换
		for( i = 0 ; i < size/2 ; i ++ ){
			temp = ((U8*)p)[i];
			((U8*)p)[i] = ((U8*)p)[size-i-1];
			((U8*)p)[size-i-1] = temp;
		}
	}
}

/***********************************************************************************
* Function: Listen;
*
* Description: 监听字符串
*           
* Input:    pl  监听信息
*           Data	串口接收的值;
*      		
* Output: none;
*          		
* Return: 如果监听到一个完整的串，返回TRUE，否则返回FALSE;
*
* Note:   
************************************************************************************/
bool	Listen( struct listen_unit *pl , BYTE Data )
{
	if( (pl->stringListen[pl->pos]|0x20) - (Data|0x20) == 0 ){
		pl->pos ++ ;  
		if( strlen(pl->stringListen) - pl->pos == 0 ){
			pl->pos = 0 ;
			return	true ;
		}
		return	false ;
	}
	pl->pos = 0 ;		
	return	false ;	
}

/***********************************************************************************
 * Function: ResetSystem;
 *
 * Description: 软件复位系统;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 	none;
 ***********************************************************************************
 */
void	ResetSystem(void)
{
	//TODO...复位前,备份处理

	puts("ResetSystem...");
	Delay_s(1);	//让DTU有足够的时间完成它的数据发送操作

	NVIC_SystemReset();
}

/***********************************************************************************
 * Function: Halt;
 *
 * Description: 停机;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 	none;
 ***********************************************************************************
 */
void	Halt(void)
{
	puts("Halt.");
	Delay_s(1);	//让DTU有足够的时间完成它的数据发送操作

	PWR_EnterSTANDBYMode();	//关闭系统时钟操作
}

/***********************************************************************************
* Function: YesOrNo;
*
* Description:	用户确定选择
*           
* Input: 	comment	选择说明
*           default_ret	默认值，如果用户既没有输入y与没有输n时返回该值
*      		
* Output: none;
*          		
* Return: bool	用户的选择;
*
* Note:   
************************************************************************************/
bool    YesOrNo( const char * comment , bool default_ret )
{
    char    ch ;

    if(comment)
		puts(comment);

    printf("确定? (Y/N) : ");
    ch = getchar();
    putchar('\n');

    if( (ch|0x20) == 'y' ){
        return  true ;
    }else if ( (ch|0x20) == 'n' ){
		return	false ;
	}else{
    	return  default_ret ;
	}
}

/***********************************************************************************
* Function: PrintClock;
*
* Description:	打印各个系统时钟频率
*           
* Input:  none;
*      		
* Output: none;
*          		
* Return: none;
*
* Note:   
************************************************************************************/
void	PrintClock( void )
{
	RCC_ClocksTypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);

	printf("SYSCLK : %10d Hz\n",rcc_clocks.SYSCLK_Frequency);
	printf("HCLK   : %10d Hz\n",rcc_clocks.HCLK_Frequency);
	printf("PCLK1  : %10d Hz\n",rcc_clocks.PCLK1_Frequency);
	printf("PCLK2  : %10d Hz\n",rcc_clocks.PCLK2_Frequency);
	printf("ADCCLK : %10d Hz\n",rcc_clocks.ADCCLK_Frequency);
}

#define	PRINT_VALUE(value)	PrintData(#value,&value,sizeof(value))
#define	PRINT_POINT(point)	PrintData(#point,point,sizeof(*point))

/**
 ***********************************************************************************
 * Function: PrintData;
 *
 * Description: 以十六进制打印结构数据中所有数据;
 *           
 * Input: 	--name	数据名称
 *          --pData	数据首地址
 *          --size	数据大小
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 
 ***********************************************************************************
 */
void	PrintData( const char *name , const void* pData , int size )
{
	int	index = 0 ;
	int	byte_per_line;
	U8*	DataPtr = (U8*)pData;

	printf("Name: %s , At: %p , Size: %d\n",name,DataPtr,size);
	puts("          00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
	puts("---------------------------------------------------------");
	while(size){
		printf("%08X: ",index);
		byte_per_line = 16 ;
		index += byte_per_line ;
		while(byte_per_line--){
			printf("%02X ",*DataPtr++);
			if(--size == 0)
				break ;
		}
		putchar('\n');
	}
}

/**
 ***********************************************************************************
 * Function: Panic;
 *
 * Description: 在出现异常时打印调试信息;
 *           
 * Input: 	--r_sp	寄存器SP的值
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 
 ***********************************************************************************
 */
void	Panic( U32 r_sp )
{	
	int	index ;

	//PrintData("Stack Pointer",(const void*)r_sp,256);

	puts("-------[ucos grobal]-------");
	printf("OSCtxSwCtr     = %d\n",OSCtxSwCtr);
	printf("OSTaskCtr      = %d\n",OSTaskCtr);
	printf("OSPrioCur      = %d\n",OSPrioCur);
	printf("OSTCBCur       = 0x%p\n",OSTCBCur);
	printf("OSPrioHighRdy  = %d\n",OSPrioHighRdy);	
	printf("OSLockNesting  = %d\n",OSLockNesting);
	printf("OSIntNesting   = %d\n",OSIntNesting);
	printf("OSRdyGrp       = %02X\n",OSRdyGrp);
	printf("OSRdyTbl       = ");
	for( index = 0 ; index < OS_RDY_TBL_SIZE ; index ++ ){
		printf("%02X ",OSRdyTbl[index]);	
	}
	putchar('\n');

	puts("-------[current task]-------");
	printf("OSTCBName      = %s\n",OSTCBCur->OSTCBTaskName);
	printf("OSTCBId        = %d\n",OSTCBCur->OSTCBId);
	printf("OSTCBPrio      = %d\n",OSTCBCur->OSTCBPrio);
	printf("OSTCBStat      = %d\n",OSTCBCur->OSTCBStat);
	printf("OSTCBStkBottom = 0x%p\n",OSTCBCur->OSTCBStkBottom);
	printf("OSTCBStkSize   = %d\n",OSTCBCur->OSTCBStkSize*sizeof(OS_STK));
	printf("OSTCBStkPtr    = 0x%p\n",OSTCBCur->OSTCBStkPtr);
	putchar('\n');
}

/**
 ***********************************************************************************
 * Function: TurnOffPowerOffAlarm;
 *
 * Description: 关闭掉电报警;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 	将PA12配置成推挽输出，并输出高电平
 ***********************************************************************************
 */
void	TurnOffPowerOffAlarm()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE); 	//打通GPIOC的时钟

	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_12;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA , GPIO_Pin_12);
}
