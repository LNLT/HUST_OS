#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SIZE 1024

int main(int argc, char **argv)
{
    int fd_in, fd_out;
    char s[SIZE];
    if (argc != 3)
    {
        perror("please input correctly!\n");
        exit(-1);
    }
    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1)
    {
        perror("the file can't find!\n");
        exit(-1);
    }

    fd_out = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (fd_out == -1)
    {
        perror("creat file failed!\n");
        exit(-1);
    }

    while (1)
    {
        int size = 0;
        int size_write=0;
        size = read(fd_in, s, SIZE);
        if (size == 0)
        {
            break;
        }
        else if (size < 0)
        {
            perror("read failed!\n");
            exit(-1);
        }
        else
        {
            size_write=write(fd_out, s, size);
            if (size_write == 0)
            {
                break;
            }
            else if (size_write < 0)
            {
                perror("write failed!\n");
                exit(-1);
            }
        }
    }
    close(fd_in);
    close(fd_out);
}