typedef struct{
  unsigned char type; // unsigned 8bits
  unsigned char code; // unsigned 8bits
  unsigned short checksum; // unsigned 16bits
  unsigned short identifier; // unsigned 16bits
  short seqNum; // signed 16bits
}SPac;

typedef struct{
  unsigned char type; // unsigned 8bits
  unsigned char code; // unsigned 8bits
  int others; // 32bits
  int IP; // 32bits
  Spac original; // 64bits
}RPac;

typedef struct{
  unsigned char IP[4];
  unsigned short identifier;
  short seqNum;
}Info;

void set_checksum(SPac *pac);
Info *get_Info(RPac *pac);
