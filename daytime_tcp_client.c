#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* daytime server ip = 24.56.178.140 */

#define DEBUG
#define MAXLINE 256

#ifdef DEBUG
#define D(fmt, ...) do {\
			printf(fmt" func:%s line:%d err:%s\n",\
				##__VA_ARGS__, __func__, __LINE__,\
				strerror(errno));\
			exit(EXIT_FAILURE);\
		} while(0)
#else
#define D()
#endif

int main(int argc, char *argv[])
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in server_addr;
	socklen_t len = sizeof(server_addr);
	int reuse = 1;

	if (argc != 2)
		D("%s <server ip address>", argv[0]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		D("socket");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		D("setsockopt");

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(13);
	if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
		D("ip %s", argv[1]);

	if (connect(sockfd, (struct sockaddr *)&server_addr, len) < 0)
		D("connect");

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF)
			D("fputs");
	}
	if (n < 0)
		D("read error");

	return 0;
}
