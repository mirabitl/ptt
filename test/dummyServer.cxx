#include "dummyServer.hh"
#include <cstdlib>
#include <iostream>
#include <ctime>



dummyServer::dummyServer(std::string name,uint32_t port) : _running(false)
{
  _fsm=new fsmweb(name);

  // Register state
  _fsm->addState("CREATED");
  _fsm->addState("CONFIGURED");
  _fsm->addState("RUNNING");
  _fsm->addTransition("CONFIGURE","CREATED","CONFIGURED",boost::bind(&dummyServer::configure, this,_1));
    _fsm->addTransition("CONFIGURE","CONFIGURED","CONFIGURED",boost::bind(&dummyServer::configure, this,_1));
  _fsm->addTransition("START","CONFIGURED","RUNNING",boost::bind(&dummyServer::start, this,_1));
  _fsm->addTransition("STOP","RUNNING","CONFIGURED",boost::bind(&dummyServer::stop, this,_1));
  _fsm->addTransition("HALT","RUNNING","CREATED",boost::bind(&dummyServer::halt, this,_1));
  _fsm->addTransition("HALT","CONFIGURED","CREATED",boost::bind(&dummyServer::halt, this,_1));

  _fsm->addCommand("DOWNLOAD",boost::bind(&dummyServer::download, this,_1,_2));
  _fsm->addCommand("LIST",boost::bind(&dummyServer::list, this,_1,_2));

  //Start server
  _fsm->start(port);
}

void dummyServer::configure(ptt::fsmmessage* m)
{
  std::cout<<"Received "<<m->command()<<std::endl;
  std::cout<<"Received "<<m->value()<<std::endl;

 
  Json::Value jc=m->content();
  std::cout<<jc<<std::endl;
 
  Json::Value array_keys;
  array_keys.append(12);
  array_keys.append(22);
  array_keys.append(32);

  // Overwrite msg
    //Prepare complex answer
  m->setAnswer(array_keys);
  
}

void dummyServer::start(ptt::fsmmessage* m)
{
    std::cout<<"Received "<<m->command()<<std::endl;

    _running=true;
    Json::Value jc=m->content();
    std::cout<<jc<<std::endl;

}
void dummyServer::stop(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
  
    // Stop running
    _running=false;
    ::sleep(1);
    std::cout<<"joining"<<std::endl;
   
}
void dummyServer::halt(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
    if (_running)
      this->stop(m);
    std::cout<<"Destroying"<<std::endl;
   
}
void dummyServer::download(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"dowmnload"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="download called";
}

void dummyServer::list(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"list"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="list called";
}


int main()
{
  dummyServer s("DUS",45000);
  while (1)
    {
      ::sleep(1);
    }
}



