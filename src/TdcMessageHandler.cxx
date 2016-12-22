#include "TdcMessageHandler.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace ptt;
TdcMessageHandler::TdcMessageHandler(std::string directory) : _storeDir(directory),_readout(0)
{
  _sockMap.clear();
}
void TdcMessageHandler::processMessage(NL::Socket* socket) throw (std::string)
{
  // build id

  uint64_t id=( (uint64_t) ptt::TdcMessageHandler::convertIP(socket->hostTo())<<32)|socket->portTo();
  std::cout<<"Message received from "<<socket->hostTo()<<":"<<socket->portTo()<<" =>"<<std::hex<<id<<std::dec<<std::endl;
  std::map<uint64_t, ptrBuf>::iterator itsock=_sockMap.find(id);
  if (itsock==_sockMap.end())
  {
    ptrBuf p(0,new unsigned char[0x100000]);
    std::pair<uint64_t, ptrBuf> p1(id,p);
    _sockMap.insert(p1);
    itsock=_sockMap.find(id);
    // Build subdir
    struct stat st = {0};
    std::stringstream s;
    s<<_storeDir<<"/"<<socket->hostTo();//<<"/"<<socket->portTo();
    std::cout<<"Creating "<<s.str()<<std::endl;
    if (stat(s.str().c_str(), &st) == -1) {
      mkdir(s.str().c_str(), 0700);
    }
    s<<"/"<<socket->portTo();
    std::cout<<"Creating "<<s.str()<<std::endl;
    if (stat(s.str().c_str(), &st) == -1) {
      mkdir(s.str().c_str(), 0700);
    }
    // std::stringstream sf;
    // sf<<_storeDir<<"/"<<socket->hostTo()<<"/"<<socket->portTo()<<"/events";
    // _fd= ::open(s.str().c_str(),O_CREAT| O_RDWR | O_NONBLOCK,S_IRWXU);
    // if (_fd<0)
    //   {
    
    // 	//LOG4CXX_FATAL(_logShm," Cannot open shm file "<<s.str());
    // 	perror("No way to store to file :");
    // //std::cout<<" No way to store to file"<<std::endl;
    // 	return;
    //   }

  }
  ptrBuf &p=itsock->second;
  uint32_t* iptr=(uint32_t*) &p.second[0];
  // Check
  size_t ier=0;
  uint32_t size_remain=4;
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&p.second[4-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  p.first+=4;
  if (ntohl(iptr[0]) !=0XCAFEBABE)
  {
    printf("%s Invalid frame header %x  from %lx \n",__PRETTY_FUNCTION__,ntohl(iptr[0]),id);
    return;
  }
  // Now read length
  size_remain=4;
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&p.second[p.first+4-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  p.first+=4;
  uint32_t length=(ntohl(iptr[1])>>16)&0xFFFF;
  printf("Expected length %d \n",length);
  // Now read payload
  size_remain=length;
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&p.second[p.first+length-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  p.first+=length;
  _readout++;
  // Now store the event
#ifdef OLDWAY
  std::stringstream s;
  s<<_storeDir<<"/"<<socket->hostTo()<<"/"<<socket->portTo()<<"/event_"<<_readout;
  int fd= ::open(s.str().c_str(),O_CREAT| O_RDWR | O_NONBLOCK,S_IRWXU);
  if (fd<0)
  {
    
    //LOG4CXX_FATAL(_logShm," Cannot open shm file "<<s.str());
    perror("No way to store to file :");
    //std::cout<<" No way to store to file"<<std::endl;
    return;
  }
  ier=write(fd,p.second,p.first);
  if (ier!=p.first) 
  {
    std::cout<<"pb in write "<<ier<<std::endl;
    return;
  }
  ::close(fd);
#else
  unsigned char bup[0x10000];
  memcpy(bup,p.second,p.first);
  // uint32_t* lbuf=(uint32_t*) p.second;
  // uint16_t* sbuf=(uint16_t*) p.second;
  // for (int i=0;i<p.first/4;i++)
  //   {
  //    uint32_t ll=ntohl(lbuf[i]);
  //    //lbuf[i]=ntohl(lbuf[i]);
  //    // printf("%x ntohl %x \n",lbuf[i],ll) ;     
  //   }
  // //getchar();
  // for (int i=0;i<p.first/2;i++)
  //   {
  //    uint16_t ll=ntohs(sbuf[i]);
  //    //lbuf[i]=ntohl(lbuf[i]);
  //    //printf("%x ntohl %x \n",sbuf[i],ll);      
  //   }
  // //getchar();
  // unsigned char* hbuf=(unsigned char*) &sbuf[4];
  std::stringstream s;
  s<<_storeDir<<"/"<<socket->hostTo()<<"/"<<socket->portTo()<<"/events";
  FILE * fp = fopen (s.str().c_str(), "a+");
  for (int i=0;i<p.first;)
    {
     
      uint64_t fr[8];
      for (int j=0;j<8;j++)
	fr[j]=bup[i+8+j]&0XFF;
      
      
      // it=fr[3]|(fr[4]<<8)|(fr[5]<<16)|(fr[6]<<24)|(fr[7]<<32);
      // double rti=it*2.5/256;
      // fprintf(fp,"%d,%llx,%f\n",ch,it,rti);
      // printf("%x,%d,%d,%llx,%f,%llx\n",fr[2],i,ch,it,rti,(fr[7]<<32));
      uint8_t ch=(fr[2]&0XFF)/2;
      uint64_t it=fr[7]|(fr[6]<<8)|(fr[5]<<16)|(fr[4]<<24)|(fr[3]<<32);
      double rti=it*2.5/256;
      //printf("%x,%d,%d,%llx,%f,%llx\n",fr[2],i,ch,it,rti,(fr[7]<<32));
      fprintf(fp,"%d,%llx,%f\n",ch,it,rti);
      i+=8;
    }
  //getchar();
  fclose(fp);
#endif
  p.first=0;
}
void TdcMessageHandler::removeSocket(NL::Socket* socket)
{
  uint64_t id=((uint64_t) ptt::TdcMessageHandler::convertIP(socket->hostTo())<<32)|socket->portTo();
  std::map<uint64_t, ptrBuf>::iterator itsock=_sockMap.find(id);
  if (itsock==_sockMap.end()) return;
  delete itsock->second.second;
  _sockMap.erase(itsock);
}
uint32_t TdcMessageHandler::convertIP(std::string hname)
{
  struct hostent *he;
  struct in_addr **addr_list;
  int i;
  char ip[100];
  if ( (he = gethostbyname( hname.c_str() ) ) == NULL) 
  {
    return 0;
  }
  
  addr_list = (struct in_addr **) he->h_addr_list;
  
  for(i = 0; addr_list[i] != NULL; i++) 
  {
    //Return the first one;
    strcpy(ip , inet_ntoa(*addr_list[i]) );
    break;
  }
  
  in_addr_t ls1=inet_addr(ip);
  return (uint32_t) ls1;
}
