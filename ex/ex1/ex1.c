#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#define max 10
pid_t p1, p2;
int pipefd[2];//0读端，1写入端
void my_func_parent(int sig_no)
{
    kill(p1, SIGUSR1);
    kill(p2, SIGUSR1);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
    printf("Parent Process is Killed!\n");
    exit(1);

}
void my_func_child1(int sig_no)
{
    close(pipefd[0]);
    printf("Child Process 1 is Killed by Parent!\n");
    exit(1);
}
void my_func_child2(int sig_no)
{
    close(pipefd[0]);
    printf("Child Process 2 is Killed by Parent!\n");
    exit(1);
}

void main()
{
    char s[30];
    pipe(pipefd);

    signal(SIGINT, my_func_parent); //设置signal处理
    int count = 0;
    if ((p1= fork())== 0)
    { //子进程1
        signal(SIGINT, SIG_IGN);
        signal(SIGUSR1, my_func_child1);
        close(pipefd[1]);
        while (read(pipefd[0], s, sizeof(s))>0)
        {
            printf("%s\n", s);
            count++;
        }
        printf("子进程1接受到%d的信息！\n",count);
        close(pipefd[0]);
        exit(0);
    }
    else
    {
        if ((p2 = fork()) == 0)
        { //子进程2
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, my_func_child2);
            close(pipefd[1]);
            while (read(pipefd[0], s, sizeof(s))>0)//管道为空时且写端没有全部关闭则read阻塞，一直等待，无法返回；管道不为空时且写端全部关闭则读完管道内的数据后返回值为0（写端全部关闭，此时写文件描述符为0，返回0）
            {
                printf("%s\n", s);
                count++;
            }
            printf("子进程2接受到%d的信息！\n",count);
            close(pipefd[0]);
            exit(0);
        }
        else
        { //父进程
            int count_p = 1;
            close(pipefd[0]);
            while (count_p <= max)
            {
                sprintf(s, "I send you %d times.", count_p);
                write(pipefd[1], s, sizeof(s));
                count_p++;
                sleep(1);
            }
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
            printf("Parent Process is Killed!\n");
            exit(0);
        }
    }
}