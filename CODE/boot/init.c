
#include "includes.h"
#include "stm32f10x.h"
#include "usart.h"
#include "stream.h"
#include "global.h"
#include "utility.h"
#include "define.h"


void	PrintTitle( void )
{
	putchar('\n');
	puts("+-------------------------[ tinySH test ]---------------------------+");
	puts("|                                                                   |");
	puts("| Author     : 李春君                                               |");
	puts("| E-mail     : hevake_lcj@126.com                                   |");
	printf("| Version    : V%03d                                                 |\n",g_version);
	puts("+-------------------------------------------------------------------+");
}

void	DEVICE_Init( void )
{
	USART1_Init();
	USART2_Init();
	USART3_Init();

#ifdef STM32F10X_CL
	UART4_Init();
	UART5_Init();
#endif //STM32F10X_CL

	STDIO_Init();	//必须在USARTx初始化之后

	PrintTitle();	//打印标题
	puts("Initializing Device ...");

	CheckCPUEndian();	//检查处理器的存储模式，则对g_isChangeEndian进行设置
}
