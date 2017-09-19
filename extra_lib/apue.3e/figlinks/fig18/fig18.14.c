#include "apue.h"

int
main(void)
{
    printf("fd 0: %s\n", isatty(0) ? "tty" : "not a tty");
    printf("fd 1: %s\n", isatty(1) ? "tty" : "not a tty");
    printf("fd 2: %s\n", isatty(2) ? "tty" : "not a tty");

    printf("fd 0 path: %s\n", ttyname(0));
    printf("fd 1 path: %s\n", ttyname(1));
    printf("fd 2 path: %s\n", ttyname(2));

    printf("Controlling terminal is %s\n", ctermid(NULL));

    exit(0);
}

/*
h265@H265:fig18$ ./fig18.14 < /etc/passwd 2>/dev/null 
fd 0: not a tty
fd 1: tty
fd 2: not a tty
fd 0 path: (null)
fd 1 path: /dev/pts/2
fd 2 path: (null)
Controlling terminal is /dev/tty
h265@H265:fig18$ ./fig18.14 
fd 0: tty
fd 1: tty
fd 2: tty
fd 0 path: /dev/pts/2
fd 1 path: /dev/pts/2
fd 2 path: /dev/pts/2
Controlling terminal is /dev/tty
*/
