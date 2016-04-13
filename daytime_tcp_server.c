#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define DEBUG
#include "debug.h"

#define MAXLINE 256
#define LISTENQ 10

int main(int argc, char *argv[])
{
	int listenfd, connfd, n;
	struct sockaddr_in serv_addr;
	struct sockaddr_in peer_addr;
	char buff[MAXLINE + 1];
	socklen_t len = sizeof(struct sockaddr_in);
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		D("socket");

	bzero(&serv_addr, sizeof(serv_addr));
	bzero(&peer_addr, sizeof(peer_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(13);

	if (bind(listenfd, (struct sockaddr *)&serv_addr, len))
		D("bind");

	if (listen(listenfd, LISTENQ))
		D("listen");

	for ( ; ; ) {
		connfd = accept(listenfd, (struct sockaddr *)&peer_addr, &len);
		if (connfd < 0)
			D("accept");

		ticks = time(NULL);
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		n = write(connfd, buff, strlen(buff));
		if (n != strlen(buff))
			D("write");
		close(connfd);
	}
	close(listenfd);
	return 0;
}
