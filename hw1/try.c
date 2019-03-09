#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "icmp.h"

int sockfd, ttl;
pthread_t child_id;
struct timeval tv[3][100];

long get_msec_diff(struct timeval *t1, struct timeval *t2)
  return (t2->tv_sec - t1->tv_sec)*1000 + (t2->tv_usec - t1->tv_usec)/1000;

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
  // receive buffer
  char buff[1000];
  int segment_size;
  struct sockaddr_in tmp_addr;
  socklen_t tmp_size = sizeof(tmp_addr);
  Info *info;
  struct timeval *tmp_tv;
  long msec_diff;
  int end_count = 0;
  
  for(int i=0;i<1000;++i){
    if(i % 100 == 0) printf("%d\n", i);
    // receive package
    segment_size = recvfrom(sockfd, buff, 1000, 0, (struct sockaddr *)&tmp_addr, &tmp_size);
    // get current time
    gettimeofday(tmp_tv, NULL);
    
    if(segment_size > 0){
      // unpack message
      info = get_info(buff);
      if(info->type == 3){
	// Destination unreachable
	end_count++;
	msec_diff = get_msec_diff(&tv[info->identifier][info->seqNum], &tmp_tv);
	
  }
  /*------------------------------------------------------*/
  
  exit(0);
}
