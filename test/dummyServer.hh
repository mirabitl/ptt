#ifndef _dummyServer_h
#define _dummyServer_h

#include "fsmweb.hh"

class dummyServer {
public:
  dummyServer(std::string name,uint32_t port);
  void configure(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void stop(ptt::fsmmessage* m);
  void halt(ptt::fsmmessage* m);
  void download(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void list(Mongoose::Request &request, Mongoose::JsonResponse &response);

private:
  fsmweb* _fsm;
  bool _running,_readout;
};

#endif
