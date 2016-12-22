#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include<stdlib.h> //for exit(0);
#include<stdio.h> //for exit(0);
#include<string.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
#include "bal.hh"
int hostname_to_ip(const char * hostname , char* ip)
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
int main()
{
  char ip[100];
     
  int ier=hostname_to_ip("lyoilcrpi27.in2p3.fr" , ip);
  printf("resolved to %s %d" ,  ip,ier);
     
    printf("\n");
    in_addr_t ls1=inet_addr(ip);
    std::cout<<std::hex<<ls1<<std::dec<<std::endl;
  struct in_addr t;
  t.s_addr=ls1;
  std::cout<<inet_ntoa(t)<<std::endl;


  std::vector<std::string> results;
  boost::split(results,"134.158.81.142:5555:DIFSERVER:1", boost::is_any_of(":"));
  for (int i=0;i<results.size();i++)
    std::cout<<results[i]<<std::endl;
  ier=hostname_to_ip(results[0].c_str() , ip);
  uint64_t port=  atol(results[1].c_str());
  printf("resolved to %s -> %llx  %d\n" ,  ip,(int64_t) ((uint64_t) inet_addr(ip)| (port<<32)) ,ier);
  Json::Value _jval;
      _jval.clear();
      _jval["host"]="lyoilcrpi18.in2p3.fr";
      _jval["port"]=10003;
      _jval["name"]="DIFSERVER";
      _jval["number"]=245;
   Json::FastWriter f;std::cout<<f.write(_jval)<<" "<<f.write(_jval).size()<<std::endl;
   std::string ss="{\"host\":\"lyoilcrpi19.ipnlmail.in2p3.fr\",\"name\":\"SDDCANDDIFSERVER\",\"number\":24500,\"port\":100003}";
   wemove::balAddress b(ss);
   std::cout<<b.host()<<" "<<b.name()<<" "<<b.port()<<" "<<b.number()<<" "<<b.address().length()<<std::endl;
}
