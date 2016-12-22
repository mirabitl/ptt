#include "PostOffice.hh"
int main()
{
  //std::cout<<ptt::fsmweb::postWeb("http://www.google.fr/")<<std::endl;
  PostOffice s("LMDAQ",5555);
  while (1)
    {
      ::sleep(1);
    }
}
