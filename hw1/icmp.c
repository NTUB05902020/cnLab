#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

void set_checksum(ICMP *pac){
  uint32_t sum = 0;
  sum = pac->type;
  sum <<= 8;
  sum += pac->code + pac->identifier + pac->seqNum;
  pac->checksum = (uint16_t)~((sum & 0xffff) + (sum>>16));
}

ICMP *set_header(int identifier, int seqNum){
  ICMP *pac = (ICMP *)malloc(sizeof(ICMP));
  // echo request
  pac->type = (uint8_t)8;
  pac->code = 0;
  pac->identifier = (uint16_t)identifier;
  pac->seqNum = (uint16_t)seqNum;
  set_checksum(pac);
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
  info->IP = IP;
  info->type = icmp_head->type;
  info->identifier = (icmp_head->identifier);
  info->seqNum = (icmp_head->seqNum);
  //printf("info->type = %d, id = %d, seq = %d\n", info->type, info->identifier, info->seqNum);
  return info;
}
