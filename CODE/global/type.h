#ifndef	__TYPE_H__
#define	__TYPE_H__

#include "os_cpu.h"		//��UCOS�����Ͷ���ӽ�ȥ

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
}bool;	//Ϊ����OS����

#endif	//__TYPE_H__
