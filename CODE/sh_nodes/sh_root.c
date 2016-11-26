
#include <stdio.h>
#include "shell.h"

NODE_FUN_NAMED(sh_hello,"hello")
{
	puts("Hello, this is tinySH !");
	puts("You can add your command as you like.");
	puts("try these command :");
	puts("@ ls    : list items in current or specified directory.");
	puts("@ cd    : change directory.");
	puts("@ tree  : list all items as a tree.");
	puts("@ clean : clean screen.");
	puts("@ ps    : print all task information.");
	puts("@ reset : reset system.");
	puts("@ halt  : halt.");
	return 0;
}

NODE_FUN_NAMED(sh_name,"name")
{
	puts("My name is [Hevake_lcj]");
	return 0;
}

NODE_FUN_NAMED(sh_email,"email")
{
	puts("My E-mail is [hevake_lcj@126.com]");
	return 0;
}

NODE_FUN_NAMED(sh_qq,"qq")
{
	puts("My QQ is [527020730]");
	return 0;
}

NODE_DIR_NAMED(sh_about,"about")
{
	&sh_name,
	&sh_email,
	&sh_qq,		
	0,
};

NODE_DIR(root)
{
	&sh_hello,
	&sh_about,
	0,
};
