#include "tgv.hh"
#include "bal.hh"
using namespace ptt;
tgv::tgv(std::string host, uint32_t port) :_host(host),_port(port),_group(NULL),_socket(NULL)
{
  char ip[100];
  ptt::balAddress::hostname_to_ip(_host.c_str(),ip);
  _id=ptt::balAddress::encode_destination(ip,_port);
this->connect();
}
void ptt::tgv::connect()
{
  std::cout<<"connecting to "<<_host<<" on port "<<_port<<std::endl;
   if (_group!=NULL) delete _group;
  _group=new NL::SocketGroup();
  
if (_socket!=NULL) delete _socket;
  try {
  _socket=new NL::Socket(_host.c_str(),_port);
  _group->add(_socket);
  }
  catch (NL::Exception e)
  {
     std::cout<<e.msg()<<" Cannot open  socket "<<_host<<":"<<_port<<std::endl;
    return;
  }
  return;
}
void ptt::tgv::doScan(){}

void ptt::tgv::send(ptt::balMessage* m)
{
  try {
  _socket->send((const void*) &(m->code()),sizeof(uint64_t));
  _socket->send((const void*) m->dest(),128);
  _socket->send((const void*) m->orig(),128);
  _socket->send((const void*) m->payload(),m->payloadSize());
  }
   catch (NL::Exception e)
  {
    std::cout<<e.msg()<<" Cannot write "<<std::hex<<m->code()<<std::dec<<" to  socket "<<m->dest()<<std::endl;
    return;
    _group->remove(_socket);
    //delete _socket;
     try {
  _socket=new NL::Socket(_host.c_str(),_port);
  _group->add(_socket);
  }
  catch (NL::Exception e)
  {
     std::cout<<e.msg()<<" Cannot open  socket "<<_host<<":"<<_port<<std::endl;
    return;
  }
    return;
  }
}
