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
 |  							 引 用 变 量 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
extern  const node_struct root;	//根目录
extern  const node_struct bin;	//tinySH自身的命令
extern  const node_struct sbin;	//用户定义命令

/**
 ====================================================================================
 |  							 引 用 函 数 声 明									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */
int copy_path(const path_struct *from , path_struct *to);
void get_node_name(const char *path,char* name);
node_struct* search_node_in( const node_struct *parent_node , const char *node_name );
BOOL node_passwd_verity(const node_struct *node);
node_struct** get_first_node_of(const node_struct *parent_node);
node_struct* search_node(const char * node_name);

/**
 ====================================================================================
 |  							 函 数 节 点 定 义 									|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

NODE_FUN_DEF(sh_cd,"cd",0,"change directory")
{
    path_struct _dir_path;
    char *ptr = (char*)args;
    node_struct *tmp ;
    char    name[NODE_NAME_LEN];

    if(!ptr || !ptr[0]){
        return 1;
    }
    if(ptr[0]=='/'){
        _dir_path.level[0] = dir_path.level[0];
        _dir_path.deep = 0 ;
        ptr++;
    }else{
        copy_path(&dir_path,&_dir_path);
    }
    
    while(*ptr){
        get_node_name(ptr,name);
        if(!strcmp(name,".")){
            //DO NOTHING...
        }else if(!strcmp(name,"..")){
            if(_dir_path.deep){
                _dir_path.deep -- ;
            }
        }else{
            tmp = search_node_in(_dir_path.level[_dir_path.deep],name);
            if(tmp && tmp->type==NODE_TYPE_DIR){
                if(tmp->passwd){
                    if(!node_passwd_verity(tmp)){
                        break;
                    }
                }
                _dir_path.level[++_dir_path.deep] = tmp;
            }else{
                break;
            }
        }
        while(*ptr && *ptr++!='/');  //find next '/'
    }
    if(*ptr=='\0'){
        copy_path(&_dir_path,&dir_path);
        return 0;
    }else{
        printf("tinySH: %s: No such directory\n",args);
        return 2;
    }
}

NODE_FUN_DEF(sh_ls,"ls",0,"list all elements of current or specified dircetory")
{
    path_struct _dir_path ;
    node_struct **childs_ptr = NULL ;
    const char *path = args ;
    const node_struct *current_node = CURRENT_DIR_NODE;

    if(path && *path){
        copy_path(&dir_path,&_dir_path);
        if(!CALL_NODE_FUN(sh_cd,path)){
            current_node = CURRENT_DIR_NODE ;
            copy_path(&_dir_path,&dir_path);
        }else{
            return 1;
        }
    }

    childs_ptr = (node_struct**)current_node->ptr;

    while(*childs_ptr){
        switch((*childs_ptr)->type){
            case NODE_TYPE_DIR:
                printf("%s/ \t",(*childs_ptr)->name);
                break;
            case NODE_TYPE_FUN:
                printf("%s \t",(*childs_ptr)->name);
                break;
            default:;
        }
        childs_ptr ++ ;
    }
    putchar('\n');
    return 0;
}

NODE_FUN_DEF(sh_clean,"clean",0,"clean screen")
{
    putchar(0x0C);	//发送清除界面命令，对Windows超级有效
    return 0;
}

NODE_FUN_DEF(sh_tree,"tree",0,"draw tree of current or specified dircetory to tell user what inside\nUASGE : tree [dir]")
{
    path_struct _dir_path;
    node_struct **_node_ptr[DIR_MAX_DEEP];
    node_struct *this_node = NULL ;
    const char *path = args ;
    int _dir_deep = 0 ;
    int index ;

    _node_ptr[0] = get_first_node_of(CURRENT_DIR_NODE); 
    if( path && *path ){
        copy_path(&dir_path,&_dir_path);
        if(!CALL_NODE_FUN(sh_cd,path)){
            _node_ptr[0] = get_first_node_of(CURRENT_DIR_NODE);
            copy_path(&_dir_path,&dir_path);
        }else{
            return 1;
        }
    }
    
    while(TRUE){
        this_node = *_node_ptr[_dir_deep];

        if(this_node == NULL){	//If this_node==NULL, that means it comes the end of directory. then , go out.
            if(_dir_deep==0){	//At the same time _dir_deep==0, that mean end browes
                break;
            }
            _dir_deep -- ;
            continue ;
        }

        _node_ptr[_dir_deep] ++ ; //next node

        for(index = 0 ; index < _dir_deep ; index ++ ){
            if(*_node_ptr[index]){
                printf("|  "); 
            }else{
                printf("   "); 
            }
        }

        if(*_node_ptr[_dir_deep]){
            putchar('|');
        }else{
            putchar('`');
        }
        printf("--%s",this_node->name);

        if(this_node->passwd){
            printf("[locked]");
        }
        putchar('\n');

        if(this_node->type==NODE_TYPE_DIR && this_node->passwd==NULL){	//if this node is directory and it has no password verify, then go in.
            _node_ptr[++_dir_deep] = get_first_node_of(this_node);
        }
    }
    
    return 0;
}


NODE_FUN_DEF(sh_help,"help",0,"help command")
{
    const char *cmd_name = args ;
    node_struct *cmd_node = NULL ;

    if(cmd_name && *cmd_name){
        cmd_node = search_node(cmd_name);
        if(cmd_node){
            printf("Name  : %s\n",cmd_node->name);
            printf("Type  : ");
            if(cmd_node->type==NODE_TYPE_DIR){
                puts("Directory");
            }else{
                puts("Function");
            }
            printf("Help  : ");
            if(cmd_node->help){
                puts(cmd_node->help);
            }else{
                putchar('\n');
            }
        }else{
            printf("tinySH: %s : No such function or directory\n",cmd_name);
        }
    }else{
        puts("type command hebind , such as : \"help cd\"");
    }
    return 0;
}

NODE_FUN_DEF(sh_exit,"exit",0,"exit console")
{
    is_shell_run = FALSE ;
    return 0 ;
}


/**
 ====================================================================================
 |  					          目 录 节 点 定 义 								|
 |																					|
 |  NOTE: 																			|
 ====================================================================================
 */

NODE_DIR(bin)
{
    &sh_cd,
    &sh_ls,
    &sh_clean,
    &sh_tree,
    &sh_help,
    &sh_exit,
    NULL,
};

/**
 ====================================================================================
 |  							   文  件  结  束  									|
 ====================================================================================
 */
