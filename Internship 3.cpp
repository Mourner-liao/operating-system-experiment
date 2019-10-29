#include<iostream>

#define MAX_SIZE 8

using namespace std;

void MainPage();
void InitBitMap();
void Apply();
void Recovery();
void Print();

int Map[MAX_SIZE][MAX_SIZE];
int freeNum;	//空闲块数

void MainPage()
{
	int n;

	printf("\n**************************************\n");
	printf("\n位示图管理磁盘存储空间：\n");
	printf("\n  1. 内存空间申请 ");
	printf("\n  2. 内存空间释放 ");
	printf("\n  3. 显示当前的内存空间分配情况 ");
	printf("\n  4. 退出 \n");
	printf("\n请输入你想进行的操作：");
	scanf("%d",&n);
	while(n<1||n>4)
	{
		printf("\n输入有误，请重新输入：");
		scanf("%d",&n);
	}

	switch(n)
	{
		case 1: Apply();	break;
		case 2: Recovery(); break;
		case 3: Print();	break;
		case 4: printf("\n已退出。 \n");
				 exit(1);	break;
	}
}

void InitBitMap()
{
	for(int i=0;i<MAX_SIZE;i++)
		for(int j=0;j<MAX_SIZE;j++)
			Map[i][j]=0;

	freeNum=MAX_SIZE*MAX_SIZE;
}

void Apply()
{
	int n;
	int cylinder, track, record; // 柱面号，磁道号，物理记录号

	printf("\n请输入申请的内存空间大小：");
	scanf("%d",&n);


	if(freeNum>=n)
	{
		printf("\n申请成功！\n");
		printf("\n分配的磁盘空间的物理地址为：\n");
		for(int i=0;i<MAX_SIZE;i++)
		{
			for(int j=0;j<MAX_SIZE;j++)
			{
				if(Map[i][j]==0 && n>0)
				{
					Map[i][j]=1;
					cylinder=i;
					track=j/4;
					record=j%4;
					n--;
					freeNum--;
					printf("\n  柱面号：%d ,磁道号：%d ,物理记录号：%d ",cylinder+1,track+1,record+1);
				}
				if(n==0)
					break;
			}
		}
	}
	else
	{
		printf("\n内存空间不足，申请失败！\n");
		MainPage();
	}

	printf("\n\n当前的内存空间分配情况：\n");
	Print();
}

void Recovery()
{
	int i,j;
	int cylinder, track, record;

	printf("\n请输入柱面号、磁道号、物理记录号（用空格分隔）：");
	scanf("%d %d %d",&cylinder,&track,&record);

	i=cylinder-1;
	j=(track-1)*4+(record-1);

	if(cylinder>MAX_SIZE || track>4 || record>4)
	{
		printf("\n该物理地址不存在！\n");
		MainPage();
	}
	else if(Map[i][j]==0)
	{
		printf("\n该内存块已是空闲状态！\n");
		MainPage();
	}
	else
	{
		Map[i][j]=0;
		freeNum++;
		printf("\n内存释放成功！\n");
		printf("\n  归还块的字节号：%d ,位数：%d \n",i+1,j+1);
	}

	printf("\n当前的内存空间分配情况：\n");
	Print();
}

void Print()
{
	printf("\n当前可用磁盘空间为：%d \n",freeNum);
	for(int i=0;i<MAX_SIZE;i++)
	{
        for(int j=0;j<MAX_SIZE;j++)
        {
        	if(j==0)
        		printf("\n");
        	printf("%4d", Map[i][j]);
		}
        printf("\n");
    }

    MainPage();
}

int main()
{
	InitBitMap();
	MainPage();
}
