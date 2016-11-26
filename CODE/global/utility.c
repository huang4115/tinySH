
/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
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
 |  							   ��    ��    ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#define	SECONDS_PER_MINUTE		60UL
#define	SECONDS_PER_HOUR		(60UL*SECONDS_PER_MINUTE)
#define	SECONDS_PER_DAY			(24UL*SECONDS_PER_HOUR)		//86400 > 65535

/**
 ====================================================================================
 |  							  �� �� �� �� �� ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
bool	g_isChangeEndian = false;					//�Ƿ���Ҫ���ݴ洢ģʽת��

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

/**
 ***********************************************************************************
 * Function: GetRunTime;
 *
 * Description: ��ȡ����ʱ��;
 *           
 * Input: 	--ticks   �������;
 *
 * Output:	--ptime   ʱ��;
 *          		
 * Return:	int       ����ֵ;
 *
 * Note: 
 ***********************************************************************************
 */
void	GetRunTime( U32 ticks , run_time * ptime )
{
	U32	tmp = ticks / OS_TICKS_PER_SEC ;	//����ܵ�����

	ptime->days = tmp/SECONDS_PER_DAY;	//����
	tmp %= SECONDS_PER_DAY ;
	ptime->hours = tmp / SECONDS_PER_HOUR ;	//Сʱ
	tmp %= SECONDS_PER_HOUR ;
	ptime->minutes = tmp / SECONDS_PER_MINUTE ;
	ptime->seconds = tmp % SECONDS_PER_MINUTE ;
}

/***********************************************************************************
* Function: CheckCPUEndian;
*
* Description: ���CPU�����ݴ洢ģʽ;
*           
* Input: 	none;
*
* Output:	g_isChangeEndian    �Ƿ���Ҫ�洢ģʽ�л�;
*          		
* Return:	��
*
* Note:  	ʹ��unionʵ��
************************************************************************************/
Endian	CPU_Endian ;

void	CheckCPUEndian()
{
	union{
		U16	i;
		U8	c[2];
	}test = {0x00ff} ;

	if (test.c[0] == 0x00){
		CPU_Endian = ENDIAN_LITTLE;	//С��
	}else{
		CPU_Endian = ENDIAN_BIG;	//���
	}
}
/***********************************************************************************
* Function: ExchangeEndian;
*
* Description: ת�����ݴ洢ģʽ;
*           
* Input: 	p		Ҫת��������ָ��
*			size	������ռ�Ĵ洢�ռ��С
*
* Output:	��;
*          		
* Return:	��
*
* Note:  	������λ���ǲ���С�˴洢ģʽ�����������ܲ��ô�˴洢ģʽ��������Ϣ����ʱ��Ҫ
*			ת����
************************************************************************************/
void	ExchangeEndian( void* p , U8 size )
{
	BYTE	temp ;
	U8		i ;
	if(g_isChangeEndian){	//����Ƿ���Ҫģʽת��
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
* Description: �����ַ���
*           
* Input:    pl  ������Ϣ
*           Data	���ڽ��յ�ֵ;
*      		
* Output: none;
*          		
* Return: ���������һ�������Ĵ�������TRUE�����򷵻�FALSE;
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
 * Description: �����λϵͳ;
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
	//TODO...��λǰ,���ݴ���

	puts("ResetSystem...");
	Delay_s(1);	//��DTU���㹻��ʱ������������ݷ��Ͳ���

	NVIC_SystemReset();
}

/***********************************************************************************
 * Function: Halt;
 *
 * Description: ͣ��;
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
	Delay_s(1);	//��DTU���㹻��ʱ������������ݷ��Ͳ���

	PWR_EnterSTANDBYMode();	//�ر�ϵͳʱ�Ӳ���
}

/***********************************************************************************
* Function: YesOrNo;
*
* Description:	�û�ȷ��ѡ��
*           
* Input: 	comment	ѡ��˵��
*           default_ret	Ĭ��ֵ������û���û������y��û����nʱ���ظ�ֵ
*      		
* Output: none;
*          		
* Return: bool	�û���ѡ��;
*
* Note:   
************************************************************************************/
bool    YesOrNo( const char * comment , bool default_ret )
{
    char    ch ;

    if(comment)
		puts(comment);

    printf("ȷ��? (Y/N) : ");
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
* Description:	��ӡ����ϵͳʱ��Ƶ��
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
 * Description: ��ʮ�����ƴ�ӡ�ṹ��������������;
 *           
 * Input: 	--name	��������
 *          --pData	�����׵�ַ
 *          --size	���ݴ�С
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
 * Description: �ڳ����쳣ʱ��ӡ������Ϣ;
 *           
 * Input: 	--r_sp	�Ĵ���SP��ֵ
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
 * Description: �رյ��籨��;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 	��PA12���ó����������������ߵ�ƽ
 ***********************************************************************************
 */
void	TurnOffPowerOffAlarm()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE); 	//��ͨGPIOC��ʱ��

	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_12;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA , GPIO_Pin_12);
}
