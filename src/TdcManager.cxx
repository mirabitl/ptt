#include "TdcManager.hh"
using namespace ptt;

TdcManager::TdcManager(std::string name,uint32_t port) : _group(NULL),_sCtrl(NULL),_sTDC1(NULL),_sTDC2(NULL)
{
  //_fsm=new ptt::fsm(name);
  _fsm=new fsmweb(name);
  // Register state
  _fsm->addState("CREATED");
  _fsm->addState("INITIALISED");
  _fsm->addState("CONFIGURED");
  _fsm->addState("RUNNING");
  
  _fsm->addTransition("INITIALISE","CREATED","INITIALISED",boost::bind(&TdcManager::initialise, this,_1));
  _fsm->addTransition("CONFIGURE","INITIALISED","CONFIGURED",boost::bind(&TdcManager::configure, this,_1));
  _fsm->addTransition("CONFIGURE","CONFIGURED","CONFIGURED",boost::bind(&TdcManager::configure, this,_1));
  
  _fsm->addTransition("START","CONFIGURED","RUNNING",boost::bind(&TdcManager::start, this,_1));
  _fsm->addTransition("STOP","RUNNING","CONFIGURED",boost::bind(&TdcManager::stop, this,_1));
  _fsm->addTransition("DESTROY","CONFIGURED","CREATED",boost::bind(&TdcManager::destroy, this,_1));
  _fsm->addTransition("DESTROY","INITIALISED","CREATED",boost::bind(&TdcManager::destroy, this,_1));
  
  
  
  //_fsm->addCommand("JOBLOG",boost::bind(&TdcManager::c_joblog,this,_1,_2));
  _fsm->addCommand("STATUS",boost::bind(&TdcManager::c_status,this,_1,_2));
  
  
  
 
    std::cout<<"Service "<<name<<" started on port "<<port<<std::endl;
    _fsm->start(port);
  
 
  // Initialise NetLink
  NL::init();
  
}
void TdcManager::c_status(Mongoose::Request &request, Mongoose::JsonResponse &response)
{
  response["STATUS"]="DONE";
  response["EVENT"]=_msh->readout();
  
}
void TdcManager::initialise(ptt::fsmmessage* m)
{
  ///LOG4CXX_INFO(_logLdaq," CMD: "<<m->command());
  std::cout<<m->command()<<std::endl<<m->content()<<std::endl;
  
  uint32_t pSLC=m->content()["portSLC"].asInt();
  std::string hSLC=m->content()["hostSLC"].asString();
  
  uint32_t pTDC1=m->content()["portTDC1"].asInt();
  std::string hTDC1=m->content()["hostTDC1"].asString();
  
  
  uint32_t pTDC2=m->content()["portTDC2"].asInt();
  std::string hTDC2=m->content()["hostTDC2"].asString();
  
  
  
  if (_group!=NULL) delete _group;
  _group=new NL::SocketGroup();
  _msh =new ptt::TdcMessageHandler("/dev/shm");
  _onRead= new ptt::OnRead(_msh);
  _onClientDisconnect= new ptt::OnClientDisconnect();
  _onDisconnect= new ptt::OnDisconnect(_msh);
  
  _onAccept=new ptt::OnAccept(_msh);
  
  _group->setCmdOnRead(_onRead);
  _group->setCmdOnAccept(_onAccept);
 // _group->setCmdOnDisconnect(_onDisconnect);
  _group->setCmdOnDisconnect(_onClientDisconnect);
  //return;
  std::cout<<hSLC<<":"<<pSLC<<std::endl;
  if (_sCtrl!=NULL) delete _sCtrl;
  try {
  _sCtrl=new NL::Socket(hSLC.c_str(),pSLC);
  _group->add(_sCtrl);
  }
  catch (NL::Exception e)
  {
     std::stringstream s;
     s<<e.msg()<<" SLC socket "<<hSLC<<":"<<pSLC;
     std::cout<<s.str()<<std::endl;
    Json::Value array;
   array["ERROR"]=s.str();

   m->setAnswer(array);
    return;
  }
  std::cout<<hTDC1<<":"<<pTDC1<<std::endl;
  
  if (_sTDC1!=NULL) delete _sTDC1;
  try {
  _sTDC1=new NL::Socket(hTDC1.c_str(),pTDC1);
  _group->add(_sTDC1);
  }
  catch (NL::Exception e)
  {
     std::stringstream s;
     s<<e.msg()<<" TDC1 socket "<<hTDC1<<":"<<pTDC1;
     std::cout<<s.str()<<std::endl;
    Json::Value array;
   array["ERROR"]=s.str();

   m->setAnswer(array);
    return;
  }
  std::cout<<hTDC2<<":"<<pTDC2<<std::endl;
  try {
  if (_sTDC2!=NULL) delete _sTDC2;
  _sTDC2=new NL::Socket(hTDC2.c_str(),pTDC2);
  _group->add(_sTDC2);
  }
 catch (NL::Exception e)
  {
     std::stringstream s;
     s<<e.msg()<<" TDC2 socket "<<hTDC2<<":"<<pTDC2;
     std::cout<<s.str()<<std::endl;
    Json::Value array;
   array["ERROR"]=s.str();

   m->setAnswer(array);
    return;
  }
  this->listen();
  disconnected_=0;
  
}

