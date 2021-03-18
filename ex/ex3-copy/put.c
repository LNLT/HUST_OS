#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX 10
#define size 1024
#define shmsize 10240
#define SHMKEY 10
#define LENKEY 11
#define SEMKEY 21
int semid;
int shmid;
int lenid;
char *buf;
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
    printf("put run\n");
    char name[20];
    scanf("%s",name);
    shmid=shmget(SHMKEY+MAX,size,IPC_CREAT|0666);
    if (shmid == -1)
    {
        printf("共享内存获取失败！\n");
        exit(1);
    }
    buf=shmat(shmid,0,0);
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

    FILE *pfile=fopen(name,"wb");
    if(pfile==NULL)
        printf("打开写入文件失败\n");
    int k=0;
    while(1){
        P(semid,3);
        if(k==*(len+4)-1)
        {   
            fwrite((buf+size*(k%MAX)),1,*len,pfile);
            //printf("write %d %d\n",k,*len);
        }
        else
        {  
            fwrite((buf+size*(k%MAX)),1,size,pfile);
        }
        k++;
        V(semid,2);
        if(k==*(len+4)){
            printf("put:i=%d last_size=%d\n",k,*len);
            break;
        }
    }

    exit(0);
}