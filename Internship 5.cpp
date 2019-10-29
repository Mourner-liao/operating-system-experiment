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

#define BUF 10  // 缓存的大小
#define MAX 20  // 最大可以输入的字符

using namespace std;

typedef struct Pcb  // 数据结构的定义和全局变量
{
    char name[10];  // 进程名
    char state[10];     // 运行状态
    char reason[10];     // 若阻塞，其原因
    int breakp;     // 断点保护
    struct Pcb *next;   // 阻塞时的顺序
} Pcb,*link;

int s1,s2;  // 信号量
    link producer;    // 生产者进程
    link consumer;    // 消费者进程
char str[MAX];  // 输入的字符串
char buffer[BUF];   // 缓冲池
int len;    // 输入长度
int sp=0;   //string 的指针
int in=0;   // 生产者指针
int out=0;  // 消费者指针
char temp;  // 供打印的临时产品
char rec_p[MAX];    // 生产记录
int rproducer=0;  // 生产记录指针
char rec_c[MAX];    // 消费记录
int rp2=0;  // 消费记录指针
    link ready;     // 就绪队列
    link b_s1;  //s1 阻塞队列
    link b_s2;  //s2 阻塞队列
int pc;  // 程序计数器
int count;  // 字符计数器
int con_cnt;    // 消费计数器
int i=3;


void Init();    // 初始化
void P(int s);  //P 操作
void V(int s);  //V 操作
void Block(int s);  // 阻塞函数
void Wakeup(int s); // 唤醒函数
void Control();     // 处理机调度
void Processor();   // 处理机执行
void Print();   // 打印函数

void Init() // 初始化
{
    s1=BUF;
    s2=0;

    producer=(link)malloc(sizeof(Pcb));   // 建立新的结点 , 并初始化为生产者
    strcpy(producer->name,"Producer");
    strcpy(producer->state,"Ready");
    strcpy(producer->reason,"Null");
    producer->breakp=0;
    producer->next=NULL;

    consumer=(link)malloc(sizeof(Pcb));   // 建立新的结点 , 并初始化为消费者
    strcpy(consumer->name,"Consumer");
    strcpy(consumer->state,"Ready");
    strcpy(consumer->reason,"Null");
    consumer->breakp=0;
    consumer->next=NULL;

    ready=producer;
    ready->next=consumer; // 初始化为生产进程在前，消费进程在后
    consumer->next=NULL;
    b_s1=NULL;
    b_s2=NULL;  // 阻塞进程为 NULL
    pc=0;
    con_cnt=0;  // 消费计数器
}

void P(int s)
{
    if(s==1)    //p(s1)
    {
        s1--;
        if(s1<0)
            Block(1);   // 阻塞当前生产进程
        else
        {
            printf("\ts1 信号申请成功！\n");
            ready->breakp=pc;   // 保存断点
        }
    }
    else    //p(s2)
    {
        s2--;
        if(s2<0)
            Block(2);   // 阻塞当前消费进程
        else
        {
            printf("\ts2 信号申请成功！\n");
            ready->breakp=pc;   // 保存断点
        }
    }
}

void V(int s)
{
    if(s==1)    //v(s1)
    {
        s1++;
        if(s1<=0)
            Wakeup(1);  // 唤醒生产进程
        ready->breakp=pc;   // 保存断点
    }
    else    //v(s2)
    {
        s2++;
        if(s2<=0)
            Wakeup(2);  // 唤醒消费进程
        ready->breakp=pc;   // 保存断点
    }
}

void Block(int s)   // 阻塞函数的定义
{
    link p;
    int num1=0;
    int num2=0;

    if(s==1)    // 生产进程
    {
        strcpy(producer->state,"Block");  // 改变状态
        strcpy(producer->reason,"S1");    // 说明原因
        p=b_s1;
        while(p)
        {
            num1++;
            p=p->next;  //p 的值为 NULL, 表示队尾
        }

        if(!b_s1)
            b_s1=producer;
        else
            p=producer;

        producer->next=NULL;
        printf("\tProducer 生产进程阻塞了！\n");
        ready->breakp=pc;   // 保存断点
        ready=ready->next;  // 在就绪队列中去掉 , 指向下一个
        num1++;
    }
    else    // 消费进程
    {
        strcpy(consumer->state,"Block");
        strcpy(consumer->reason,"S2");
        p=b_s2;
        while(p)
        {
            num2++;
            p=p->next;  //p 的值为 NULL, 表示队尾
        }

        if(!b_s2)
            b_s2=consumer;
        else
            p=consumer;

        ready->breakp=pc;   // 保存断点
        ready=ready->next;  // 在就绪队列中去掉 , 指向下一个
        consumer->next=NULL;
        printf("\tConsumer 消费进程阻塞了!\n");
        num2++;
    }

    printf("\t阻塞的生产进程个数为：%d\n",num1);
    printf("\t阻塞的消费进程个数为：%d\n",num2);
}

void Wakeup(int s)  // 唤醒函数的定义
{
    link p;
    link q=ready;

    if(s==1)    // 唤醒 b_s1 队首进程 , 生产进程队列
    {
        p=b_s1;
        b_s1=b_s1->next;    // 阻塞指针指向下一个阻塞进程
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");

        while(q)    // 插入就绪队列
            q=q->next;
        q=p;
        p->next=NULL;
        printf("\tproducer 生产进程唤醒了！\n");
    }
    else    // 唤醒 b_s2 队首进程 , 消费进程队列
    {
        p=b_s2;
        b_s2=b_s2->next;    // 阻塞指针指向下一个阻塞进程
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");

        while(q->next)  // 插入就绪队列
            q=q->next;

        q->next=p;
        p->next=NULL;
        printf("\tconsumer 消费进程唤醒了！\n");
    }
}

