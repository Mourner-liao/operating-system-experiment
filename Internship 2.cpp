#include<iostream>
#include<stdio.h>

using namespace std;

struct freespace {
	int id;
	int address;
	int size;
	bool flag;
};

typedef struct DLNode {
	freespace data;
	DLNode *pre;
	DLNode *next;
} MemoryLink;

void print();
void MainPage();
bool Recovery(int num);
bool Distribution(int size);
void DApply();
void RApply();

MemoryLink *head = new DLNode;
MemoryLink *last = new DLNode;
int fullnum;

void Initialization()
{
	head->pre=NULL;
	head->next=last;
	head->data.address=0;
	head->data.size=128;
	head->data.flag=0;
	last->pre=head;
	last->next=NULL;
	last->data.address=128;
	last->data.size=0;
	last->data.flag=0;
}

void DApply()
{
	int size;

	printf("\n请输入申请的内存大小：");
	scanf("%d",&size);
	while(size>128||size<=0)
	{
		printf("\n输入的内存大小有误，请重新输入：");
		scanf("%d",&size);
	}

	if(Distribution(size))
	{
		printf("\n内存申请成功！\n");
		print();
	}
	else
		printf("\n空间不足，内存申请失败！\n");

	MainPage();
}

void RApply()
{
	int num;

	printf("\n请输入想要释放的内存分区号：");
	scanf("%d",&num);
	while(num>fullnum||num<=0)
	{
		printf("\n不存在该内存分区，请重新输入：");
		scanf("%d",&num);
	}

	if(Recovery(num))
	{
		printf("\n内存释放成功！\n");
		print();
	}
	else
		printf("\n该分区已是空闲分区，无需释放！\n");

	MainPage();
}

bool Distribution(int size)
{
	MemoryLink *p = head;

	while(p!=last)
	{
		if(p->data.size>size && !p->data.flag)
		{
			MemoryLink *space = new DLNode;
			space->pre=p;
			space->next=p->next;
			p->next->pre=space;
			p->next=space;
			space->data.address=p->data.address+size;
			space->data.size=p->data.size-size;
			space->data.flag=0;
			p->data.address=0;
			p->data.size=size;
			p->data.flag=1;
			return true;
		}
		else if(p->data.size==size && !p->data.flag)
		{
			p->data.size=size;
			p->data.flag=1;
			return true;
		}
		else if(p->data.size<size || p->data.flag)
			p=p->next;
	}
	return false;
}

bool Recovery(int num)
{
	MemoryLink *q = head;

	while(q!=last)
	{
		if(q->data.id==num && q->data.flag)
		{
			if(q==head)
			{
				if(q->next->data.flag)
				{
					head->data.flag=0;
					return true;
				}
				else if(!q->next->data.flag && q->next->next!=last)
				{
					head->next=head->next->next;
					head->next->next->pre=head;
					head->data.size+=head->next->data.size;
					head->data.flag=0;
					return true;
				}
				else if(!q->next->data.flag && q->next->next==last)
				{
					head->data.size+=head->next->data.size;
					head->data.flag=0;
					head->next=last;
					last->pre=head;
					return true;
				}
			}

			if(!q->pre->data.flag && !q->next->data.flag)
			{
				q->pre->next=q->next->next;
				q->next->next->pre=q->pre;
				q->pre->data.size=q->pre->data.size+q->data.size+q->next->data.size;
				q->pre->data.flag=0;
				return true;
			}
			else if(!q->pre->data.flag)
			{
				q->pre->next=q->next;
				q->next->pre=q->pre;
				q->pre->data.size+=q->data.size;
				return true;
			}
			else if(!q->next->data.flag)
			{
				if(q->next->next==last)
				{
					q->data.size+=q->next->data.size;
					q->data.flag=0;
					q->next=last;
					last->pre=q;
					return true;
				}

				q->next=q->next->next;
				q->next->next->pre=q;
				q->data.size+=q->next->data.size;
				q->data.flag=0;
				return true;
			}
			else if(q->pre->data.flag && q->next->data.flag)
			{
				q->data.flag=0;
				return true;
			}
		}
		else
			q=q->next;
	}
	return false;
}

void print()
{
	int n=0;
	MemoryLink *p = head;

	printf("\n内存空间分配情况：\n\n");
	while(p!=last)
	{
		n++;
		p->data.id=n;
		printf("\n第 %d 个分区：\n",n);
		if(p->data.flag)
			printf("已被占用\n");
		else
			printf("空闲区\n");
		printf("分区号：%d \n",p->data.id);
		printf("起始地址：%d \n",p->data.address);
		printf("大小：%dK \n",p->data.size);

		p=p->next;
	}
	fullnum=n;
}

void MainPage()
{
	int x;

	printf("\n-------------------------------\n");
	printf("\n动态分区分配模拟：首次适应算法\n\n");
	printf("初始内存区空间大小为 128K \n\n");
	printf("1.申请内存\n");
	printf("2.释放内存\n");
	printf("3.显示当前内存状况\n");
	printf("4.退出\n");
	printf("\n请输入你想进行的操作：");
	scanf("%d",&x);
	while(x<=0 || x>4)
	{
		printf("\n输入有误，请重新输入：");
		scanf("%d",&x);
	}

	switch(x)
	{
		case 1:DApply();	break;
		case 2:RApply();	break;
		case 3:print();
			   MainPage();	break;
		case 4:
		default:printf("\n已退出。\n\n");
				exit(1);	break;
	}
}

int main()
{
	Initialization();
	MainPage();
}
