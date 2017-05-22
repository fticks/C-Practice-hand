/*
 alarm()
*/

#include "unp.h"

#include <stdio.h>
#include <unistd.h>//pause() alarm()
#include <signal.h>
#include <stdlib.h>

int u32time = 0;

void
sigalrm_handler(int signo)
{
    u32time ++;
    printf("alarm! --- %d\n", u32time);
    
    alarm(1);//set again SIGALRM signal
}

int
main(int argc, char **argv)
{
    signal(SIGALRM, sigalrm_handler);
    alarm(1);//start signal
    
    while(1) {
        pause();
    }

    exit(0);
}
