#ifndef	__UTILITY_H__
#define	__UTILITY_H__

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "type.h"

/**
 ====================================================================================
 |  							  �� �� �� �� �� ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
typedef struct{
	U16	days;
	U8	hours;
	U8	minutes;
	U8	seconds;	
}run_time;

typedef enum{
	ENDIAN_LITTLE,ENDIAN_BIG,
}Endian;

//�ַ���������Ϣ�ṹ��
struct listen_unit{
    const char *stringListen;
    U8      pos ;
};

/**
 ====================================================================================
 |  							   ��  ��  ��  ��									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
extern	Endian	CPU_Endian ;

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
void	GetRunTime( U32 ticks , run_time * ptime );

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
void	CheckCPUEndian(void);

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
void	ExchangeEndian( void* p , U8 size );

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
bool	Listen( struct listen_unit *pl , BYTE Data );

/***********************************************************************************
 * Function: ResetSystem;
 *
 * Description: ������λϵͳ;
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
void	ResetSystem(void);

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
void	Halt(void);

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
bool    YesOrNo( const char * comment , bool default_ret );

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
void	PrintClock( void );

/**
 ***********************************************************************************
 * Function: PrintData;
 *
 * Description: ��ӡ����;
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
void	PrintData( const char *name , const void* pData , int size );

//#define	PRINT_VALUE(value)	PrintData(#value,&value,sizeof(value))
#define	PRINT_POINT(point)	PrintData(#point,point,sizeof(*point))

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
void	Panic( U32 r_sp );

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
void	TurnOffPowerOffAlarm(void);

#endif	//__UTILITY_H__