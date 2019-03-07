#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>

int sockfd, ttl;

void changeTTL(int newValue){
	ttl = newValue;
	int ret = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	printf("setsockopt = %d\n", ret);
}

int main(int argc, char **argv){
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	printf("sockfd = %d\n", sockfd);
	changeTTL(60);
	exit(0);
}
