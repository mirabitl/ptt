#include "PostOffice.hh"
#include "bal.hh"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/dir.h>  
#include <sys/param.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>
#include <stdint.h>
#include <fcntl.h>



PostOffice::PostOffice(std::string name,uint32_t port) : _running(false),_base("/dev/shm"),_group(NULL),_readSocket(NULL),_msh(NULL)
{
  _fsm=new fsmweb(name);

  // Register state
  _fsm->addState("CREATED");
  _fsm->addState("CONFIGURED");
  _fsm->addState("RUNNING");
  _fsm->addTransition("CONFIGURE","CREATED","CONFIGURED",boost::bind(&PostOffice::configure, this,_1));
  
  _fsm->addTransition("START","CONFIGURED","RUNNING",boost::bind(&PostOffice::start, this,_1));
  _fsm->addTransition("STOP","RUNNING","CONFIGURED",boost::bind(&PostOffice::stop, this,_1));
  _fsm->addTransition("HALT","RUNNING","CREATED",boost::bind(&PostOffice::halt, this,_1));
  _fsm->addTransition("HALT","CONFIGURED","CREATED",boost::bind(&PostOffice::halt, this,_1));

  //_fsm->addCommand("DIRECTORY",boost::bind(&PostOffice::directory, this,_1,_2));
  _fsm->addCommand("LISTMBOX",boost::bind(&PostOffice::listmbox, this,_1,_2));
  //_fsm->addCommand("REGISTERMBOX",boost::bind(&PostOffice::registermbox, this,_1,_2));

  //Start server
  _fsm->start(port);
  // Create OUT box
  struct stat st = {0};

  stringstream s;
  stringstream sfifo;
  s<<_base;
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/PO";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/OUTPUT";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/DATA";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  // Clear Data
  this->clearData(s.str());
  // Create fifos
  NL::init();
  
  std::cout<<"Clear TGV "<<std::endl;
  _tgvMap.clear();
  if (_group!=NULL) delete _group;
  _group=new NL::SocketGroup();
  if (_msh!=NULL) delete _msh;
  _msh =new ptt::PostMessageHandler(_base);
  _onRead= new ptt::OnRead(_msh);
  _onClientDisconnect= new ptt::OnClientDisconnect();
  _onDisconnect= new ptt::OnDisconnect(_msh);
  _onAccept=new ptt::OnAccept(_msh);
  _group->setCmdOnRead(_onRead);
  _group->setCmdOnAccept(_onAccept);
  _group->setCmdOnDisconnect(_onDisconnect);
  if (_readSocket!=NULL) delete _readSocket;
  std::cout<<"Create socket "<<std::endl;
  try {
    _readSocket=new NL::Socket(6666);
    _group->add(_readSocket);
  }
  catch (NL::Exception e)
  {
    std::stringstream s;
    s<<e.msg()<<" Cannot create socket ";
    std::cout<<s.str()<<std::endl;
  }
  std::cout<<"Call listen "<<std::endl; 
  this->listen();
}

void PostOffice::configure(ptt::fsmmessage* m)
{
  std::cout<<"Received "<<m->command()<<std::endl;
  std::cout<<"Received "<<m->value()<<std::endl;
  Json::Value jc=m->content();
  std::cout<<" content" <<jc<<std::endl;
  
  _postName=jc["postName"].asString();
  _postPort=jc["postPort"].asUInt();
  
  for (std::map<uint64_t,ptt::tgv*>::iterator it= _tgvMap.begin();it!=_tgvMap.end();it++) delete it->second;
  _tgvMap.clear();
  

  const Json::Value& jhosts = jc["hosts"];
  Json::Value array_keys;
  
  for (Json::ValueConstIterator it = jhosts.begin(); it != jhosts.end(); it++)
    {
      const Json::Value& h=(*it);
      std::cout<<(*it)<<std::endl;
      //std::string hn=(*it).asString();
      // Create a tgv to this hosts
      uint64_t id=( (uint64_t) ptt::balAddress::encode_po((char*) h["name"].asString().c_str(),h["port"].asUInt()));
      std::cout<<h["name"].asString()<<":"<<h["port"].asUInt()<<" TGV box ->"<<std::hex<<id<<std::dec<<std::endl;
      ptt::tgv* t= new ptt::tgv(h["name"].asString(),h["port"].asUInt());
      _tgvMap.insert(std::pair<uint64_t,ptt::tgv*>(id,t));
    }
    
    Json::Value array;
 array["ERROR"]="OK";
 
 m->setAnswer(array);
 return;
}

void PostOffice::start(ptt::fsmmessage* m)
{
    std::cout<<"Received "<<m->command()<<std::endl;
   
    _running=true;
    Json::Value jc=m->content();
    std::cout<<jc<<std::endl;
   

    this->check();
}

void PostOffice::dolisten()
{
  
  while(1) {
    
    if(!_group->listen(2000))
      std::cout << "\nNo msg recieved during the last 2 seconds";
    ::usleep(10);
  }
  
  
}
void PostOffice::listen()
{
  std::cout<<"Listenning on 6666"<<std::endl;
  g_read.create_thread(boost::bind(&PostOffice::dolisten, this));
  
}