void Control()  // 处理器调度程序
{
    int rd;
    int num=0;
    link p=ready;

    if(ready==NULL) // 若无就绪进程 , 结束
        return;
    while(p)    // 统计就绪进程个数
    {
        num++;
        p=p->next;  // 最终 p 变为 NULL
    }

    printf("\t就绪进程个数为：%d\n",num);

    time_t t;
    srand((unsigned) time(&t));
    rd=rand()%num;  // 随机函数产生随机数

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

void Processor()    // 模拟处理器指令执行
{
    if(strcmp(ready->name,"Producer")==0)   // 当前进程为生产者
        switch(pc)
        {
            case 0: //produce
                    printf("\t生产者生产了字符 %c\n",str[sp]);
                    rec_p[rproducer]=str[sp]; // 添加到生产记录
                    sp=(sp+1)%len;
                    pc++;
                    ready->breakp=pc;   // 保存断点
                    break;
            case 1: //p(s1)
                    pc++;
                    P(1);
                    break;
            case 2: //put
                    buffer[in]=rec_p[rproducer];  // 放到缓冲区
                    printf("\t字符 %c 成功放入缓冲区！\n",buffer[in]);
                    rproducer++;
                    in=(in+1)%BUF;
                    pc++;
                    ready->breakp=pc;   // 保存断点
                    break;
            case 3: //v(s2)
                    pc++;
                    printf("\t释放一个 s2 信号 \n");
                    V(2);
                    break;
            case 4://goto 0
                    printf("\t生产进程 Produce 操作 \n");
                    pc=0;
                    count--;    // 剩余字符个数减 1
                    printf("\t剩余字符 count=%d 个 \n",count);
                    ready->breakp=pc;   // 保存断点

                    if(count<=0)    // 生产结束
                    {
                        printf("\t生产者结束生产！\n");
                        strcpy(producer->state,"Stop");
                        strcpy(producer->reason,"Null");
                        ready->breakp=-1;
                        ready=ready->next;  // 在就绪队列中去掉
                    }
        }
    else    // 当前进程为消费者
        switch(pc)
        {
            case 0: //p(s2)
                    pc++;
                    P(2);
                    break;
            case 1: //get
                    printf("\t消费者取字符！\n");
                    temp=buffer[out];
                    out=(out+1)%BUF;
                    pc++;
                    ready->breakp=pc; // 保存断点
                    break;
            case 2: //v(s1)
                    pc++;
                    printf("\t释放一个 s1\n");
                    V(1);
                    break;
            case 3: //consume
                    printf("\t消费了字符 %c\n",temp);
                    rec_c[rp2]=temp;    // 添加到消费记录
                    rp2++;
                    con_cnt++;

                    if(con_cnt>=len)
                    {
                        strcpy(consumer->state,"Stop");   // 完成态
                        consumer->breakp=-1;
                        return;
                    }

                    pc++;
                    ready->breakp=pc;   // 保存断点
                    break;
            case 4: //goto 0
                    printf("\t消费进程 goto P原语 操作 \n");
                    pc=0;
                    ready->breakp=pc;   // 保存断点
        }
}

void Print()
{
    int j;

    printf("\n------------- 生产者消费者模拟 ------------- \n\n");
    printf("\t模拟过程的字符串为：");
    printf("%s\n",&str);
    printf("\t已生产：");

    for(j=0;j<=rproducer;j++)
        printf("%c",rec_p[j]);

    printf("\n\t缓存区：");

    for(j=rp2;j<=rproducer;j++)
        printf("%c",buffer[j]);

    printf("\n\t已消费：");

    for(j=0;j<=rp2;j++)
        printf("%c",rec_c[j]);

    printf("\n\n------------- 进程控制块的信息 ------------- \n\n");
    printf(" 进程名\t\t状态 \t等待原因\t断点\n\n");
    printf(" %s\t%s\t%s\t\t%d\n\n",producer->name,producer->state,producer->reason,producer->breakp);
    printf(" %s\t%s\t%s\t\t%d\n",consumer->name,consumer->state,consumer->reason,consumer->breakp);

    if(i==0)
    {
        printf("\n--------------------------------------------\n");
        printf(" 程序已退出！\n");
        printf("--------------------------------------------\n\n");
        exit(0);
    }

    else if(i==1||i==3)
    {
        printf("\n--------------------------------------------\n");
        printf(" 1. 继续  \n 2. 持续处理  \n 0. 退出 \n");
        printf("--------------------------------------------\n");
        printf("\n请输入：");
        scanf("%d",&i);

    }
    else if(i==2)
        i=4;
}

int main()
{
    printf("\n--------------------------------------------\n");
    printf("\t进程同步模拟——生产者消费者 \n");
    printf("--------------------------------------------\n");
    printf("\n请输入需要生产的字符串：");
    scanf("%s",str);    //string 数组存放将要产生的字符
    len=strlen(str);
    count=len;  // 输入字符的个数
    Init(); // 初始化

    while(con_cnt<len)  // 消费完所有的字符为结束
    {
        system("cls");  // 清屏操作
        printf("\n--------------------------------------------\n");
        printf("\t进程同步模拟——生产者消费者 \n");
        printf("--------------------------------------------\n");
        printf("\n--------------- 模拟指令流程 --------------- \n\n");
        Control();  // 处理器调度程序
        Processor();    // 模拟处理器指令执行
        Print();    // 输出显示各个信息
    }

    printf("\n--------------------------------------------\n");
    printf(" 程序结束 !\n");
    printf("--------------------------------------------\n\n");
}
