#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(){
    pid_t p1,p2,p3;
    if((p1=fork())==0){
        printf("p1 go\n");
        execv("./window1",NULL);
    }
    else {
        if((p2=fork())==0){
            printf("p2 go\n");
            execv("./window2",NULL);
        }
        else {
            if((p3=fork())==0){
                printf("p3 go\n");
                execv("./window3",NULL);
            }
            else{
                wait(NULL);
                wait(NULL);
                wait(NULL);
                exit(0);
            }
        }
    } 
}