#include	"unp.h"

static int doit(int, const char *);

int
main(int argc, char **argv)
{
	int		tcpsock, udpsock;
	struct sockaddr_in	servaddr;

	if ( (tcpsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("TCP socket error");

#ifdef notdef
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(9);
	if (ascii2addr(AF_INET, "127.0.0.1", &servaddr.sin_addr) != 4)
		err_quit("ascii2addr error");

	if (connect(tcpsock, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
#endif

	doit(tcpsock, "tcp");

	if ( (udpsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		err_sys("UDP socket error");

	doit(udpsock, "udp");
	exit(0);
}

static int
doit(int fd, const char *name)
{
	int			val;
	socklen_t	optlen;

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &optlen) < 0)
		err_sys("SO_SNDBUF getsockopt error");
	printf("%s send buffer size = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &val, &optlen) < 0)
		err_sys("SO_RCVBUF getsockopt error");
	printf("%s recv buffer size = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_SNDLOWAT, &val, &optlen) < 0)
		err_sys("SO_SNDLOWAT getsockopt error");
	printf("%s send low-water mark = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_RCVLOWAT, &val, &optlen) < 0)
		err_sys("SO_RCVLOWAT getsockopt error");
	printf("%s receive low-water mark size = %d\n", name, val);

	return 1;
}
