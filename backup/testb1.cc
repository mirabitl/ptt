#include "PRSlow.hh"
int main()
{
  PRSlow s;
  // std::bitset<32> a(0xDE);
  // //std::cout<<a<<std::endl;
  // s.setBit(13);
  
  // s.setByte(59,0xDE,8);
  // s.setByte(67,0xAD,8);
  // s.Print();
  // std::cout<<std::hex<<(int) s.getByte(59)<<std::endl;
  // std::cout<<std::hex<<(int) s.getByte(67)<<std::endl;
  

  //s.loadConfig("./PR1.txt");
  // for (int ch=0;ch<32;ch++)
  //   {
  //     s.setInputDacCommand(ch,1);
  //     s.setMaskDiscriTime(ch,1);
  //     s.set6bDac(ch,37);
  //   }
  //s.Print();

  PRSlow s1,s2;
  Json::Reader reader;
  //bool parsingSuccessful = reader.parse(fname,_jasic);
  
  std::ifstream ifs ("PRall.json", std::ifstream::in);
  Json::Value jall;
  bool parsingSuccessful = reader.parse(ifs,jall,false);
  s2.setJson(jall["ASIC2"]);
  s2.dumpJson();
  
//   PRSlow::loadAsics("./slow_control_calib.csv",s1,s2);
//   s2.toJson();
//   s2.dumpJson();
//   // s1.Print();
//   // getchar();
//   // s2.Print();
// 
//   //s1.dumpJson();
//   //getchar();
//     
//   //uint16_t adr[160],val[160];
//   //s1.prepare4Tdc(adr,val);
//   //s2.prepare4Tdc(adr,val,80);
//   s1.loadJson("./PR1.json");
//   //getchar();
//   s1.setFromJson();
//   s1.setInputDac(3,125);
//   
//   s1.toJson();
  //s1.dumpJson();
  //getchar();
}
