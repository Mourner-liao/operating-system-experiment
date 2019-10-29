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

	printf("\n������������ڴ��С��");
	scanf("%d",&size);
	while(size>128||size<=0)
	{
		printf("\n������ڴ��С�������������룺");
		scanf("%d",&size);
	}

	if(Distribution(size))
	{
		printf("\n�ڴ�����ɹ���\n");
		print();
	}
	else
		printf("\n�ռ䲻�㣬�ڴ�����ʧ�ܣ�\n");

	MainPage();
}

void RApply()
{
	int num;

	printf("\n��������Ҫ�ͷŵ��ڴ�����ţ�");
	scanf("%d",&num);
	while(num>fullnum||num<=0)
	{
		printf("\n�����ڸ��ڴ���������������룺");
		scanf("%d",&num);
	}

	if(Recovery(num))
	{
		printf("\n�ڴ��ͷųɹ���\n");
		print();
	}
	else
		printf("\n�÷������ǿ��з����������ͷţ�\n");

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

	printf("\n�ڴ�ռ���������\n\n");
	while(p!=last)
	{
		n++;
		p->data.id=n;
		printf("\n�� %d ��������\n",n);
		if(p->data.flag)
			printf("�ѱ�ռ��\n");
		else
			printf("������\n");
		printf("�����ţ�%d \n",p->data.id);
		printf("��ʼ��ַ��%d \n",p->data.address);
		printf("��С��%dK \n",p->data.size);

		p=p->next;
	}
	fullnum=n;
}

void MainPage()
{
	int x;

	printf("\n-------------------------------\n");
	printf("\n��̬��������ģ�⣺�״���Ӧ�㷨\n\n");
	printf("��ʼ�ڴ����ռ��СΪ 128K \n\n");
	printf("1.�����ڴ�\n");
	printf("2.�ͷ��ڴ�\n");
	printf("3.��ʾ��ǰ�ڴ�״��\n");
	printf("4.�˳�\n");
	printf("\n������������еĲ�����");
	scanf("%d",&x);
	while(x<=0 || x>4)
	{
		printf("\n�����������������룺");
		scanf("%d",&x);
	}

	switch(x)
	{
		case 1:DApply();	break;
		case 2:RApply();	break;
		case 3:print();
			   MainPage();	break;
		case 4:
		default:printf("\n���˳���\n\n");
				exit(1);	break;
	}
}

int main()
{
	Initialization();
	MainPage();
}
