#ifndef _POST_MESSAGE_HANDLER_HH
#define _POST_MESSAGE_HANDLER_HH
#include "MessageHandler.hh"
#include <map>
#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace ptt
{
  class PostMessageHandler : public ptt::MessageHandler
  {
  public:
    PostMessageHandler(std::string directory);
    virtual void processMessage(NL::Socket* socket) throw (std::string);
    virtual void removeSocket(NL::Socket* sock);
    void lock();
    void unlock();
  private:
    std::string _storeDir;
    uint64_t _nReceived,_bReceived;
    boost::interprocess::interprocess_mutex _bsem;
    
  };
};

#endif
