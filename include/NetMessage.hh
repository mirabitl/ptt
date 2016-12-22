#ifndef _NetMessage_h
#define _NetMessage_h
#include <string.h>
#include <string>
#include <stdint>

class NetMessageHeader
{
public:
  NetMessageHeader(){_size=0;}
  void setSize(uint32_t s) {_size=s;}
  uint32_t size(){return _size;}
private:
  uint32_t _size;
};

class NetMessage
{
 public:

  NetMessage(uint32_t payLoadsize=4) { _message= new unsigned char[sizeof(NetMessageHeader)+payloadsize];_alloc=true;}
  NetMessage(unsigned char* b,uint32_t maxlen){ _message=b;_alloc=false;}
  ~NetMessage(){if (_alloc) delete _message;}
  
  NetMessageHeader* getHeader(){return (NetMessageheader*) _message;}
  uint32_t payloadSize(){return this->getHeader()->size();}
  uint32_t size(){return this->getHeader()->size()+sizeof(NetMessageHeader);}
  unsigned char* payload(){return _message[sizeof(NetMessageHeader)];}
  unsigned char* message(){return _message;}
  void setPayload(unsigned char* b,uint32_t s=0){memcpy(payload(),b,s);}
 private:
  
  unsigned char* _message;
  bool _alloc;

};
#endif
