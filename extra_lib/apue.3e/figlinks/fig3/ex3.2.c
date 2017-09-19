#include <stdio.h>
#include <fcntl.h>//open'param-oflag
#include <unistd.h>//close lseek dup/dup2
#include "apue.h"

int
main(void)
{
    int fd1 = open("Makefile", O_RDONLY);
    int fd2 = dup(fd1);
    int fd3 = dup2(fd1, 30);

    printf("%d\n", fd1);
    printf("%d\n", fd2);
    printf("%d\n", fd3);

    exit(0);
}
