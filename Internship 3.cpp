#include<iostream>

#define MAX_SIZE 8

using namespace std;

void MainPage();
void InitBitMap();
void Apply();
void Recovery();
void Print();

int Map[MAX_SIZE][MAX_SIZE];
int freeNum;	//���п���

void MainPage()
{
	int n;

	printf("\n**************************************\n");
	printf("\nλʾͼ������̴洢�ռ䣺\n");
	printf("\n  1. �ڴ�ռ����� ");
	printf("\n  2. �ڴ�ռ��ͷ� ");
	printf("\n  3. ��ʾ��ǰ���ڴ�ռ������� ");
	printf("\n  4. �˳� \n");
	printf("\n������������еĲ�����");
	scanf("%d",&n);
	while(n<1||n>4)
	{
		printf("\n�����������������룺");
		scanf("%d",&n);
	}

	switch(n)
	{
		case 1: Apply();	break;
		case 2: Recovery(); break;
		case 3: Print();	break;
		case 4: printf("\n���˳��� \n");
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
	int cylinder, track, record; // ����ţ��ŵ��ţ������¼��

	printf("\n������������ڴ�ռ��С��");
	scanf("%d",&n);


	if(freeNum>=n)
	{
		printf("\n����ɹ���\n");
		printf("\n����Ĵ��̿ռ�������ַΪ��\n");
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
					printf("\n  ����ţ�%d ,�ŵ��ţ�%d ,�����¼�ţ�%d ",cylinder+1,track+1,record+1);
				}
				if(n==0)
					break;
			}
		}
	}
	else
	{
		printf("\n�ڴ�ռ䲻�㣬����ʧ�ܣ�\n");
		MainPage();
	}

	printf("\n\n��ǰ���ڴ�ռ���������\n");
	Print();
}

void Recovery()
{
	int i,j;
	int cylinder, track, record;

	printf("\n����������š��ŵ��š������¼�ţ��ÿո�ָ�����");
	scanf("%d %d %d",&cylinder,&track,&record);

	i=cylinder-1;
	j=(track-1)*4+(record-1);

	if(cylinder>MAX_SIZE || track>4 || record>4)
	{
		printf("\n�������ַ�����ڣ�\n");
		MainPage();
	}
	else if(Map[i][j]==0)
	{
		printf("\n���ڴ�����ǿ���״̬��\n");
		MainPage();
	}
	else
	{
		Map[i][j]=0;
		freeNum++;
		printf("\n�ڴ��ͷųɹ���\n");
		printf("\n  �黹����ֽںţ�%d ,λ����%d \n",i+1,j+1);
	}

	printf("\n��ǰ���ڴ�ռ���������\n");
	Print();
}

void Print()
{
	printf("\n��ǰ���ô��̿ռ�Ϊ��%d \n",freeNum);
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
