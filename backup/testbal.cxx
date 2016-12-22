#include "bal.hh"
int main()
{
 printf("%lx \n",wemove::balAddress::encode_po("lyopc252",6666));
 
 wemove::balMessage m(0x100000);
 printf("code initial %lx \n",m.code());
 for (int i=1;i<257;i++)
 {
  m.encode(i,1);
  printf("Avec %d 1 -> %lx \n",i,m.code());
 }
 uint64_t code=m.code();
 uint32_t len=wemove::balMessage::length(0x4523+128+sizeof(uint64_t));
 printf( "length %x %x %lx\n",len, len&0xFFFFFFF, (code&~0xFFFFFFF )|len);
 m.setPayloadSize(0x4523);
 printf("size 0x4523 -> %lx %lx \n",m.code(),~((uint64_t)0xF0000000));
 
 
}
