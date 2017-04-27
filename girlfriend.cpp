//逻辑上有个没搞清楚的问题，当被唤醒的进程的优先权大于当前正在执行进程的优先权时应该怎么处理？
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int n;
typedef struct PCB
{
    char name[100];  //进程名
    int priority;    //优先级
    char state[20];  //状态
    int needtime;    //运行所需时间
    int cputime;     //时间片  初始值为0
    struct PCB* next;//指向下一个节点  初始值NULL
}PCB;
PCB* ready;   //就绪态链表
PCB* running; //运行态
PCB* blocked; //阻塞态链表

void creatjincheng()//创建进程
{
   PCB* newpcb=malloc(sizeof(PCB));
   newpcb->cputime=0;
   newpcb->next=NULL;
   printf("请输入进程名 进程优先级 需要时间（提示：以空格隔开）:");
   scanf("%s%d%d",newpcb->name,&newpcb->priority,&newpcb->needtime);
   PCB* head=running;//初始化， 指针指向就绪队列头部
   int cnt=3;
   while(cnt--){
    if(cnt==2) head=running;
    else if(cnt==1) head=ready;
    else if(cnt==0) head=blocked;
    while(head->next!=NULL){
        head=head->next;
        if(strcmp(head->name,newpcb->name)==0){
            printf("输入非法！进程名已存在！\n");
            return;
        }
    }
   }
   head=ready;
   while(1){
        if(head->next==NULL||(head->priority > newpcb->priority && head->next->priority < newpcb->priority))
        {
            strcpy(newpcb->state,"ready");
            if(head->next==NULL){
               head->next=newpcb;
            }
             else{
               PCB* nex=head->next;
               head->next=newpcb;
               newpcb->next=nex;
            }
            printf("创建成功！\n");
            break;
        }
        head=head->next;
   }

//   showLink();
}

