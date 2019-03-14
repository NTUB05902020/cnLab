#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "icmp.h"

int sockfd, ttl;

float get_msec_diff(struct timeval *t1, struct timeval *t2){
  return (t2->tv_sec - t1->tv_sec)*1000 + ((float)t2->tv_usec - (float)t1->tv_usec)/1000;
}

void changeTTL(int newValue){
  ttl = newValue;
  int ret = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
}

int main(int argc, char **argv){
  
  if(argc != 2){
    fprintf(stderr, "用法: %s <target IP>", argv[0]);
    exit(1);
  }

  // get destination IP address
  struct hostent *dst_hp;
  struct sockaddr_in dst;
  socklen_t dst_size = sizeof(dst);
  
  if((dst_hp = gethostbyname(argv[1])) == NULL){
      printf("unable to get the destination IP\n");
      exit(1);
  }
  dst.sin_addr = (*(struct in_addr *)dst_hp->h_addr);
  dst.sin_family = AF_INET;
  dst.sin_port = htons(81000);

  char dst_ip[50];
  sprintf(dst_ip, "%s", inet_ntoa(dst.sin_addr));
  printf("Destination IP is %s\n", dst_ip);
  
  // create socket
  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  printf("sockfd = %d\n", sockfd);
  
  ICMP *send_pac;
  struct timeval t1, t2;
  char buff[1000];
  int segment_size;
  struct sockaddr_in tmp_addr;
  socklen_t tmp_size = sizeof(tmp_addr);
  Info *info;
  float msec_diff;
  int fin = 0;

  for(uint16_t seqNum = 1; seqNum < 100 && fin == 0; seqNum++){
    changeTTL(seqNum);
    printf("%-5d", seqNum);
    char ipAddr[16];
    char time[3][100];
    for(uint16_t id = 0; id < 3; id++){
      struct timeval timeout = {1, 0};
      fd_set in_set;
      FD_ZERO(&in_set);
      FD_SET(sockfd, &in_set);
      
      /*--------------------start sending---------------------*/
      send_pac = set_header(id, seqNum);
      // get send time
      gettimeofday(&t1, NULL);
      sendto(sockfd, send_pac, sizeof(ICMP), 0, (struct sockaddr *)&dst, dst_size);
      //printf("s_id:%d s_seq:%d\n", send_pac->identifier, send_pac->seqNum);
      free(send_pac);

      /*--------------------start receiving---------------------*/
      
      if(select(sockfd+1, &in_set, NULL, NULL, &timeout) < 0){
	printf("cant select\n");
	exit(1);
      }
      if(FD_ISSET(sockfd, &in_set)){
	  segment_size = recvfrom(sockfd, buff, 1000, 0, (struct sockaddr *)&tmp_addr, &tmp_size);
	  // get receive time
	  gettimeofday(&t2, NULL);
	  if(segment_size > 0){
	    // unpack message
	    info = get_info(buff);
	    // print out IP address
	    sprintf(ipAddr, "%3u.%3u.%3u.%3u", (info->IP>>24) & 0xff, (info->IP>>16) & 0xff, (info->IP>>8) & 0xff, (info->IP) & 0xff);

	    msec_diff = get_msec_diff(&t1, &t2);
	    // print time difference
	    sprintf(time[id], "%8.3fms", msec_diff);
	    if(info->type == (uint8_t)0)
	      // echo reply
	      fin = 1;
	    else if(info->type == (uint8_t)11)
	      // time exceeded
	      fin = 0;
	    else{
	      // error
	      printf("receive type error\n");
	      fin = 1;
	    }
	    free(info);
	  }
      }
      else
	sprintf(time[id], "         *");
    }
    printf("%s%s%s%s", ipAddr, time[0], time[1], time[2]);
    printf("\n");
  }
	
  exit(0);
}
