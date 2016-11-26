/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: XXXX.h
 * Project  : STM32F10X
 * Processor: STM32F10X
 * Compiler : ARM Compiler
 * 
 * Author : 李春君
 * Version: 1.00
 * Date   : 2011.XX.XX 
 * Email  : hevake_lcj@126.com
 * Modification: none
 * 
 * Description: 
 *   XXXX 
 *
 * Others: none;
 *      	
 * Function List: 
 *	 1. int		XXXX(int tmp);
 *   
 * History:
 *   1. Author:       none  
 *	    Version: 	  0.0
 *	    Date:         2011.XX.XX
 *      Modification: none
 *
 ************************************************************************************
 */

#ifndef	__SHELL_H__
#define	__SHELL_H__

/**
 ====================================================================================
 |  							   宏    定    义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#define DIR_MAX_DEEP        10

////////////////////////////////////
// unconfigable

#define NODE_DEC(node)  \
    extern const node_struct node

#define CALL_NODE_FUN(node,args)\
    fun_node_##node(args)

#define  CURRENT_DIR_NODE    (dir_path.level[dir_path.deep])


#define NODE_FUN_DEF(node,node_name,passwd,help) \
    int  fun_node_##node ( const char * ); \
    const node_struct node={NODE_TYPE_FUN,node_name,(void*)fun_node_##node,passwd,help}; \
    int  fun_node_##node ( const char * args )

#define NODE_FUN(node)  \
    NODE_FUN_DEF(node,#node,0,0)

#define NODE_FUN_NAMED(node,name)  \
    NODE_FUN_DEF(node,name,0,0)

///////////////////////////

#define NODE_DIR_DEF(node,name,passwd,help)  \
    const node_struct *dir_node_##node[];\
    const node_struct node={NODE_TYPE_DIR,name,(void*)dir_node_##node,passwd,help}; \
    const node_struct *dir_node_##node[]=

#define NODE_DIR(node)  \
    NODE_DIR_DEF(node,#node,0,0)

#define NODE_DIR_NAMED(node,name)  \
    NODE_DIR_DEF(node,name,0,0)

#define NODE_DIR_PASSWD(node,passwd)  \
    NODE_DIR_DEF(node,#node,passwd,0)

/**
 ====================================================================================
 |  							  变 量 类 型 定 义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
typedef enum{
    NODE_TYPE_DIR = 0 , 
    NODE_TYPE_FUN ,
}node_type; 

typedef struct _node{
    node_type type ;
    char      *name;
	void      *ptr ;
	char      *passwd ;
	char      *help ;
}node_struct;

typedef struct{
    const  node_struct *level[DIR_MAX_DEEP];
    int    deep ;
}path_struct;

typedef int (*node_fun_t) (const char*);

/**
 ====================================================================================
 |  							   变  量  声  明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
extern    path_struct    dir_path;

/**
 ***********************************************************************************
 * Function: ShellEntry;
 *
 * Description: shell入口;
 *           
 * Input: 	none;
 *
 * Output:	none;
 *          		
 * Return:	none;
 *
 * Note: 
 ***********************************************************************************
 */
void ShellEntry(void);


#endif	//__SHELL_H__
/**
 ====================================================================================
 |  							   文  件  结  束  									|
 ====================================================================================
 */
