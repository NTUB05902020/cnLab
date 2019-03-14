#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

static uint16_t get_checksum(const char *pac, size_t size){
  uint32_t sum = 0;
  for(int i = 0; i < size; i += 2){
    sum += *(uint16_t *)pac;
    pac += 2;//move to next 16bits 
  }
  while(sum>>16 != 0)
    sum = (sum & 0xffff) + (sum >> 16);
  return ~sum;
}
  
ICMP *set_header(uint16_t identifier, uint16_t seqNum){
  ICMP *pac = (ICMP *)malloc(sizeof(ICMP));
  // echo request
  pac->type = (uint8_t)8;
  pac->code = 0;
  pac->checksum = 0;
  pac->identifier = htons(identifier);
  pac->seqNum = htons(seqNum);
  pac->checksum = get_checksum((const char *)pac, sizeof(*pac));
  return pac;
}

Info *get_info(char *buff){
  // IPv4 VHL = version(4 bits) + header length(4 bits)
  uint8_t vhl, header_size;
  vhl = *(uint8_t *)buff;
  header_size = (vhl & 0x0f) * 4;

  // get the IP address of the source
  // source address starts at the 12th byte of the IP header
  uint32_t IP = *(uint32_t *)(buff + 12);

  // get the ICMP header
  ICMP *icmp_head = (ICMP *)(buff + header_size);

  Info *info = (Info *)malloc(sizeof(Info));
  info->IP = ntohl(IP);
  info->type = icmp_head->type;
  info->identifier = ntohs(icmp_head->identifier);
  info->seqNum = ntohs(icmp_head->seqNum);
  //printf("info->type = %u, id = %hu, seq = %hu\n", info->type, info->identifier, info->seqNum);
  return info;
}
