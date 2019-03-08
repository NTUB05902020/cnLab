typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
// ICMP header structure
typedef struct{
  uint8_t type; // unsigned 8bits
  uint8_t code; // unsigned 8bits
  uint16_t checksum; // unsigned 16bits
  uint16_t identifier; // unsigned 16bits
  uint16_t seqNum; // unsigned 16bits
}ICMP;

typedef struct{
  uint32_t IP;
  uint16_t identifier;
  uint16_t seqNum;
}Info;
// set the checksum field of the given package "pac"
void set_checksum(ICMP *pac);
// return IP, identifier and seqNum of the given package "buff"
Info *get_info(char *buff);