void  diaodujincheng()//就绪态变为执行态
{

    if(running->next==NULL){
        if(ready->next==NULL){
            printf("当前无进程正在运行且就绪队列中无进程！\n");
            return;
        }
        PCB* toBeRun=ready->next;
        if(toBeRun->next!=NULL) ready->next=toBeRun->next;
        else ready->next=NULL;
        toBeRun->next=NULL;
        strcpy(toBeRun->state,"running");
        running->next=toBeRun;
    }
    running->next->cputime++;
    running->next->needtime--;
    if(running->next->needtime==0){
            free(running->next);
            running->next=NULL;
    }
    showLink();
    char next[100];
    printf("输入1查看下个时间片，其它数字返回主界面：");
    scanf("%s",next);
    if(strcmp(next,"1")==0) diaodujincheng();
    else return;
}
void  zusejincheng()//执行态变为阻塞态，
//插入到阻塞队列中
{
     printf("输入要阻塞的进程的进程名称：");
     char s[100];
     scanf("%s",s);
//     PCB*　temp=running;
     int flag=0;
     if(running->next!=NULL&&strcmp(s,running->next->name)==0){
        flag=1;
        PCB* toBeBlocked=running->next;
        running->next=NULL;
        PCB* bhead=blocked;
        while(bhead->next!=NULL){
            bhead=bhead->next;
        }
        strcpy(toBeBlocked->state,"blocked");
        toBeBlocked->next=NULL;
        bhead->next=toBeBlocked;
        printf("阻塞成功！\n");
        return;
     }
     if(flag==0) printf("当前无进程正在执行或未在执行的进程中找到该进程！\n");
     return;
//     scanf("%s",pcb[n].name);

}
void huanxingjincheng()//由阻塞态变为就绪态
{
    printf("请输入待唤醒进程名：");
    char s[100];
    scanf("%s",s);
    PCB* bhead=blocked;
    int flag=0;
    while(bhead->next!=NULL){
        bhead=bhead->next;
        if(strcmp(bhead->name,s)==0){
            flag=1;
            PCB* toBeWake=bhead;
            PCB* temp=blocked;
            PCB* rhead=ready;
            while(temp->next!=toBeWake){
                temp=temp->next;
            }
            if(toBeWake->next==NULL){
                temp->next=NULL;
            }
            else{
                temp->next=toBeWake->next;
            }

            toBeWake->next=NULL;
            while(1){
                if((rhead->next==NULL)||(rhead->priority > toBeWake->priority && rhead->next->priority < toBeWake->priority)){
                    strcpy(toBeWake->state,"ready");
                    if(rhead->next==NULL) rhead->next=toBeWake;
                    else{
                        PCB* nex=rhead->next;
                        rhead->next=toBeWake;
                        toBeWake->next=nex;
                    }
                    printf("唤醒成功！\n");
                    break;
                }
                rhead=rhead->next;
            }
            break;
        }
    }
    if(flag==0){
        printf("未在阻塞队列中找到该进程！\n");
    }
    return;
}
void chexiaojincheng()//删除进程
{
     printf("输入要撤销的进程的进程名称：\n");
     char s[100];
     scanf("%s",s);
     int cnt=3;
     int flag=0;
     PCB* temp=malloc(sizeof(PCB));
     PCB* head=malloc(sizeof(PCB));
     while(cnt--){
        if(flag) break;
        if(cnt==2) temp=running,head=running;
        else if(cnt==1) temp=ready,head=ready;
        else if(cnt==0) temp=blocked,head=blocked;
        while(temp->next!=NULL){
            temp=temp->next;
            if(strcmp(s,temp->name)==0){
                flag=1;
                if(temp==NULL){
                    while(head->next!=temp){
                        head=head->next;
                    }
                    head->next=NULL;
                    free(temp) ;
                }
                else{
                    while(head->next!=temp){
                        head=head->next;
                    }
                    head->next=temp->next;
                    free(temp) ;
                }
            printf("撤销进程成功！\n");
            break;
            }
        }
     }
     if(flag==0){
        printf("不存在该进程！\n");
     }

//     scanf("%s",pcb[n].name);
}
void showLink(){ //显示
    int flag=0;
    PCB* head=malloc(sizeof(PCB));
    int cnt=3;
    while(cnt--){
        if(cnt==2) head=running;
        else if(cnt==1) head=ready;
        else if(cnt==0) head=blocked;
        while(head->next!=NULL){
            head=head->next;
            if(flag==0){
                printf("进程名称\t进程优先权\t进程状态\t进程仍需时间\t进程已执行时间\t\n");
                flag=1;
            }
            printf("%s\t\t%d\t\t%s\t\t%d\t\t%d\t\t\n",head->name,head->priority,head->state,head->needtime,head->cputime);
        }
    }

    if(flag==0){
        printf("当前无进程！\n");
    }
}

int main()
{
    running=malloc(sizeof(PCB));
    running->priority=INT_MAX;
    running->next=NULL;
    ready=malloc(sizeof(PCB));
    ready->priority=INT_MAX;
    ready->next=NULL;
    blocked=malloc(sizeof(PCB));
    blocked->priority=INT_MAX;
    blocked->next=NULL;
    while(1)
    {
        printf("\t***************************\n");
        printf("\t* 1-------创建进程------- *\n");
        printf("\t* 2-------阻塞进程------- *\n");
        printf("\t* 3-------唤醒进程------- *\n");
        printf("\t* 4-------撤销进程------- *\n");
        printf("\t* 5-------调度进程------- *\n");
        printf("\t* 6-----显示当前进程----- *\n");
        printf("\t* 0---------退出--------- *\n");
        printf("\t***************************\n");
        scanf("%d",&n);
        if(n == 1)
           creatjincheng();
        else if(n == 2)
           zusejincheng();
        else if(n == 3)
           huanxingjincheng();
        else if(n == 4)
           chexiaojincheng();
        else if(n==5)
            diaodujincheng();
        else if(n==6)
            showLink();
        else break;
    }
}
