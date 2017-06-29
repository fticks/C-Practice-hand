
#include <pthread.h>
#include <time.h>

#include "apue.h"

int
main(int argc, char *argv[])
{
    int err;
    struct timespec tout;
    struct tm *tmp;
    char buf[64];
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock);
    printf("mutex is locked\n");
    //time format change
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("current time is %s\n", buf);

    tout.tv_sec += 2;	/* 10 seconds from now */
    /* caution: this could lead to deadlock */
    err = pthread_mutex_timedlock(&lock, &tout);
    
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);//format date and time
    printf("the time is now %s\n", buf);
    
    if (err == 0)
    	printf("mutex locked again!\n");
    else
    	printf("can't lock mutex again: %s\n", strerror(err));
    
    exit(0);
}