void TdcManager::dolisten()
{
 
       while(true) {

                if(!_group->listen(2000))
		  std::cout << "\nNo msg recieved during the last 2 seconds";
        }

    
}
void TdcManager::listen()
{
  g_store.create_thread(boost::bind(&TdcManager::dolisten, this));

}
void TdcManager::configure(ptt::fsmmessage* m)
{
  //LOG4CXX_INFO(_logLdaq," CMD: "<<m->command());
  std::cout<<m->command()<<std::endl<<m->content()<<std::endl;
  
  std::string config=m->content()["configFile"].asString();
  
  // Read the file
 
  //PRSlow::loadAsics(config,_s1,_s2);
  Json::Reader reader;
  std::ifstream ifs (config.c_str(), std::ifstream::in);
  Json::Value jall;
  bool parsingSuccessful = reader.parse(ifs,jall,false);
  _s1.setJson(jall["ASIC1"]);
  _s1.dumpJson();
  _s2.setJson(jall["ASIC2"]);
  _s2.dumpJson();
 

  _s1.setVthDiscriCharge(900);
  _s2.setVthDiscriCharge(900);
    _s1.setVthTime(900);
  _s2.setVthTime(900);
  _s1.prepare4Tdc(_slcAddr,_slcBuffer);
  _s2.prepare4Tdc(_slcAddr,_slcBuffer,80);
  //s2.prepare4Tdc(adr,val,80);
  _slcBytes=160;
  _slcBuffer[_slcBytes]=0x3;
  _slcAddr[_slcBytes]=0x201;
  _slcBytes++;

  
  //this->parseConfig(config);
  // Now write data
  this->writeRamAvm();
  this->queryCRC();
}
void TdcManager::start(ptt::fsmmessage* m)
{
  //LOG4CXX_INFO(_logLdaq," CMD: "<<m->command());
  std::cout<<m->command()<<std::endl<<m->content()<<std::endl;
  _msh->setReadout(0);
}
void TdcManager::stop(ptt::fsmmessage* m)
{
  //LOG4CXX_INFO(_logLdaq," CMD: "<<m->command());
  std::cout<<m->command()<<std::endl<<m->content()<<std::endl;
  
}
void TdcManager::destroy(ptt::fsmmessage* m)
{
  //LOG4CXX_INFO(_logLdaq," CMD: "<<m->command());
  std::cout<<m->command()<<std::endl<<m->content()<<std::endl;
  delete _group;
  delete _sCtrl;
  delete _sTDC1;
  delete _sTDC2;
  delete _msh;
  
}
void TdcManager::parseConfig(std::string name)
{
  _slcBytes=0;
  memset(_slcBuffer,0,0x1000*sizeof(uint16_t));
  memset(_slcAddr,0,0x1000*sizeof(uint16_t));
  
  FILE *fp=fopen(name.c_str(), "r");
  int ier;
  do
  {
    ier=fscanf(fp,"%x",&_slcBuffer[_slcBytes]);
    _slcAddr[_slcBytes]=_slcBytes;
    _slcBytes++;
  } while (ier!=EOF && _slcBytes<0x1000);
  _slcBuffer[_slcBytes]=0x3;
  _slcAddr[_slcBytes]=0x201;
  _slcBytes++;
  fclose(fp);
}

void TdcManager::writeRamAvm()
{
  if (_slcBuffer[1]<2) return;
  uint16_t sockbuf[0x20000];
  sockbuf[0]=htons(0xFF00);
  sockbuf[1]=htons(_slcBytes);
  int idx=2;
  for (int i=0;i<_slcBytes;i++)
  {
    sockbuf[idx]=htons(_slcAddr[i]);
    sockbuf[idx+1]=htons(_slcBuffer[i]);
    idx+=2;
  }
  // Send the Buffer
  try
  {
    _sCtrl->send((const void*) sockbuf,idx*sizeof(uint16_t));
  }
  catch (NL::Exception e)
  {
    throw e.msg();
  }
  
}
void TdcManager::queryCRC()
{
  _slcBuffer[0]=0x208;
  _slcAddr[0]=0x1;
  _slcBytes=1;
  _slcBuffer[_slcBytes]=0x3;
  _slcAddr[_slcBytes]=0x201;
  _slcBytes++;
  this->writeRamAvm();
  
}
