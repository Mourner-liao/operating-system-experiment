#include<string.h>
#include<ctype.h>
#include<malloc.h>
#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<io.h>
#include<math.h>
#include<process.h>
#include <iostream>
#include <time.h>

#define BUF 10  // ����Ĵ�С
#define MAX 20  // ������������ַ�

using namespace std;

typedef struct Pcb  // ���ݽṹ�Ķ����ȫ�ֱ���
{
    char name[10];  // ������
    char state[10];     // ����״̬
    char reason[10];     // ����������ԭ��
    int breakp;     // �ϵ㱣��
    struct Pcb *next;   // ����ʱ��˳��
} Pcb,*link;

int s1,s2;  // �ź���
    link producer;    // �����߽���
    link consumer;    // �����߽���
char str[MAX];  // ������ַ���
char buffer[BUF];   // �����
int len;    // ���볤��
int sp=0;   //string ��ָ��
int in=0;   // ������ָ��
int out=0;  // ������ָ��
char temp;  // ����ӡ����ʱ��Ʒ
char rec_p[MAX];    // ������¼
int rproducer=0;  // ������¼ָ��
char rec_c[MAX];    // ���Ѽ�¼
int rp2=0;  // ���Ѽ�¼ָ��
    link ready;     // ��������
    link b_s1;  //s1 ��������
    link b_s2;  //s2 ��������
int pc;  // ���������
int count;  // �ַ�������
int con_cnt;    // ���Ѽ�����
int i=3;


void Init();    // ��ʼ��
void P(int s);  //P ����
void V(int s);  //V ����
void Block(int s);  // ��������
void Wakeup(int s); // ���Ѻ���
void Control();     // ���������
void Processor();   // �����ִ��
void Print();   // ��ӡ����

void Init() // ��ʼ��
{
    s1=BUF;
    s2=0;

    producer=(link)malloc(sizeof(Pcb));   // �����µĽ�� , ����ʼ��Ϊ������
    strcpy(producer->name,"Producer");
    strcpy(producer->state,"Ready");
    strcpy(producer->reason,"Null");
    producer->breakp=0;
    producer->next=NULL;

    consumer=(link)malloc(sizeof(Pcb));   // �����µĽ�� , ����ʼ��Ϊ������
    strcpy(consumer->name,"Consumer");
    strcpy(consumer->state,"Ready");
    strcpy(consumer->reason,"Null");
    consumer->breakp=0;
    consumer->next=NULL;

    ready=producer;
    ready->next=consumer; // ��ʼ��Ϊ����������ǰ�����ѽ����ں�
    consumer->next=NULL;
    b_s1=NULL;
    b_s2=NULL;  // ��������Ϊ NULL
    pc=0;
    con_cnt=0;  // ���Ѽ�����
}

void P(int s)
{
    if(s==1)    //p(s1)
    {
        s1--;
        if(s1<0)
            Block(1);   // ������ǰ��������
        else
        {
            printf("\ts1 �ź�����ɹ���\n");
            ready->breakp=pc;   // ����ϵ�
        }
    }
    else    //p(s2)
    {
        s2--;
        if(s2<0)
            Block(2);   // ������ǰ���ѽ���
        else
        {
            printf("\ts2 �ź�����ɹ���\n");
            ready->breakp=pc;   // ����ϵ�
        }
    }
}

void V(int s)
{
    if(s==1)    //v(s1)
    {
        s1++;
        if(s1<=0)
            Wakeup(1);  // ������������
        ready->breakp=pc;   // ����ϵ�
    }
    else    //v(s2)
    {
        s2++;
        if(s2<=0)
            Wakeup(2);  // �������ѽ���
        ready->breakp=pc;   // ����ϵ�
    }
}

