#ifndef _testBuilder_h
#define _testBuilder_h

#include "fsmweb.hh"
#include "bal.hh"

class testBuilder {
public:
  testBuilder(std::string name,uint32_t port);
  void configure(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void stop(ptt::fsmmessage* m);
  void halt(ptt::fsmmessage* m);
  void readMail(uint64_t code);
  void download(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void list(Mongoose::Request &request, Mongoose::JsonResponse &response);

private:
  fsmweb* _fsm;
  bool _running,_readout;
  ptt::bal* _bal;
};

#endif
