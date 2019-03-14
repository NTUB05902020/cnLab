typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long long int uint64_t;
// ICMP header structure
typedef struct{
  uint8_t type; // unsigned 8bits
  uint8_t code; // unsigned 8bits
  uint16_t checksum; // unsigned 16bits
  uint16_t identifier; // unsigned 16bits
  uint16_t seqNum; // unsigned 16bits
}ICMP;

typedef struct{
  uint8_t type;
  uint32_t IP;
  uint16_t identifier;
  uint16_t seqNum;
}Info;
// return the header of icmp package with identifier and seqNum set
ICMP *set_header(uint16_t identifier, uint16_t seqNum);
// return IP, identifier and seqNum of the given package "buff"
Info *get_info(char *buff);
