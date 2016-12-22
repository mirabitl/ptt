#ifndef _TDC_MESSAGE_HANDLER_HH
#define _TDC_MESSAGE_HANDLER_HH
#include "MessageHandler.hh"
#include <map>
#include <string>
typedef std::pair<uint32_t,unsigned char*> ptrBuf;
namespace ptt
{
  class TdcMessageHandler : public ptt::MessageHandler
  {
  public:
    TdcMessageHandler(std::string directory);
    virtual void processMessage(NL::Socket* socket) throw (std::string);
    virtual void removeSocket(NL::Socket* sock);
    static uint32_t convertIP(std::string hname);
    uint32_t readout(){return _readout;}
    void setReadout(uint32_t e){_readout=e;}
  private:
    std::string _storeDir;
    std::map<uint64_t, ptrBuf> _sockMap;
    uint32_t _readout;
  };
};

#endif