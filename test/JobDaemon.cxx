#include "fsmjob.hh"
int main()
{
  //std::cout<<ptt::fsmweb::postWeb("http://www.google.fr/")<<std::endl;
  ptt::fsmjob s("JOB",9999);
  while (1)
    {
      ::sleep(1);
    }
}
