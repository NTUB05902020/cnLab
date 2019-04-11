#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <pthread.h>

int sockfd, ttl;
pthread_t child_id;

void changeTTL(int newValue){
	ttl = newValue;
	int ret = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	printf("setsockopt = %d\n", ret);
}

void *func(void *arg){
	/*--------------------start send---------------------*/
	printf("Here in child thread, %s\n", (char *)arg);
	while(1);
	/*--------------------------------------------------*/
}

int main(int argc, char **argv){
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	printf("sockfd = %d\n", sockfd);
	changeTTL(1);
	
	if(pthread_create(&child_id, NULL, func, "child thread") != 0){
		perror("pthread_create "); exit(1);
	}
	
	/*--------------------start receive---------------------*/
	for(int i=0;i<1000;++i){
		if(i % 100 == 0) printf("%d\n", i);
	}
	/*------------------------------------------------------*/
	
	exit(0);
}
