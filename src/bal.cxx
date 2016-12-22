#include "bal.hh"

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
using namespace ptt;
using namespace std;

ptt::bal::bal(std::string base,std::string name,uint16_t num,balFunctor handler) : _base(base),_name(name),_num(num), _handler(handler),_active(false)
{
  // Create the directory
  struct stat st = {0};

  stringstream s;
  stringstream sfifo;
  s<<base;
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/PO";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/BAL";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"/"<<name;
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  s<<"_"<<num;
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  sfifo<<s.str();
  s<<"/DATA";
  if (stat(s.str().c_str(), &st) == -1) {mkdir(s.str().c_str(), 0700);  }
  _dataDir=s.str();
  sfifo<<"/FIFO";
  if (stat(sfifo.str().c_str(), &st) == -1) {mkdir(sfifo.str().c_str(), 0700);  }
  _fifoDir=sfifo.str();
  // Create fifos
  //this->initFifo();
  
}

void ptt::bal::initFifo()
{
  stringstream name;
  name<<_fifoDir<<"/input";
  struct stat st;
  if (stat(name.str().c_str(), &st) != 0)
    mkfifo(name.str().c_str(), 0666);
  std::cout<<"Oppening "<<name.str()<<std::endl;
  _fd = open(name.str().c_str(), O_RDONLY);
    if (_fd == -1) {
        perror("open");
        return;
    }
    std::cout<<"initfifo done"<<std::endl;
  // FD_ZERO(&_fifoSet);
  // FD_SET(_fd, &_fifoSet);
    
}

void ptt::bal::start()
{
  _active=true;
g_store.create_thread(boost::bind(&ptt::bal::doCheck, this));
  
}
void ptt::bal::stop()
{
  _active=false;
  
}

