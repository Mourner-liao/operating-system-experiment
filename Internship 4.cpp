#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int main()
{
	int p1,p2;
	if(p1=fork())	/*�ӽ��̴����ɹ�*/
		putchar('b');
	else
	{
		if(p2=fork())	/*�ӽ��̴����ɹ�*/
			putchar('c');
		else
			putchar('a');	/*������ִ��*/
	}
}
