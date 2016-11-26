
/************************************************************************************
 * File name: define.h
 * 
 * Author : ���
 * Version: 1.00
 * Date   : 2009.5.6 17:34
 * Email  : hevake_lcj@163.com
 * Modification: ����
 * 
 * Description: �����ĺ궨��
 *
 * Others: none;
 *
 * Function List: none;
 * History:
 *   1. Author:       none  
 *	   Version: 	 0.0
 *	   Date:         2009.*.*2
 *      Modification: none
 *
 *
 ************************************************************************************
 */

#ifndef	__DEFINE_H__
#define	__DEFINE_H__

#define	null					0x00

#define	ASCII_ESC	27
#define	ASCII_TAB	'\t'
#define	ASCII_BACK	8

#define	BIT(n)					(1<<(n))
#define	STR(str)				#str
#define	PERROR(type,str)		//printf("\n" #type ": %s---line:%d---%s(){" #str "}\n", __FILE__ ,__LINE__ ,__FUNCTION__)
#define	NumOfArray(array)		(sizeof(array)/sizeof(*array))

#define	MEM_ADDR_VERIFY(base,size,addr) \
			do{\
				if((addr)&&((long)(addr))<(base)||((long)(addr))>=((base)+(size))){\
					printf("\nADDR_ERROR: %s---line:%d---%s(){"#addr"=0x%p}\n", __FILE__ ,__LINE__ ,__FUNCTION__,(addr));\
					while(1);\
				}\
			}while(0)

#define	RAM_ADDR_VERIFY(addr)	MEM_ADDR_VERIFY(0x20000000,0x10000,addr)	
#define	FLASH_ADDR_VERIFY(addr)	MEM_ADDR_VERIFY(0x80000000,0x40000,addr)

#define	TABLE_DECLARE(tableName,elemType)\
		extern const elemType tableName##_list[];\
		extern const int tableName##_length ;

#define	TABLE_BEGIN(tableName,elemType)\
		const elemType  tableName##_list[] = {

#define	TABLE_END(tableName)\
		};\
		const int tableName##_length = sizeof(tableName##_list)/sizeof(*tableName##_list) ;


//Ϊ�˴����࣬�ú��װ��һ�����񴴽�����
#define	CREATE_TASK( task_entry, arg, prio, stack_zoom, stack_size, errno)\
	  		errno = OSTaskCreateExt(task_entry,	\
							(void*)arg,	\
							&stack_zoom[stack_size-1],	\
							prio,	\
							prio,	\
							stack_zoom,		\
							stack_size,		\
							(void*)NULL,	\
							OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);\
			if (!errno){	\
				OSTaskNameSet(prio,#task_entry,&errno);	\
			}
							

/**
 *    �ȴ�������
 *        condition    �ȴ������������ǿɱ�ġ����������᲻����
 *        time_out     ��ʱ�������ǳ�����꣬��ʱ��λ����   
 */
#define	WAIT_FOR( condition , time_out )	\
			do{								\
				int	times = (time_out)*10 ;	\
				while(!(condition) && ((time_out)==0 || times--) )\
					Delay_ms(100);			\
			}while(0)

//�л�U16�Ĵ洢ģʽ
#define ntohs(val)   ((((val)&0x00ff)<<8)|(((val)&0xff00)>>8))	

#endif
