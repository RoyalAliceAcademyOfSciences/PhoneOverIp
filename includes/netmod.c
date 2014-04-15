/*
 * netmod.c
 *
 *  Created on: Oct 10, 2013
 *      Author: ml
 */

#include "netmod.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


int sockfd;
struct sockaddr_in cliaddr;

int netmod_init(unsigned short port)
{
	bzero(&cliaddr, sizeof(cliaddr));

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
}

int netmod_recv()
{
	int n;
	socklen_t len;
	char mesg[1000];
	bzero(&cliaddr, sizeof(cliaddr));

	len = sizeof(cliaddr);
	n = recvfrom(sockfd, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
	printf("-------------------------------------------------------\n");
	mesg[n] = 0;
	printf("Received the following:\n");
	printf("%s", mesg);
	printf("-------------------------------------------------------\n");

	return n;
}

int netmod_send(char* buff, unsigned short len)
{
	if(cliaddr.sin_addr.s_addr == 0)
		return 0;
	printf("send audio to network\n");
	return sendto(sockfd, buff, len, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
