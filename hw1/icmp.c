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
Info *get_info(char *buff){
  // IPv4 VHL = version(4 bits) + header length(4 bits)
  vhl = *(uint8_t *)buff;
  header_size = (vhl & 0x0f) * 4;

  // get the IP address of the source
  // source address starts at the 12th byte of the IP header
  uint32_t IP = *(uint8_t *)(buff + 12);

  // get the ICMP header
  ICMP *icmp_head = (ICMP *)(buff + header_size);

  // check type
  if(icmp_head->type == 0){
    // echo reply
    Info *info = (Info *)malloc(sizeof(Info));
    info->IP = IP;
    info->identifier = ntohs(icmp_head->identifier);
    info->seqNum = ntohs(icmp_head->seqNum);
    return info;
  }
  // invalid
  return NULL;
}
