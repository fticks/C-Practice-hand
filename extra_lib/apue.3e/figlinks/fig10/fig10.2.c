#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "apue.h"

static void	sig_usr(int);	/* one handler for both signals */

int
main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR2");
	for ( ; ; )
		pause();
}

static void
sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		err_dump("received signal %d\n", signo);
}

/*sample run and result
h265@H265:fig10$ ./fig10.2 &
[1] 17634
h265@H265:fig10$ kill -USR1 17634
received SIGUSR1
h265@H265:fig10$ kill -USR2 17634
received SIGUSR2
h265@H265:fig10$ kill 17634
[1]+  Terminated              ./fig10.2
*/
