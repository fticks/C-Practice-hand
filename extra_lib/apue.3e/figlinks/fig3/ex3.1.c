#include <stdio.h>
#include <fcntl.h>//open'param-oflag
#include <unistd.h>//close lseek
#include "apue.h"

//for check the linux which will be open stdout-1/stdin-0/sterr-2 three file descriptor for every process.
//and open function wiil return the min fd from process in the opertion system

int
main(void)
{
    int fd1 = open("Makefile", O_RDONLY);
    int fd2 = open("exectue.sh", O_RDONLY);

    printf("%d\n", fd1);
    printf("%d\n", fd2);

    int seek = lseek(fd1, 0, SEEK_CUR);
    printf("%d\n", seek);

    exit(0);
}
