#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
    long int back=syscall(334);
    printf("world=%ld\n",back);
    return 0;
}
