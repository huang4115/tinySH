#include "stack.h"

#pragma pack(push) //±£´æ¶ÔÆë×´Ì¬
#pragma pack(8)

#if defined ( __CC_ARM   )

OS_STK	__align(8)	stack_start[STK_SIZE_START];
OS_STK	__align(8)	stack_feeddog[STK_SIZE_FEEDDOG];

#elif defined ( __ICCARM__ )

OS_STK	stack_start[STK_SIZE_START];
OS_STK	stack_feeddog[STK_SIZE_FEEDDOG];

#endif

#pragma pack(pop)
