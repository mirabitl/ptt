#include "PostOffice.hh"
int main()
{
  //std::cout<<ptt::fsmweb::postWeb("http://www.google.fr/")<<std::endl;
  
  char* wp=getenv("PTT_WEBPORT");
  
  if (wp!=NULL)
     PostOffice s("PTT",atoi(wp));
  else
  {
    std::cerr<<" PTT_WEBPORT must be specified starting on port 5555 \n";
     PostOffice s("PTT",5555);
  }
  while (1)
    {
      ::sleep(1);
    }
}
