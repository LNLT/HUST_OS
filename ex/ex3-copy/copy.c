#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define size 1024
#define shmsize 10240
#define SHMKEY 10
#define LENKEY 11
#define SEMKEY 21
int semid;
int shmid1;
int shmid2;
int lenid;
char *buf1;
char *buf2;
int *len;
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
int main(){
    printf("copy run\n");
    shmid1=shmget(SHMKEY,shmsize,IPC_CREAT|0666);
    if (shmid1 == -1)
    {
        printf("共享内存1获取失败！\n");
        exit(1);
    }
    buf1=shmat(shmid1,0,0);
    shmid2=shmget(SHMKEY+MAX,shmsize,IPC_CREAT|0666);
    if (shmid2 == -1)
    {
        printf("共享内存2获取失败！\n");
        exit(1);
    }
    buf2=shmat(shmid2,0,0);
    lenid=shmget(LENKEY,8,IPC_CREAT|0666);
    if (lenid == -1)
    {
        printf("长度共享内存获取失败！\n");
        exit(1);
    }
    len=shmat(lenid,0,0);


    semid = semget(SEMKEY, 4, IPC_CREAT|0666);
    if (semid == -1)
    {
        printf("信号灯集获取失败！\n");
        exit(1);
    }
    int i=0;
    while(1){
        P(semid,1);
        P(semid,2);
        memcpy(buf2+size*(i%MAX),buf1+size*(i%MAX),size);//错误点,需要一块一块复制,否则会出现覆盖问题
        i++;
        V(semid,3);
        V(semid,0);
        if(i==*(len+4)){
            printf("copy:i=%d\n",i);
            break;
        }
    }
    
    exit(0);
}