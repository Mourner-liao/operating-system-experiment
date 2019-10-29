#include<stdio.h>
#include<bits/stdc++.h>
#include<string>
#include<vector>
#include<iostream>

using namespace std;

class PCB
{
	private:
		int name;
		int time;
		int priority;
		char state;

	public:
		void setName(int n)
		{	name=n;	}
		void setTime(int t)
		{	time=t;	}
		void setPriority(int p)
		{	priority=p;	}
		void setState(char s)
		{	state=s; }

		int returnName()
		{	return name;	}
		int returnTime()
		{	return time;	}
		int returnPriority()
		{	return priority;	}
		char returnState()
		{	return state;	}

		void changeTime()
		{	time--;	}
		void changePriority()
		{	priority--;	}
		void changeState(char s)
		{	state=s; }
};

bool cmp(PCB a,PCB b)
{
  return a.returnPriority()>b.returnPriority();
}

void print(vector<PCB> v)
{
    printf("\n\n������\t����ʱ��\t������\t״̬\n");
    printf("*****************************************\n");

    for(unsigned int i = 0; i < v.size(); i++)
        printf("  %d\t    %d\t\t   %d\t  %c\t\n", v[i].returnName(), v[i].returnTime(), v[i].returnPriority(), v[i].returnState());

    printf("\n��ǰ�������еĽ���Ϊ��%d\n",v.front().returnName());
	printf("����ǰ������ʱ�䣺%d  ��������%d  ״̬��%c",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
}

void run(vector<PCB> v)
{
	printf("\n�������ȵ����㷨\n");
	while(!v.empty())
	{
		sort(v.begin(),v.end(),cmp);
		print(v);

		v.front().changeTime();
		v.front().changePriority();

		if(v.front().returnTime()!=0)
		{
			printf("\n���к�����ʱ�䣺%d  ��������%d  ״̬��%c\n",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
			v.push_back(v.front());
			v.erase(v.begin(),v.begin()+1);
		}
		else
		{
			v.front().changeState('E');
			printf("\n���к�����ʱ�䣺%d  ��������%d  ״̬��%c\n",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
			v.erase(v.begin(),v.begin()+1);
		}
	}

	printf("\n���������\n\n");
}

void Initialization(vector<PCB> v)
{
	int name;
	int time;
	int priority;

	printf("\n����ģ�⡪�����ȵ����㷨\n");
	for(int i=0;i<5;i++)
	{
		PCB P;
		printf("\nPCB %d \n\n",i+1);
		printf("�������������");
		scanf("%d",&name);
		printf("����������ʱ�䣺");
		scanf("%d",&time);
		printf("���������ȼ�����");
		scanf("%d",&priority);

		P.setName(name);
		P.setTime(time);
		P.setPriority(priority);
		P.setState('R');

		v.push_back(P);
	}
	run(v);
}

int main()
{
	vector<PCB> v;
	Initialization(v);

	return 0;
}
