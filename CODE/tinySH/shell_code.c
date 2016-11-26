/************************************************************************************
 *  Copyright (c), 2009 -  , XXXX软件有限公司
 *            All rights reserved.
 *
 * File name: XXXX.c
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

/**
 ====================================================================================
 |  							   文  件  包  含									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
#include "shell_head.h"
#include <string.h>
#include <stdio.h>

/**
 ====================================================================================
 |  							   变  量  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
path_struct    dir_path;

/**
 ====================================================================================
 |  							 引 用 变 量 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
NODE_DEC(bin);	//tinySH自身的命令
NODE_DEC(sbin);	//用户定义命令

/**
 ====================================================================================
 |  							   函  数  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

int copy_path(const path_struct *from , path_struct *to)
{
    int deep;
    for(deep = 0 ; deep <= from->deep ; deep ++ ){
        to->level[deep] = from->level[deep] ;
    }
    to->deep = from->deep ;
    return deep ;
}

node_struct* search_node_in( const node_struct *parent_node , const char *node_name )
{
    node_struct **childs_ptr = NULL ;

    if(parent_node==NULL || parent_node->type!=NODE_TYPE_DIR){
        return NULL ;
    } 

    childs_ptr = (node_struct**)parent_node->ptr;

    while(*childs_ptr){
        if(!strcmp((*childs_ptr)->name,node_name)){
            return *childs_ptr; 
        }   
        childs_ptr ++ ;
    }

    return NULL;  
}

void get_node_name(const char *path,char* name)
{
    int index ;

    while(*path && *path==ASCII_SPACE && path++ ); //jump space char
    for(index = 0 ; *path && *path!=ASCII_SPACE && *path!='/' && index <NODE_NAME_LEN-1 ; index ++ ){
        *name++ = *path++ ;
    }
    *name = '\0';
}

void pick_cmd_and_args(const char* str, char *cmd , char *args)
{
	int count = 0;

    while(*str && *str==ASCII_SPACE && str++ ); //skip space char
    while(*str && *str!=ASCII_SPACE && 
          ++count<NODE_NAME_LEN ){
        *cmd++ = *str++ ;
    }
    *cmd = '\0';
	count = 0;
    while(*str && *str==ASCII_SPACE && str++ ); //skip space char
    while(*str && ++count<(INPUT_BUFFER_SIZE-NODE_NAME_LEN ) ){
        *args++ = *str++ ;
    }
    *args = '\0';
}

BOOL node_passwd_verity(const node_struct *node)
{
    char input_buffer[PASSWORD_LEN];
	int  input_pos = 0 ;
	char ch;

    printf("tinySH: %s: password verity\n",node->name);
    while(TRUE){
        printf("password : ");
        input_pos = 0 ;
		
        while(TRUE){
            ch = getchar();
            if(ch==ASCII_ENTER){
                input_buffer[input_pos] = '\0';
                break;
            }else if(ch==ASCII_TAB){
                //TODO...
            }else if(ch==ASCII_BACKSPACE){
                if(input_pos){
                    input_pos -- ;
                    putchar(ASCII_BACKSPACE);
                    putchar(ASCII_SPACE);
                    putchar(ASCII_BACKSPACE);
                }
            }else if(ch==ASCII_ESC){
                    putchar('\n');
                    return FALSE;
            }else{
                if(input_pos < PASSWORD_LEN-1){
                    putchar('*'); 
                    input_buffer[input_pos++] = ch ;
                }
            }
        }
        putchar('\n');
        if(!strcmp(input_buffer,node->passwd)){
        return TRUE;
        }
    }
}

node_struct** get_first_node_of(const node_struct *parent_node)
{
    if(parent_node && parent_node->type==NODE_TYPE_DIR)
    {
        return (node_struct**)parent_node->ptr;
    }
    return NULL;
}

node_struct* search_node(const char * node_name)
{
    node_struct *node_ret = NULL ;
    node_ret = search_node_in(CURRENT_DIR_NODE,node_name);	//从当前目录搜索
    
    if( node_ret == NULL && &bin != CURRENT_DIR_NODE ){	//在当前目录没有找到，且当前目录不是bin目录
        node_ret = search_node_in(&bin,node_name);			//则从bin目录搜索
    }
	if( node_ret == NULL && &sbin != CURRENT_DIR_NODE ){//在bin目录没有找到，且当前目录不是sbin目录
        node_ret = search_node_in(&sbin,node_name);			//则从sbin目录搜索
    }
	return	node_ret ;
}

/**
 ====================================================================================
 |  							   文  件  结  束  									|
 ====================================================================================
 */
