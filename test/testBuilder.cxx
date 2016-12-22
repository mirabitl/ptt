#include "testBuilder.hh"
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace ptt;

testBuilder::testBuilder(std::string name,uint32_t port) : _running(false),_bal(NULL)
{
  _fsm=new fsmweb(name);

  // Register state
  _fsm->addState("CREATED");
  _fsm->addState("CONFIGURED");
  _fsm->addState("RUNNING");
  _fsm->addTransition("CONFIGURE","CREATED","CONFIGURED",boost::bind(&testBuilder::configure, this,_1));
    _fsm->addTransition("CONFIGURE","CONFIGURED","CONFIGURED",boost::bind(&testBuilder::configure, this,_1));
  _fsm->addTransition("START","CONFIGURED","RUNNING",boost::bind(&testBuilder::start, this,_1));
  _fsm->addTransition("STOP","RUNNING","CONFIGURED",boost::bind(&testBuilder::stop, this,_1));
  _fsm->addTransition("HALT","RUNNING","CREATED",boost::bind(&testBuilder::halt, this,_1));
  _fsm->addTransition("HALT","CONFIGURED","CREATED",boost::bind(&testBuilder::halt, this,_1));

  _fsm->addCommand("DOWNLOAD",boost::bind(&testBuilder::download, this,_1,_2));
  _fsm->addCommand("LIST",boost::bind(&testBuilder::list, this,_1,_2));

  //Start server
  _fsm->start(port);

 
  _bal = new ptt::bal("/dev/shm","BUILDER",1,boost::bind(&testBuilder::readMail, this,_1));
  _bal->start();
  std::cout<<" Loop started\n";
}

void testBuilder::readMail(uint64_t code)
{
  balMessage m((uint32_t) 0x500000);
  
  m.read(_bal->dataDir(),code);
  //std::cout<<balMessage::id(m.code())<<"from "<<m.origin().address()<<std::endl;
  //std::cout<<"to "<<m.destination().address()<<std::endl;
  //balAddress ba((m));
  if (balMessage::id(m.code()>>16)%100==0)
  {
  balMessage* mr=new balMessage(0x500);
 
  memcpy(mr->orig(),m.dest(),128);
  memcpy(mr->dest(),m.orig(),128);
  
  mr->encode(balMessage::id(m.code()),ptt::balMessage::ACK);
  mr->setPayloadSize(0x100);
  _bal->post((*mr));
  delete mr;
  }
  return;
  //std::cout<<"Received message with code "<<std::hex<<code<<" or " <<m->code()<<std::dec<<"from"<<ba.address()<<" with payload size "<<m->payloadSize()<<std::endl;
  //std::cout<<"Received message with code "<<std::hex<<code<<" or " <(uint64_t) m<<std::endl;
  printf("%lx at %lx \n",code,&m);
  //delete m;
}
void testBuilder::configure(ptt::fsmmessage* m)
{
  std::cout<<"Received "<<m->command()<<std::endl;
  std::cout<<"Received "<<m->value()<<std::endl;

 
  Json::Value jc=m->content();
  std::cout<<jc<<std::endl;
 
  Json::Value array_keys;
  array_keys.append(12);
  array_keys.append(22);
  array_keys.append(32);
  m->setAnswer(array_keys);
  
  
  ptt::balMessage bm((uint32_t) 0x500);
  std::string _builder("{\"host\":\"lyosdhcal10\",\"name\":\"FEEDER\",\"number\":2,\"port\":6666}");
  
  ptt::balAddress ma(_builder);
  std::cout<<ma.address().c_str()<<ma.address().length()<<std::endl;
  memcpy(bm.dest(),ma.address().c_str(),ma.address().length());

  int id=0;
  for (int k=0;k<150;k++)
  {
    id++;
    bm.encode(id,1);
    std::cout <<"preparing "<<std::hex<<bm.code()<<std::endl; 
    uint32_t l=0x200;
    bm.setPayloadSize(l);
    char* b=bm.payload();
    for (uint64_t i=0;i<l;i++)
      b[i]=(i+id)&0xFF;
    std::cout <<"Sendding "<<l<<"  bytes "<<std::hex<<bm.code()<<std::dec<<std::endl; 
    _bal->post(bm);
    
    //::sleep(1);
  }
  
  
  
  // Overwrite msg
    //Prepare complex answer
  
  
}

void testBuilder::start(ptt::fsmmessage* m)
{
    std::cout<<"Received "<<m->command()<<std::endl;

    _running=true;
    Json::Value jc=m->content();
    std::cout<<jc<<std::endl;

}
void testBuilder::stop(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
  
    // Stop running
    _running=false;
    ::sleep(1);
    std::cout<<"joining"<<std::endl;
   
}
void testBuilder::halt(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
    if (_running)
      this->stop(m);
    std::cout<<"Destroying"<<std::endl;
   
}
void testBuilder::download(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"dowmnload"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="download called";
}

void testBuilder::list(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"list"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="list called";
}


int main()
{
  testBuilder s("DUS",45000);
  getchar();
  while (1)
    {
      ::sleep(1);
    }
}



