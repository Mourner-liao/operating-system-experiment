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
    printf("\n\n进程名\t运行时间\t优先数\t状态\n");
    printf("*****************************************\n");

    for(unsigned int i = 0; i < v.size(); i++)
        printf("  %d\t    %d\t\t   %d\t  %c\t\n", v[i].returnName(), v[i].returnTime(), v[i].returnPriority(), v[i].returnState());

    printf("\n当前正在运行的进程为：%d\n",v.front().returnName());
	printf("运行前：运行时间：%d  优先数：%d  状态：%c",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
}

void run(vector<PCB> v)
{
	printf("\n进程优先调度算法\n");
	while(!v.empty())
	{
		sort(v.begin(),v.end(),cmp);
		print(v);

		v.front().changeTime();
		v.front().changePriority();

		if(v.front().returnTime()!=0)
		{
			printf("\n运行后：运行时间：%d  优先数：%d  状态：%c\n",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
			v.push_back(v.front());
			v.erase(v.begin(),v.begin()+1);
		}
		else
		{
			v.front().changeState('E');
			printf("\n运行后：运行时间：%d  优先数：%d  状态：%c\n",v.front().returnTime(),v.front().returnPriority(),v.front().returnState());
			v.erase(v.begin(),v.begin()+1);
		}
	}

	printf("\n程序结束。\n\n");
}

void Initialization(vector<PCB> v)
{
	int name;
	int time;
	int priority;

	printf("\n进程模拟——优先调度算法\n");
	for(int i=0;i<5;i++)
	{
		PCB P;
		printf("\nPCB %d \n\n",i+1);
		printf("请输入进程名：");
		scanf("%d",&name);
		printf("请输入运行时间：");
		scanf("%d",&time);
		printf("请输入优先级数：");
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