void PostOffice::check()
{
  g_post.create_thread(boost::bind(&PostOffice::docheck, this));
  
}
void PostOffice::initFifo()
{
  stringstream name;
  name<<_base<<"/PO/OUTPUT/yellowbox";
  std::cout<<"Checking "<<name.str()<<std::endl;
  struct stat st;
  if (stat(name.str().c_str(), &st) != 0)
  {
    std::cout<<"Create fifo "<<std::endl;
    mkfifo(name.str().c_str(), 0666);
  }
  std::cout<<"open fifo "<<name.str()<<std::endl;
  _fd = open(name.str().c_str(), O_RDONLY);
  std::cout<<"Open "<<name.str()<<" "<<_fd<<std::endl;
  if (_fd == -1) {
    perror("open");
    return;
  }
  //FD_ZERO(&_fifoSet);
  //FD_SET(_fd, &_fifoSet);
  
}

void PostOffice::docheck()
{
  int n;
  uint64_t code;
  char buf[1024];
  uint64_t * ibuf=(uint64_t*) buf;
  this->initFifo();
  while (_running)
  {
    int bytes = read(_fd, buf, 8);
    if (bytes>0)
    {
      //printf("done reading (%lu bytes) %lx\n", bytes,ibuf[0]);
      if (bytes==8)
      {
        code= ibuf[0];
        if (code!=0 )
        {
          balMessage m((uint32_t) 0x500000);
          stringstream name;
          name<<_base<<"/PO/OUTPUT/DATA";
          m.read(name.str(),code);
          std::string adr;
          adr.assign(m.dest());
          balAddress ba(adr);
          //std::cout<<" message "<<std::hex<<m.code()<<std::dec<<"to "<<ba.address()<<std::endl;
          //std::cout<<" socket id "<<std::hex<<ba.socketId()<<std::dec<<std::endl;
          ::usleep(1);
          if (_tgvMap.find(ba.socketId())!=_tgvMap.end())
          {_msh->lock();
            _tgvMap[ba.socketId()]->send(&m);
            _msh->unlock();
          }
        }
      }
    }
  }
    /*
    n = select(_fd+1, &_fifoSet, NULL, NULL, NULL);
    if (!n)
      continue;
    if (n == -1)
    {
      perror("select");
      return;
    }
    if (FD_ISSET(_fd, &_fifoSet))
    {
      printf("Descriptor %d is ready.\n",_fd);
      char buf[1024];
      
      
     int ier=0;
      int size_remain=8;
      uint64_t code=0;
      //char * buf=(char*) code;
       ier=read(_fd,buf,8);
       printf("ier %d char reads \n",ier);
       if (ier>0) {
         memcpy(&code,buf,8);
       }
      if (code!=0 )
      {
      balMessage m((uint32_t) 0x100000);
      stringstream name;
      name<<_base<<"/PO/OUTPUT/DATA";
      m.read(name.str(),code);
      std::string adr;
      adr.assign(m.dest());
      balAddress ba(adr);
      std::cout<<" message to "<<ba.address()<<std::endl;
      std::cout<<" socket id "<<std::hex<<ba.socketId()<<std::dec<<std::endl;
      if (_tgvMap.find(ba.socketId())!=_tgvMap.end())
        _tgvMap[ba.socketId()]->send(&m);
      }
      
      }
    }
    */
  }
  





void PostOffice::stop(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
  
    // Stop running
    _running=false;
    ::sleep(1);
    std::cout<<"joining"<<std::endl;
   g_post.join_all();
}
void PostOffice::halt(ptt::fsmmessage* m)
{
  
  
    std::cout<<"Received "<<m->command()<<std::endl;
    if (_running)
      this->stop(m);
    std::cout<<"Destroying"<<std::endl;
   
}
extern  int alphasort(); 
int file_select_po(const struct direct *entry)  
{  
  if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))  
    return (0);  
  else  
    return (1);  
}  
void PostOffice::clearData(std::string dir)
{
      int count,i;  
    struct direct **files;  

    count = scandir(dir.c_str(), &files, file_select_po, alphasort);      

    std::cout<<dir<<" Nfiles "<<count<<std::endl;
    if(count <= 0)    {return ;}
    
  
    for (i=1; i<count+1; ++i)  
    {
      // file name

      std::cout<<files[i-1]->d_name<<std::endl;
      std::stringstream s;
      s<<dir<<"/"<<files[i-1]->d_name;
      ::unlink(s.str().c_str());
      
     
      free(files[i-1]);
    }
    free(files);
  
}
void PostOffice::listmbox(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"dowmnload"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    
    int count,i;  
    struct direct **files;  
    std::stringstream sc;
    sc.str(std::string());
    sc<<_base<<"/PO/BAL/";
    
    count = scandir(sc.str().c_str(), &files, file_select_po, alphasort);      
   
    if(count <= 0)    {return ;}
    
    std::stringstream sd;     
    //printf("Number of files = %d\n",count);
    Json::Value array;
    for (i=1; i<count+1; ++i)  
    {
      // file name
      std::string fName;
      fName.assign(files[i-1]->d_name);
      
      array.append(fName);

      free(files[i-1]);
    }
    free(files);
    std::cout<<array<<std::endl;
    response["answer"]=array;
}

void PostOffice::list(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
    std::cout<<"list"<<request.getUrl()<<" "<<request.getMethod()<<" "<<request.getData()<<std::endl;
    response["answer"]="list called";
}




