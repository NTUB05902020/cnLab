#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
void set_checksum(SPac *pac){
  unsigned int sum = 0;
  sum = pac->type;
  sum <<= 8;
  sum += pac->code + pac->identifier + pac->seqNum;
  pac->checksum = ~(sum + (sum>>16));
}  
Info *get_Info(RPac *pac){
  // echo reply
  if(pac->type == 0){
    Info *info = (Info *)malloc(sizeof(Info));
    info->IP = pac->IP;
    info->identifier = pac->original.identifier;
    info->seqNum = pac->original.seqNum;
    return info;
  }
  # invalid
  return NULL;
}
