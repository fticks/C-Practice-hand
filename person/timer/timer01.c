/*
 sleep()
 usleep()
 select()
*/

#include "unp.h"

#include <sys/time.h>// timeval{} for selcet()
#include <sys/select.h>//select()
#include <unistd.h>//fd_set{} for selcet()

void
sleep_s(unsigned int nusecs)
{
    struct timeval  tval;

    tval.tv_sec = nusecs;
    tval.tv_usec = 0;
    select(0, NULL, NULL, NULL, &tval);
}

void
sleep_ms(unsigned int nusecs)
{
    struct timeval  tval;

    tval.tv_sec = nusecs / 1000;
    tval.tv_usec = (nusecs % 1000) * 1000;//carefully this mean is usec, but nusecs is ms
    select(0, NULL, NULL, NULL, &tval);
}

void
sleep_us(unsigned int nusecs)
{
    struct timeval  tval;

    tval.tv_sec = nusecs / 1000000;
    tval.tv_usec = nusecs % 1000000;
    select(0, NULL, NULL, NULL, &tval);
}

int
main(int argc, char **argv)
{
    int time = 0;
    err_msg("\nhello world!\n");
    while(1){
        time ++;
        //sleep(1);
        //usleep(1000*1000);
        //sleep_s(1);
        sleep_ms(1000);
        //sleep_us(1000*1000);
        err_msg("%d", time);
    }
    exit(0);
}
