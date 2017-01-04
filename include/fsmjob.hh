#ifndef _FSMJOB_HH
#define _FSMJOB_HH
#include "fsmweb.hh"
#include <iostream>
#include <sstream>
// -- std headers
#include <stdint.h>
#include <sys/types.h>
#include <map>
#include <string>

// -- json headers
#include "json/json.h"

namespace ptt
{
/**
 *  @brief  DimProcessData class
 */
class processData
{
public:
	/**
	 *  @brief  Status enum
	 */
	enum Status
	{
		NOT_CREATED = 0,
		RUNNING = 1,
		KILLED = 2
	};

	/**
	 *  @brief  Constructor. Construct a process structure from a json string
	 *          that contains all needed infos on a process
	 *
	 *  @param  jsonString the jsonString to initialize the process
	 */
	processData(const std::string &jsonString);

	Json::Value    m_processInfo;     ///< The json process value
	pid_t          m_childPid;        ///< The process pid
        Status         m_status;          ///< The process status
        std::string m_string;
};

class fsmjob {
public:
  fsmjob(std::string name,uint32_t port);
  void initialise(ptt::fsmmessage* m);
  void start(ptt::fsmmessage* m);
  void kill(ptt::fsmmessage* m);
  void destroy(ptt::fsmmessage* m);
  void registration(ptt::fsmmessage* m);
  void registerjob(ptt::fsmmessage* m);
  void endregistration(ptt::fsmmessage* m);
  void status(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void joblog(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void killjob(Mongoose::Request &request, Mongoose::JsonResponse &response);
  void restartjob(Mongoose::Request &request, Mongoose::JsonResponse &response);

  void registerfile(Mongoose::Request &request, Mongoose::JsonResponse &response);
protected:
  void startProcess(ptt::processData* pd);
  void killProcess(uint32_t pid,uint32_t sig=9);
  Json::Value jsonStatus();
private:
  fsmweb* _fsm;
  typedef std::map<pid_t,ptt::processData*> PidToProcessMap;
  Json::Value m_jfile,m_jconf;
  std::string        m_hostname;         ///< The host name on which the job control is running
  std::string s_conf;
  uint32_t m_port;
  PidToProcessMap    m_processMap;       ///< The handled process map
};


}
#endif
