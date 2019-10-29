#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int main()
{
	int p1,p2;
	if(p1=fork())	/*子进程创建成功*/
		putchar('b');
	else
	{
		if(p2=fork())	/*子进程创建成功*/
			putchar('c');
		else
			putchar('a');	/*父进程执行*/
	}
}
