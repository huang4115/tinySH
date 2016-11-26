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
#include <stdio.h>
#include <string.h>

/**
 ====================================================================================
 |  							   变  量  定  义									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
BOOL is_shell_run ;

/**
 ====================================================================================
 |  							 引 用 变 量 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
NODE_DEC(root);	//根目录

/**
 ====================================================================================
 |  							 引 用 函 数 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
void pick_cmd_and_args(const char* str, char *cmd , char *args);
node_struct* search_node(const char * node_name);
BOOL node_passwd_verity(const node_struct *node);

/**
 ====================================================================================
 |  							 函 数 节 点 定 义 									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

void ShellEntry(void)
{
    char ch ;
    char input_buffer[INPUT_BUFFER_SIZE];
    char cmd[NODE_NAME_LEN] , args[INPUT_BUFFER_SIZE-NODE_NAME_LEN];
    int  input_pos ;
    int  index ;
    node_struct *fun_node = NULL ;
    node_fun_t   fun_ptr = NULL ;

    dir_path.level[0] = &root;
    dir_path.deep = 0 ;
    is_shell_run = TRUE ;

    while(is_shell_run){
        //printf("%s$ ",CURRENT_DIR_NODE->name);
        printf("[/");
        for(index = 1 ; index <= dir_path.deep ; index ++ ){
            printf("%s",dir_path.level[index]->name);
            if(index != dir_path.deep){
                putchar('/');
            }
        }
        printf("]# ");

        input_pos = 0 ;

        while(TRUE){
            ch = getchar();
            if(ch==ASCII_ENTER){
                input_buffer[input_pos] = '\0';
                break;
            }else if(ch==ASCII_TAB){
                //TODO...
            }else if(ch==ASCII_ESC){
			    //DO NOTHING.
			}else if(ch==ASCII_BACKSPACE){
                if(input_pos){
                    input_pos -- ;
                    putchar(ASCII_BACKSPACE);
                    putchar(ASCII_SPACE);
                    putchar(ASCII_BACKSPACE);
                }
            }else{
                if(input_pos < INPUT_BUFFER_SIZE-1){
				    putchar(ch); 
                    input_buffer[input_pos++] = ch ;
                }
            }
        }

        putchar('\n');

        if(*input_buffer){ 

            cmd[0] = '\0';
            args[0] = '\0';

            pick_cmd_and_args(input_buffer,cmd,args);

            fun_node = search_node(cmd);

            if(fun_node && fun_node->type == NODE_TYPE_FUN){
                if(fun_node->passwd==NULL || node_passwd_verity(fun_node)){
                    fun_ptr = (node_fun_t)fun_node->ptr; 
                    if(fun_ptr){
                        (*fun_ptr)(args);
                    }
                }
            }else{
                printf("tinySH: %s: No such function\n",cmd);
            }
        }
    }
}
