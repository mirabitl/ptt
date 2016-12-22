#ifndef _POST_OFFICE_HH
#define _POST_OFFICE_HH
#include "PostMessageHandler.hh"
#include "fsmweb.hh"
#include "PRSlow.hh"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <iostream>
#include "tgv.hh"

class PostOffice 
{
public:
  PostOffice(std::string name,uint32_t port);
  ~PostOffice(){;}
  void configure(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void dolisten();
  void listen();
  void check();
  void initFifo();
  void docheck();
  void clearData(std::string dir);
  void stop(ptt::fsmmessage* m);
  void halt(ptt::fsmmessage* m);
  void listmbox(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void list(Mongoose::Request &request, Mongoose::JsonResponse &response);
  
private:
  NL::Socket* _readSocket;
  NL::Socket* _sTDC1;
  NL::Socket* _sTDC2;
  NL::SocketGroup* _group;
 
  ptt::PostMessageHandler* _msh;
  ptt::OnRead* _onRead;
  ptt::OnAccept* _onAccept;
  ptt::OnClientDisconnect* _onClientDisconnect;
  ptt::OnDisconnect* _onDisconnect;
  boost::thread_group g_read;
  boost::thread_group g_post;
  

  fsmweb* _fsm;
  std::string _base,_postName;
  uint32_t _postPort;
  bool _running;
  std::map<uint64_t,ptt::tgv*> _tgvMap;
  fd_set _fifoSet;
  int32_t _fd;
};
#endif