void ptt::bal::doCheck()
{
  int n;
  uint64_t code;
  
  
  char buf[1024];
  uint64_t * ibuf=(uint64_t*) buf;
  this->initFifo();
  while (1)
  {
    //std::cout<<"reading fifo \n";

    int bytes = read(_fd, buf, 8);
    if (bytes>0)
    {
      // printf("done reading (%lu bytes) %lx\n", bytes,ibuf[0]);
      if (bytes==8)
    {
      code=ibuf[0];
        if (code!=0 )
        {
          this->_handler(code);
          
        }
      }
    
    }
    else
      ::usleep(100);
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
            
            
            
            int ier=0;
            int size_remain=8;
            code=0;
            
            while (size_remain>0)
            {
              printf("before read \n");
              
              ier=read(_fd,&buf[8-size_remain],size_remain);
              printf("after read \n");
              
              if (ier<0)
                break;
              size_remain -=ier;
              printf(" ier %d remain %d code %lx \n",ier,size_remain,code);
            }    
            printf(" End ier %d remain %d code %lx \n",ier,size_remain,code);
            if (code!=0) this->_handler(code);
            
          
            
      }
      */
    

}
extern  int alphasort(); 
int file_select_bal(const struct direct *entry)  
{  
  if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))  
    return (0);  
  else  
    return (1);  
}  										      
void ptt::bal::post(balMessage &m)
{
  std::stringstream s;
  s<<_base<<"/PO/OUTPUT/DATA";
  int count=0;
  bool pause=true;
  while (pause)
  {
    
    struct direct **files;  
    count = scandir(s.str().c_str(), &files, file_select_bal, alphasort);  
    //    std::cout <<count<<" files "<<std::endl;
    for (int i=1; i<count+1; ++i)  
    {     
      free(files[i-1]);
    }
    free(files);
    
    if (count>200) pause=true;
    ::usleep(200);
    if (count<100) pause=false;
    
  } 
  
  char hname[1024]; 
  int ier=gethostname(hname,1024);
  std::string shn;
  shn.assign(hname);
  Json::Value j;
  j["host"]=shn;
  j["port"]=6666;
  j["name"]=_name;
  j["number"]=_num;
  balAddress ma(j);
  m.setOrigin(ma);
  
  
  
  
  std::stringstream sd;     
  //printf("Number of files = %d\n",count);
  Json::Value array;

  
  
  m.write(s.str());
  
  //std::cout<<"data writen"<<std::endl;
  std::stringstream sf;
  sf<<_base<<"/PO/OUTPUT/yellowbox";

  int ffi= ::open(sf.str().c_str(), O_WRONLY);
  
  //std::cout<<"fifo open"<<sf.str()<<" = "<<ffi<<std::endl;
  
  ::write(ffi,&m.code(),sizeof(uint64_t));
  ::close(ffi);
  std::cout<<"done posting " <<std::hex<<m.code()<<std::dec<<std::endl;
}
ptt::balMessage::balMessage(uint32_t size) : _allocate(true)
{
  _buf = new char[size];
  _code= size&0xFFFFFFF;
}
ptt::balMessage::balMessage(std::string address,uint32_t lenpay,char* payload) : _allocate(false)
{
  memcpy(_dest,address.c_str(),address.size());
  _buf=payload;
  encode(0,ptt::balMessage::NORMAL);
  setPayloadSize(lenpay);
  
}
ptt::balMessage::~balMessage()
{
  if (_allocate) delete _buf;
}
int ptt::balMessage::write(std::string path)
{
  std::stringstream sc,sd;
  sc.str(std::string());
  sc<<path<<"/_"<<std::hex<<_code<<std::dec;
  
  int fd= ::open(sc.str().c_str(),O_CREAT| O_RDWR | O_NONBLOCK,S_IRWXU);
  if (fd<0)
  {
    
    //LOG4CXX_FATAL(_logShm," Cannot open shm file "<<s.str());
    printf("No way to store to file : %s\n",sc.str().c_str());
    //std::cout<<" No way to store to file"<<std::endl;
    return -1;
  }
  int ier=::write(fd,&_code,sizeof(uint64_t));
  ier+=::write(fd,_dest,128);
  ier+=::write(fd,_orig,128);
  ier+=::write(fd,_buf,payloadSize());
  
  ::close(fd);
  return ier;
}
int ptt::balMessage::read(std::string path,uint64_t code)
{
  std::stringstream sc,sd;
  sc.str(std::string());
  sc<<path<<"/_"<<std::hex<<code<<std::dec;
  
  int fd= ::open(sc.str().c_str(),O_RDONLY);
  if (fd<0)
  {
    
    //LOG4CXX_FATAL(_logShm," Cannot open shm file "<<s.str());
    printf("No way to  open file : %s\n",sc.str().c_str());
    //std::cout<<" No way to store to file"<<std::endl;
    return -1;
  }
  int ier=::read(fd,&_code,sizeof(uint64_t));
  ier+=::read(fd,_dest,128);
  ier+=::read(fd,_orig,128);
  //std::cout<<"reading "<<std::hex<<_code<<std::dec<<" size" <<payloadSize()<<std::endl;
  ier+=::read(fd,_buf,payloadSize());
  
  ::close(fd);
  //std::cout<<"removing "<<sc.str()<<" "<<_code<<" "<<_dest<<" "<<ier<<std::endl;
  ::unlink(sc.str().c_str());
  return ier;
}
uint64_t& ptt::balMessage::code(){return _code;}
char* ptt::balMessage::dest()  { return (char*) _dest;}
char* ptt::balMessage::orig()  { return (char*) _orig;}
uint32_t ptt::balMessage::length(uint64_t code){return (uint32_t) (code & 0XFFFFFFF);}
uint8_t ptt::balMessage::type(uint64_t code){return (uint8_t) ((code>>28) & 0XF);}
uint32_t ptt::balMessage::id(uint64_t code){return (uint32_t) ((code>>32) & 0XFFFFFFFF);}
char* ptt::balMessage::payload(){ return _buf;}
void ptt::balMessage::encode(uint32_t id,uint8_t mtype) { _code =(_code& ~0xFFFFFFFF00000000)|(((uint64_t )id)<<32); 
  _code =(_code& ~((uint64_t) 0xF0000000))|((uint64_t)(mtype&0xF))<<28;}
  uint32_t ptt::balMessage::payloadSize(){ return balMessage::length(_code)-sizeof(uint64_t)-128;}
  void ptt::balMessage::setPayloadSize(uint32_t len){ _code=(_code & ~0xFFFFFFF)| balMessage::length(len);}
  
  void ptt::balMessage::setDestination(balAddress& ad){std::string sad=ad.address();memcpy(_dest,sad.c_str(),sad.length());}
  void ptt::balMessage::setOrigin(balAddress& ad){std::string sad=ad.address();memcpy(_orig,sad.c_str(),sad.length());}
  balAddress ptt::balMessage::origin(){balAddress m(_orig); return m;}
  balAddress ptt::balMessage::destination(){balAddress m(_dest); return m;}
  
  
  ptt::balAddress::balAddress(Json::Value &j)
  {
    _jval.clear();
    _jval=j;
  }
  
  ptt::balAddress::balAddress(std::string h,uint32_t p,std::string name,uint32_t num) 
  {
    _jval.clear();
    _jval["host"]=h;
    _jval["port"]=p;
    _jval["name"]=name;
    _jval["number"]=num;
  }
  ptt::balAddress::balAddress(std::string r)
  {
    Json::Reader reader;
    _jval.clear();
    bool parsingSuccessful = reader.parse(r.c_str(), _jval );     //parse process
    if ( !parsingSuccessful )
    {
      std::cout  << "Failed to parse "<<r<<std::endl;
    }
  }
  ptt::balAddress::balAddress(char* r)
  {
    Json::Reader reader;
    _jval.clear();
    bool parsingSuccessful = reader.parse((const char*) r, _jval );     //parse process
    if ( !parsingSuccessful )
    {
      std::cout  << "Failed to parse "<<r<<std::endl;
    }
  }
  ptt::balAddress::balAddress(ptt::balMessage r)
  {
    Json::Reader reader;
    _jval.clear();
    bool parsingSuccessful = reader.parse(r.dest(), _jval );     //parse process
    if ( !parsingSuccessful )
    {
      std::cout  << "Failed to parse "<<r.dest()<<std::endl;
    }
  }
  std::string ptt::balAddress::address(){Json::FastWriter f;return f.write(_jval);}
  std::string ptt::balAddress::host(){return _jval["host"].asString();}
  std::string ptt::balAddress::name(){return _jval["name"].asString();}
  uint32_t ptt::balAddress::port(){return _jval["port"].asUInt();}
  uint32_t ptt::balAddress::number(){return _jval["number"].asUInt();}
   int ptt::balAddress::hostname_to_ip(const char * hostname , char* ip)
  {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
      // get the host info
      herror("gethostbyname");
      return 1;
    }
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++) 
    {
      //Return the first one;
      strcpy(ip , inet_ntoa(*addr_list[i]) );
      return 0;
    }
    
    return 1;
  }
   uint64_t ptt::balAddress::encode_po(const char* host,uint32_t port)
  {
    char ip[100];
    hostname_to_ip(host,ip);
    return encode_destination(ip,port);
    
  }
   uint64_t ptt::balAddress::encode_destination(char* ip,uint32_t port)
  {
    in_addr_t ls1=inet_addr(ip);
    uint64_t p=((uint64_t) port<<32);
    p|=(ls1);
    //printf("%x %lx \n",ls1,p);
    return p;
  }
  uint64_t ptt::balAddress::socketId(){return ptt::balAddress::encode_po((char*) host().c_str(),port());}