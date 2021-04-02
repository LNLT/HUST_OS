#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
    long int back=syscall(333,"test.c","out.c");
    printf("%ld\n",back);
    return 0;
}
