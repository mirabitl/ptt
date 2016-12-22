#include "PostMessageHandler.hh"
#include "bal.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace ptt;
PostMessageHandler::PostMessageHandler(std::string directory) : _storeDir(directory),_nReceived(0),_bReceived(0)
{ 
  //_bsem.unlock();
}
void PostMessageHandler::processMessage(NL::Socket* socket) throw (std::string)
{
  // build id

  uint64_t id=( (uint64_t) ptt::balAddress::encode_destination((char*) socket->hostTo().c_str(),socket->portTo()));
  //  std::cout<<"Message received from "<<socket->hostTo()<<":"<<socket->portTo()<<" =>"<<std::hex<<id<<std::dec<<std::endl;
  lock();
  // Check
  balMessage b((uint32_t) 0x500000);
  //read code
  size_t ier=0;
  int size_remain=8;
  char* buf=(char*) &b.code();
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&buf[8-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      unlock();
      return;
      
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  
  // Now read address
  size_remain=128;
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&b.dest()[128-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      unlock();
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  
  size_remain=128;
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&b.orig()[128-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      unlock();
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  //printf("Expected length %d \n",b.payloadSize());
  // Now read payload
  size_remain=b.payloadSize();
  while (size_remain>0)
  {
    try 
    {
      ier=socket->read(&b.payload()[b.payloadSize()-size_remain],size_remain);
    }
    catch (NL::Exception e)
    {
      printf("%s Error message when reading block %s \n",__PRETTY_FUNCTION__,e.msg().c_str());
      unlock();
      return;
    }
    if (ier<0)
      break;
    size_remain -=ier;
    
  }
  unlock();
  _nReceived++;
  _bReceived+=b.payloadSize();
  if (_nReceived%1000==0)
    std::cout <<_nReceived<<" "<<_bReceived<<std::endl;
  std::string adr;
  adr.assign(b.dest());
  balAddress ba(adr);
  std::stringstream s,sf;
  s<<_storeDir<<"/PO/BAL/"<<ba.name()<<"_"<<ba.number()<<"/DATA";
  b.write(s.str());
  sf<<_storeDir<<"/PO/BAL/"<<ba.name()<<"_"<<ba.number()<<"/FIFO/input";
  //std::cout<<"Writing in "<<sf.str()<<std::endl;
  int ffi= ::open(sf.str().c_str(), O_WRONLY);
  ::write(ffi,&b.code(),sizeof(uint64_t));
  ::close(ffi);
}
void PostMessageHandler::lock() {_bsem.lock();}
void PostMessageHandler::unlock() {_bsem.unlock();}
void PostMessageHandler::removeSocket(NL::Socket* socket)
{
 
}
