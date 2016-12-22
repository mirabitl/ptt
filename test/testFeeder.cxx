#include "testFeeder.hh"
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace ptt;

testFeeder::testFeeder(std::string name,uint32_t port) : _running(false),_bal(NULL),_builderAddress("{\"host\":\"lyopc252\",\"port\":6666,\"name\":\"BUILDER\",\"number\":1}")
{
  _fsm=new fsmweb(name);

  // Register state
  _fsm->addState("CREATED");
  _fsm->addState("CONFIGURED");
  _fsm->addState("RUNNING");
  _fsm->addTransition("CONFIGURE","CREATED","CONFIGURED",boost::bind(&testFeeder::configure, this,_1));
    _fsm->addTransition("CONFIGURE","CONFIGURED","CONFIGURED",boost::bind(&testFeeder::configure, this,_1));
  _fsm->addTransition("START","CONFIGURED","RUNNING",boost::bind(&testFeeder::start, this,_1));
  _fsm->addTransition("STOP","RUNNING","CONFIGURED",boost::bind(&testFeeder::stop, this,_1));
  _fsm->addTransition("HALT","RUNNING","CREATED",boost::bind(&testFeeder::halt, this,_1));
  _fsm->addTransition("HALT","CONFIGURED","CREATED",boost::bind(&testFeeder::halt, this,_1));

  _fsm->addCommand("DOWNLOAD",boost::bind(&testFeeder::download, this,_1,_2));
  _fsm->addCommand("LIST",boost::bind(&testFeeder::list, this,_1,_2));

  //Start server
  _fsm->start(port);

 
 
}

void testFeeder::readMail(uint64_t code)
{
  balMessage m((uint32_t) 0x500000);
  m.read(_bal->dataDir(),code);
  std::cout<<"Received message"<<std::hex<<m.code()<<std::dec<<"from"<<m.origin().address()<<" with payload size "<<m.payloadSize()<<std::endl;
}
void testFeeder::configure(ptt::fsmmessage* m)
{
  std::cout<<"Received "<<m->command()<<std::endl;
  std::cout<<"Received "<<m->value()<<std::endl;

 
  Json::Value jc=m->content();

  std::cout<<jc<<std::endl;
  _builderAddress=jc["builder"];
  std::cout<<_builderAddress<<std::endl;
  _bal = new ptt::bal("/dev/shm",jc["name"].asString(),jc["instance"].asUInt(),boost::bind(&testFeeder::readMail, this,_1));
  _bal->start();
  std::cout<<"Apres "<<std::endl;
  Json::Value array_keys;
  array_keys.append(12);
  array_keys.append(22);
  array_keys.append(32);

  // Overwrite msg
    //Prepare complex answer
  m->setAnswer(array_keys);
  
}
void testFeeder::dosend()
{
  ptt::balMessage m(0x500000);
  ptt::balAddress ma(_builderAddress);
  memcpy(m.dest(),ma.address().c_str(),ma.address().length());
  int id=0;
  while (_running)
  {
	  id++;
      uint32_t num=((id%0xFFFF)<<16|(_bal->number()&0xFFFF));
	  m.encode(num,1);
	  //std::cout <<"preparing "<<std::hex<<m.code()<<std::endl; 
	  uint32_t l=0x20000+id%20*0x20000;
	  m.setPayloadSize(l);
	  char* b=m.payload();
	  for (uint64_t i=0;i<l;i++)
	    b[i]=(i+id)&0xFF;
	  //std::cout <<"Sendding "<<l<<"  bytes "<<std::hex<<m.code()<<std::dec<<std::endl; 
	 _bal->post(m);
    
    ::usleep(101);
  }
  
}
void testFeeder::start(ptt::fsmmessage* m)
{
    std::cout<<"Received "<<m->command()<<std::endl;

    _running=true;
    Json::Value jc=m->content();
    std::cout<<jc<<std::endl;
    g_read.create_thread(boost::bind(&testFeeder::dosend, this));
    
}
void testFeeder::stop(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
  
    // Stop running
    _running=false;
    ::sleep(1);
    std::cout<<"joining"<<std::endl;
   
}
void testFeeder::halt(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
    if (_running)
      this->stop(m);
    std::cout<<"Destroying"<<std::endl;
   
}
void testFeeder::download(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"dowmnload"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="download called";
}

void testFeeder::list(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"list"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="list called";
}


int main()
{
  testFeeder s("DUF",46000);
  while (1)
    {
      ::sleep(1);
    }
}



