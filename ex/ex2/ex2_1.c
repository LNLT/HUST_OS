#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pthread.h>
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
int all;
int num=0;
int semid;
int sell[5];
void P(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}
void V(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
    return;
}
void *pthread1(void* id)
{
    long pthread= (long) id;
    while(1){
        P(semid, 0);
        if(num<all){
            num+=1;
            printf("线程%ld已售票数：%d\n",pthread,num);
            // if(num==all)
            //     printf("已售票数：%d\n",num);
        }
        else  exit(1);
        V(semid, 0);
    }
}
void main()
{
    int n=0;
    printf("飞机票总数：");
    scanf("%d",&all);
    printf("创建售票线程个数：");
    scanf("%d",&n);
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        printf("信号灯集创建失败！\n");
        exit(1);
    }
    union semun sem;
    sem.val = 1;
    if (semctl(semid, 0, SETVAL, sem) == -1)
    {
        printf("信号灯0初始化失败！\n");
        exit(1);
    }
    pthread_t p[n];
    for (int i = 1; i <= n; i++)
    {
        if (pthread_create(&p[i], NULL, pthread1, (void*) i))
        {
            printf("pthread%d创建失败！\n",i);
            exit(1);
        }
    }
    for (int k = 1; k <= n; k++)
    {
        if (pthread_join(p[k], NULL)==-1)
        {
            printf("pthread%d结束失败！\n",k);
            exit(1);
        }
    }
    semctl(semid, 0, IPC_RMID, NULL);
    exit(0);
}