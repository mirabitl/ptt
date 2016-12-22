#ifndef _testFeeder_h
#define _testFeeder_h

#include "fsmweb.hh"
#include "bal.hh"

class testFeeder {
public:
  testFeeder(std::string name,uint32_t port);
  void configure(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void stop(ptt::fsmmessage* m);
  void halt(ptt::fsmmessage* m);
  void readMail(uint64_t code);
  void download(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void list(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void dosend();
private:
  fsmweb* _fsm;
  bool _running,_readout;
  ptt::bal* _bal;
  boost::thread_group g_read;
  Json::Value _builderAddress;
};

#endif
