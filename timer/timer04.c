/*
 timer_create()
*/

#include "unp.h"

int
main(int argc, char **argv)
{
    int time = 0;
    err_msg("\nhello world!\n");
    
    while(1){
        time ++;
        
        err_msg("%d", time);
    }
    exit(0);
}