void Block(int s)   // ���������Ķ���
{
    link p;
    int num1=0;
    int num2=0;

    if(s==1)    // ��������
    {
        strcpy(producer->state,"Block");  // �ı�״̬
        strcpy(producer->reason,"S1");    // ˵��ԭ��
        p=b_s1;
        while(p)
        {
            num1++;
            p=p->next;  //p ��ֵΪ NULL, ��ʾ��β
        }

        if(!b_s1)
            b_s1=producer;
        else
            p=producer;

        producer->next=NULL;
        printf("\tProducer �������������ˣ�\n");
        ready->breakp=pc;   // ����ϵ�
        ready=ready->next;  // �ھ���������ȥ�� , ָ����һ��
        num1++;
    }
    else    // ���ѽ���
    {
        strcpy(consumer->state,"Block");
        strcpy(consumer->reason,"S2");
        p=b_s2;
        while(p)
        {
            num2++;
            p=p->next;  //p ��ֵΪ NULL, ��ʾ��β
        }

        if(!b_s2)
            b_s2=consumer;
        else
            p=consumer;

        ready->breakp=pc;   // ����ϵ�
        ready=ready->next;  // �ھ���������ȥ�� , ָ����һ��
        consumer->next=NULL;
        printf("\tConsumer ���ѽ���������!\n");
        num2++;
    }

    printf("\t�������������̸���Ϊ��%d\n",num1);
    printf("\t���������ѽ��̸���Ϊ��%d\n",num2);
}

void Wakeup(int s)  // ���Ѻ����Ķ���
{
    link p;
    link q=ready;

    if(s==1)    // ���� b_s1 ���׽��� , �������̶���
    {
        p=b_s1;
        b_s1=b_s1->next;    // ����ָ��ָ����һ����������
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");

        while(q)    // �����������
            q=q->next;
        q=p;
        p->next=NULL;
        printf("\tproducer �������̻����ˣ�\n");
    }
    else    // ���� b_s2 ���׽��� , ���ѽ��̶���
    {
        p=b_s2;
        b_s2=b_s2->next;    // ����ָ��ָ����һ����������
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");

        while(q->next)  // �����������
            q=q->next;

        q->next=p;
        p->next=NULL;
        printf("\tconsumer ���ѽ��̻����ˣ�\n");
    }
}

void Control()  // ���������ȳ���
{
    int rd;
    int num=0;
    link p=ready;

    if(ready==NULL) // ���޾������� , ����
        return;
    while(p)    // ͳ�ƾ������̸���
    {
        num++;
        p=p->next;  // ���� p ��Ϊ NULL
    }

    printf("\t�������̸���Ϊ��%d\n",num);

    time_t t;
    srand((unsigned) time(&t));
    rd=rand()%num;  // ����������������

    if(rd==1)
    {
        p=ready;
        ready=ready->next;
        ready->next=p;
        p->next=NULL;
        strcpy(ready->state,"Run");
        strcpy(ready->next->state,"Ready");
    }
    else
        strcpy(ready->state,"Run");

    pc=ready->breakp;
}

