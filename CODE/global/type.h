#ifndef	__TYPE_H__
#define	__TYPE_H__

#include "os_cpu.h"		//将UCOS的类型定义加进去

typedef	unsigned char	U8;
typedef	char			S8;
typedef	unsigned short	U16;
typedef	short			S16;
typedef	unsigned int	U32;
typedef	int				S32;

typedef	unsigned char	BYTE;
typedef	unsigned int	WORD;
typedef	unsigned long	DWORD;

typedef	unsigned int	UINT;
typedef	unsigned char	UCHAR;

typedef	enum{
	false = 0 , true = 1 , 
}bool;	//为了与OS区别

#endif	//__TYPE_H__
