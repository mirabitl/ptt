#ifndef _BAL_HH
#define _BAL_HH
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<errno.h> 
typedef boost::function<void (uint64_t)> balFunctor;

namespace ptt
{
  class balAddress;
  class balMessage
  {
  public:
    enum {NORMAL=1,EXPRESS=2,ACK=3,RTS=4};
    balMessage(uint32_t size); 
    balMessage(std::string address,uint32_t lenpay,char* payload); 
    ~balMessage();
    void setDestination(balAddress& ad);
    void setOrigin(balAddress& ad);
    int write(std::string path);
    
    int read(std::string path,uint64_t code);
  
    uint64_t& code();
    
    char* dest();
    char* orig();
    static uint32_t length(uint64_t code);
    static uint8_t type(uint64_t code);
    static uint32_t id(uint64_t code);
    char* payload();
    void encode(uint32_t id,uint8_t mtype); 
    uint32_t payloadSize();
    void setPayloadSize(uint32_t len);
    balAddress destination();
    balAddress origin();
  private:
    uint64_t _code;
    char _dest[128];
    char _orig[128];
    char* _buf;
    
    bool _allocate;
  };
  class balAddress
{
  
  public:
  balAddress(Json::Value &j);
    balAddress(std::string h,uint32_t p,std::string name,uint32_t num);
    balAddress(std::string r);
    balAddress(char* r);
    balAddress(ptt::balMessage r);
    std::string address();
    std::string host();
    std::string name();
    uint32_t port();
    uint32_t number();
    static int hostname_to_ip(const char * hostname , char* ip);
  
  static uint64_t encode_po(const char* host,uint32_t port);
  
  static uint64_t encode_destination(char* ip,uint32_t port);
 
  uint64_t socketId();
  private:
    Json::Value _jval;
  };
 
  class bal
  {
  public:
    bal(std::string base,std::string name,uint16_t num,balFunctor handler);
    void start();
    void stop();
    void post(balMessage &m);
    void initFifo();
    void doCheck();
    std::string dataDir(){return _dataDir;}
    std::string base(){return _base;}
    std::string name(){return _name;}
    uint16_t number(){return _num;}
  private:
    std::string _base;
    std::string _name;
    uint16_t _num;
    std::string _dataDir;
    std::string _fifoDir;
    balFunctor _handler;
    fd_set _fifoSet;
    int32_t _fd;
    boost::thread_group g_store;
    bool _active;
  };
};
#endif
