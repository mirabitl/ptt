#ifndef _TDC_MANAGER_HH
#define _TDC_MANAGER_HH
#include "TdcMessageHandler.hh"
#include "fsmweb.hh"
#include "PRSlow.hh"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <iostream>
class TdcManager 
{
public:
  TdcManager(std::string name,uint32_t port);
  ~TdcManager(){;}
  void initialise(ptt::fsmmessage* m);
  void configure(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void stop(ptt::fsmmessage* m);
  void destroy(ptt::fsmmessage* m);
  void c_joblog(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void c_status(Mongoose::Request &request, Mongoose::JsonResponse &response);
  
  void parseConfig(std::string name);
  void writeRamAvm();
  void queryCRC();
  void listen();
  void dolisten();
private:
  NL::Socket* _sCtrl;
  NL::Socket* _sTDC1;
  NL::Socket* _sTDC2;
  NL::SocketGroup* _group;
 
  ptt::TdcMessageHandler* _msh;
  ptt::OnRead* _onRead;
  ptt::OnAccept* _onAccept;
  ptt::OnClientDisconnect* _onClientDisconnect;
  ptt::OnDisconnect* _onDisconnect;
  boost::thread_group g_store;


  uint32_t disconnected_;
  fsmweb* _fsm;
  uint16_t _slcBuffer[0x1000];
  uint16_t _slcAddr[0x1000];    
  uint32_t _slcBytes;
  uint16_t _slcAck[0x1000];
  uint32_t _slcAckLength;
  PRSlow _s1,_s2;
};
#endif