void Processor()    // ģ�⴦����ָ��ִ��
{
    if(strcmp(ready->name,"Producer")==0)   // ��ǰ����Ϊ������
        switch(pc)
        {
            case 0: //produce
                    printf("\t�������������ַ� %c\n",str[sp]);
                    rec_p[rproducer]=str[sp]; // ��ӵ�������¼
                    sp=(sp+1)%len;
                    pc++;
                    ready->breakp=pc;   // ����ϵ�
                    break;
            case 1: //p(s1)
                    pc++;
                    P(1);
                    break;
            case 2: //put
                    buffer[in]=rec_p[rproducer];  // �ŵ�������
                    printf("\t�ַ� %c �ɹ����뻺������\n",buffer[in]);
                    rproducer++;
                    in=(in+1)%BUF;
                    pc++;
                    ready->breakp=pc;   // ����ϵ�
                    break;
            case 3: //v(s2)
                    pc++;
                    printf("\t�ͷ�һ�� s2 �ź� \n");
                    V(2);
                    break;
            case 4://goto 0
                    printf("\t�������� Produce ���� \n");
                    pc=0;
                    count--;    // ʣ���ַ������� 1
                    printf("\tʣ���ַ� count=%d �� \n",count);
                    ready->breakp=pc;   // ����ϵ�

                    if(count<=0)    // ��������
                    {
                        printf("\t�����߽���������\n");
                        strcpy(producer->state,"Stop");
                        strcpy(producer->reason,"Null");
                        ready->breakp=-1;
                        ready=ready->next;  // �ھ���������ȥ��
                    }
        }
    else    // ��ǰ����Ϊ������
        switch(pc)
        {
            case 0: //p(s2)
                    pc++;
                    P(2);
                    break;
            case 1: //get
                    printf("\t������ȡ�ַ���\n");
                    temp=buffer[out];
                    out=(out+1)%BUF;
                    pc++;
                    ready->breakp=pc; // ����ϵ�
                    break;
            case 2: //v(s1)
                    pc++;
                    printf("\t�ͷ�һ�� s1\n");
                    V(1);
                    break;
            case 3: //consume
                    printf("\t�������ַ� %c\n",temp);
                    rec_c[rp2]=temp;    // ��ӵ����Ѽ�¼
                    rp2++;
                    con_cnt++;

                    if(con_cnt>=len)
                    {
                        strcpy(consumer->state,"Stop");   // ���̬
                        consumer->breakp=-1;
                        return;
                    }

                    pc++;
                    ready->breakp=pc;   // ����ϵ�
                    break;
            case 4: //goto 0
                    printf("\t���ѽ��� goto Pԭ�� ���� \n");
                    pc=0;
                    ready->breakp=pc;   // ����ϵ�
        }
}

void Print()
{
    int j;

    printf("\n------------- ������������ģ�� ------------- \n\n");
    printf("\tģ����̵��ַ���Ϊ��");
    printf("%s\n",&str);
    printf("\t��������");

    for(j=0;j<=rproducer;j++)
        printf("%c",rec_p[j]);

    printf("\n\t��������");

    for(j=rp2;j<=rproducer;j++)
        printf("%c",buffer[j]);

    printf("\n\t�����ѣ�");

    for(j=0;j<=rp2;j++)
        printf("%c",rec_c[j]);

    printf("\n\n------------- ���̿��ƿ����Ϣ ------------- \n\n");
    printf(" ������\t\t״̬ \t�ȴ�ԭ��\t�ϵ�\n\n");
    printf(" %s\t%s\t%s\t\t%d\n\n",producer->name,producer->state,producer->reason,producer->breakp);
    printf(" %s\t%s\t%s\t\t%d\n",consumer->name,consumer->state,consumer->reason,consumer->breakp);

    if(i==0)
    {
        printf("\n--------------------------------------------\n");
        printf(" �������˳���\n");
        printf("--------------------------------------------\n\n");
        exit(0);
    }

    else if(i==1||i==3)
    {
        printf("\n--------------------------------------------\n");
        printf(" 1. ����  \n 2. ��������  \n 0. �˳� \n");
        printf("--------------------------------------------\n");
        printf("\n�����룺");
        scanf("%d",&i);

    }
    else if(i==2)
        i=4;
}

int main()
{
    printf("\n--------------------------------------------\n");
    printf("\t����ͬ��ģ�⡪�������������� \n");
    printf("--------------------------------------------\n");
    printf("\n��������Ҫ�������ַ�����");
    scanf("%s",str);    //string �����Ž�Ҫ�������ַ�
    len=strlen(str);
    count=len;  // �����ַ��ĸ���
    Init(); // ��ʼ��

    while(con_cnt<len)  // ���������е��ַ�Ϊ����
    {
        system("cls");  // ��������
        printf("\n--------------------------------------------\n");
        printf("\t����ͬ��ģ�⡪�������������� \n");
        printf("--------------------------------------------\n");
        printf("\n--------------- ģ��ָ������ --------------- \n\n");
        Control();  // ���������ȳ���
        Processor();    // ģ�⴦����ָ��ִ��
        Print();    // �����ʾ������Ϣ
    }

    printf("\n--------------------------------------------\n");
    printf(" ������� !\n");
    printf("--------------------------------------------\n\n");
}
