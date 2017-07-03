#include <stdio.h>
#include <fcntl.h>//open'param-oflag
#include <unistd.h>//close lseek
#include <fcntl.h>//fcntl
#include <stdlib.h>//atoi
#include "apue.h"

void
myset_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int		val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    	err_sys("fcntl F_GETFL error");

    val |= flags;		/* turn on flags */

    if (fcntl(fd, F_SETFL, val) < 0)
    	err_sys("fcntl F_SETFL error");
}

void
myclr_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int		val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    	err_sys("fcntl F_GETFL error");

    val &= ~flags;		/* turn on flags */

    if (fcntl(fd, F_SETFL, val) < 0)
    	err_sys("fcntl F_SETFL error");
}

int 
pri_fcntl(int fd, int argc, char *argv[]){
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    	err_sys("fcntl error for fd %d", fd);

    switch (val & O_ACCMODE) {
    case O_RDONLY:
    	printf("read only");
    	break;

    case O_WRONLY:
    	printf("write only");
    	break;

    case O_RDWR:
    	printf("read write");
    	break;

    default:
    	err_dump("unknown access mode");
    }

    if (val & O_APPEND)
    	printf(", append");
    if (val & O_NONBLOCK)
    	printf(", nonblocking");
    if (val & O_SYNC)
    	printf(", synchronous writes");

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC)
    	printf(", synchronous writes");
#endif

    putchar('\n');

    return 0;
}

char string[]={"123124124"};

int
main(int argc, char *argv[])
{
    int val;

    //if (argc != 2)
    //    err_quit("usage: a.out <descriptor#>");

    //int fd = open("new", O_RDONLY|O_CREAT);
    int fd = open(argv[1], O_RDONLY|O_CREAT);
    printf("%d\n",fd);

    myclr_fl(fd, O_RDONLY);

    //string[0] = '2';

    /*int ret = write(fd, string, sizeof(string));
    if(ret<0)
        err_sys("write err\n");*/

    pri_fcntl(fd, argc, argv);

    myset_fl(fd, O_SYNC);

    pri_fcntl(fd, argc, argv);

    exit(0);
}

