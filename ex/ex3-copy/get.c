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
    printf("get run\n");
    char name[20];
    scanf("%s",name);
    shmid=shmget(SHMKEY,shmsize,IPC_CREAT|0666);
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

    
    FILE *pfile=fopen(name,"rb");
    if(pfile==NULL)
        printf("打开读写文件失败\n");
    fseek(pfile,0L,SEEK_END);
    int flen=ftell(pfile);
    printf("flen=%d\n",flen);
    fseek(pfile,0L,SEEK_SET);

    int i=0;
    while(1){
        P(semid,0);
        *len=fread((buf+size*(i%MAX)),1,size,pfile);
        if(i==0){
            if(flen%size==0){
                *(len+4)=flen/size;
            }
            if(flen%size!=0){
                *(len+4)=flen/size+1;
            }
        }
        i++;
        if(flen%size==0&&i==(flen/size)){
            *(len+4)=i;
            printf("get:i=%d\n",i);
            printf("len=%d\n",*len);
            printf("i=%d\n",*(len+4));
            V(semid,1);
            break;
        }
        if(flen%size!=0&&i==(flen/size+1)){
            *(len+4)=i;
            printf("get:i=%d last_size=%d\n",i,*len);
            V(semid,1);
            break;
        }
        V(semid,1);
    }
    
    exit(0);
}