/*
 setitimer()
*/

#include "unp.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>//signal()
#include <time.h>//itimerval{}
#include <sys/time.h>//setitimer()

void 
sigroutine(int signo){
    switch (signo){
        case SIGALRM:
            printf("Catch a signal -- SIGALRM \n");
            //signal(SIGALRM, sigroutine);
            break;
        case SIGVTALRM:
            printf("Catch a signal -- SIGVTALRM \n");
            //signal(SIGVTALRM, sigroutine);
            break;
    }
    return;
}

int
main(int argc, char **argv)
{
    struct itimerval value, ovalue, value2;          
    
    printf("process id is %d\n", getpid());
    
    signal(SIGALRM, sigroutine);
    signal(SIGVTALRM, sigroutine);

    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);     //(1)

    value2.it_value.tv_sec = 0;
    value2.it_value.tv_usec = 500000;
    value2.it_interval.tv_sec = 0;
    value2.it_interval.tv_usec = 500000;
    setitimer(ITIMER_VIRTUAL, &value2, &ovalue);    //(2)

    while(1);
    
    exit(0);
}

