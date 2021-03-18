#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAX 10
#define size 1024
#define shmsize 10240
#define SHMKEY 10
#define LENKEY 11
#define SEMKEY 21
pid_t p1, p2, p3;
int semid;
int shmid1;
int shmid2;
int lenid;
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
int main(){
    shmctl(shmid1,IPC_RMID,0);
    shmctl(shmid2,IPC_RMID,0);
    shmctl(lenid,IPC_RMID,0);
    shmid1=shmget(SHMKEY,shmsize,IPC_CREAT|0666);
    if (shmid1 == -1)
    {
        printf("共享内存1创建失败！\n");
        exit(1);
    }
    shmid2=shmget(SHMKEY+MAX,shmsize,IPC_CREAT|0666);
    if (shmid2 == -1)
    {
        printf("共享内存2创建失败！\n");
        exit(1);
    }
    lenid=shmget(LENKEY,8,IPC_CREAT|0666);
    if (lenid == -1)
    {
        printf("长度共享内存创建失败！\n");
        exit(1);
    }

    
    semid = semget(SEMKEY, 4, IPC_CREAT|0666);
    if (semid == -1)
    {
        printf("信号灯集创建失败！\n");
        exit(1);
    }
    union semun empty1,full1,empty2,full2;
    empty1.val=MAX;
    full1.val=0;
    empty2.val=MAX;
    full2.val=0;
    if(semctl(semid,0,SETVAL,empty1)==-1){
        printf("信号灯0初始化失败！\n");
        exit(1);
    }
    if(semctl(semid,1,SETVAL,full1)==-1){
        printf("信号灯1初始化失败！\n");
        exit(1);
    }
    if(semctl(semid,2,SETVAL,empty2)==-1){
        printf("信号灯2初始化失败！\n");
        exit(1);
    }
    if(semctl(semid,3,SETVAL,full2)==-1){
        printf("信号灯3初始化失败！\n");
        exit(1);
    }


    if ((p1= fork())== 0)
    { 
        printf("get created\n");
        execv("./get",NULL);
    }
    else{
        if ((p2 = fork()) == 0)
        {
            printf("copy created\n");
            execv("./copy",NULL);
        }
        else{
            if((p3 = fork()) == 0){
                printf("put created\n");
                execv("./put",NULL);
            }
            else{
                wait(NULL);
                wait(NULL);
                wait(NULL);
                semctl(semid,0,IPC_RMID,NULL);
                shmctl(shmid1,IPC_RMID,0);
                shmctl(shmid2,IPC_RMID,0);
                shmctl(lenid,IPC_RMID,0);
                exit(0);
            }
        }
    }
}