#ifndef _TGV_HH
#define _TGV_HH
#include "bal.hh"

#include <netlink/socket.h>
#include <netlink/socket_group.h>
#include <string>
namespace ptt
{
  class tgv 
  {
  public:
    tgv(std::string host,uint32_t port);
    uint64_t socketId(){ return _id;}
    void send(ptt::balMessage* m);
  private:
    void connect();
    void doScan();
    std::string _host;
    uint32_t _port;
    boost::thread_group g_store;
    NL::Socket* _socket;
    NL::SocketGroup* _group;
    uint64_t _id;
  };
};
#endif