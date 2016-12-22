#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;

class PRSlow
{
public:
  PRSlow(){ memset(_l,0,20*sizeof(uint32_t));}
  bool getBit(int b){return (_l[b/32]>>(b%32))&1;}
  void setBit(int b){ _l[b/32]|=(1<<(b%32));};
  void clearBit(int b){ _l[b/32]&= ~(1<<(b%32));};
  void setBitState(int b,bool t){if (t) setBit(b); else clearBit(b);}
  uint8_t getByte(int b){return
      (getBit(b)|getBit(b+1)<<1|getBit(b+2)<<2|getBit(b+3)<<3|getBit(b+4)<<4|getBit(b+5)<<5|getBit(b+6)<<6|getBit(b+7)<<7);
      }
  void setByte(int b,uint8_t val,uint8_t len)
  {
    //unsigned char* p=&_l[b/32];
    for(int i=0;i<len;i++)
      if ((val>>i)&1)
	setBit(b+i);
      else
	clearBit(b+i);
  }
  void Print()
  {
    /*
    for (int i=0;i<20;i++)
      {
	std::cout<<std::hex<<_l[i]<<std::dec<<std::endl;
	std::bitset<32> a(_l[i]);
	std::cout<<a<<std::endl;
      }
    */

  std::cout<<"EN10bDac ="<<getEN10bDac()<<std::endl;
  std::cout<<"PP10bDac ="<<getPP10bDac()<<std::endl;
  std::cout<<"EN_adc ="<<getEN_adc()<<std::endl;
  std::cout<<"PP_adc ="<<getPP_adc()<<std::endl;
  std::cout<<"sel_starb_ramp_adc_ext ="<<getsel_starb_ramp_adc_ext()<<std::endl;
  std::cout<<"usebcompensation ="<<getusebcompensation()<<std::endl;
  std::cout<<"EN_bias_dac_delay ="<<getEN_bias_dac_delay()<<std::endl;
  std::cout<<"PP_bias_dac_delay ="<<getPP_bias_dac_delay()<<std::endl;
  std::cout<<"EN_bias_ramp_delay ="<<getEN_bias_ramp_delay()<<std::endl;
  std::cout<<"PP_bias_ramp_delay ="<<getPP_bias_ramp_delay()<<std::endl;
  std::cout<<"EN_discri_delay ="<<getEN_discri_delay()<<std::endl;
  std::cout<<"PP_discri_delay ="<<getPP_discri_delay()<<std::endl;
  std::cout<<"EN_temp_sensor ="<<getEN_temp_sensor()<<std::endl;
  std::cout<<"PP_temp_sensor ="<<getPP_temp_sensor()<<std::endl;
  std::cout<<"EN_bias_pa ="<<getEN_bias_pa()<<std::endl;
  std::cout<<"PP_bias_pa ="<<getPP_bias_pa()<<std::endl;
  std::cout<<"EN_bias_discri ="<<getEN_bias_discri()<<std::endl;
  std::cout<<"PP_bias_discri ="<<getPP_bias_discri()<<std::endl;
  std::cout<<"cmd_polarity ="<<getcmd_polarity()<<std::endl;
  std::cout<<"latch ="<<getlatch()<<std::endl;
  std::cout<<"EN_bias_6bit_dac ="<<getEN_bias_6bit_dac()<<std::endl;
  std::cout<<"PP_bias_6bit_dac ="<<getPP_bias_6bit_dac()<<std::endl;
  std::cout<<"EN_bias_tdc ="<<getEN_bias_tdc()<<std::endl;
  std::cout<<"PP_bias_tdc ="<<getPP_bias_tdc()<<std::endl;
  std::cout<<"ON_OFF_input_dac ="<<getON_OFF_input_dac()<<std::endl;
  std::cout<<"EN_bias_charge ="<<getEN_bias_charge()<<std::endl;
  std::cout<<"PP_bias_charge ="<<getPP_bias_charge()<<std::endl;
  std::cout<<"Cf3_100fF ="<<getCf3_100fF()<<std::endl;
  std::cout<<"Cf2_200fF ="<<getCf2_200fF()<<std::endl;
  std::cout<<"Cf1_2p5pF ="<<getCf1_2p5pF()<<std::endl;
  std::cout<<"Cf0_1p25pF ="<<getCf0_1p25pF()<<std::endl;
  std::cout<<"EN_bias_sca ="<<getEN_bias_sca()<<std::endl;
  std::cout<<"PP_bias_sca ="<<getPP_bias_sca()<<std::endl;
  std::cout<<"EN_bias_discri_charge ="<<getEN_bias_discri_charge()<<std::endl;
  std::cout<<"PP_bias_discri_charge ="<<getPP_bias_discri_charge()<<std::endl;
  std::cout<<"EN_bias_discri_adc_time ="<<getEN_bias_discri_adc_time()<<std::endl;
  std::cout<<"PP_bias_discri_adc_time ="<<getPP_bias_discri_adc_time()<<std::endl;
  std::cout<<"EN_bias_discri_adc_charge ="<<getEN_bias_discri_adc_charge()<<std::endl;
  std::cout<<"PP_bias_discri_adc_charge ="<<getPP_bias_discri_adc_charge()<<std::endl;
  std::cout<<"DIS_razchn_int ="<<getDIS_razchn_int()<<std::endl;
  std::cout<<"DIS_razchn_ext ="<<getDIS_razchn_ext()<<std::endl;
  std::cout<<"SEL_80M ="<<getSEL_80M()<<std::endl;
  std::cout<<"EN_80M ="<<getEN_80M()<<std::endl;
  std::cout<<"EN_slow_lvds_rec ="<<getEN_slow_lvds_rec()<<std::endl;
  std::cout<<"PP_slow_lvds_rec ="<<getPP_slow_lvds_rec()<<std::endl;
  std::cout<<"EN_fast_lvds_rec ="<<getEN_fast_lvds_rec()<<std::endl;
  std::cout<<"PP_fast_lvds_rec ="<<getPP_fast_lvds_rec()<<std::endl;
  std::cout<<"EN_transmitter ="<<getEN_transmitter()<<std::endl;
  std::cout<<"PP_transmitter ="<<getPP_transmitter()<<std::endl;
  std::cout<<"ON_OFF_1mA ="<<getON_OFF_1mA()<<std::endl;
  std::cout<<"ON_OFF_2mA ="<<getON_OFF_2mA()<<std::endl;
  std::cout<<"ON_OFF_otaQ ="<<getON_OFF_otaQ()<<std::endl;
  std::cout<<"ON_OFF_ota_mux ="<<getON_OFF_ota_mux()<<std::endl;
  std::cout<<"ON_OFF_ota_probe ="<<getON_OFF_ota_probe()<<std::endl;
  std::cout<<"DIS_trig_mux ="<<getDIS_trig_mux()<<std::endl;
  std::cout<<"EN_NOR32_time ="<<getEN_NOR32_time()<<std::endl;
  std::cout<<"EN_NOR32_charge ="<<getEN_NOR32_charge()<<std::endl;
  std::cout<<"DIS_triggers ="<<getDIS_triggers()<<std::endl;
  std::cout<<"EN_dout_oc ="<<getEN_dout_oc()<<std::endl;
  std::cout<<"EN_transmit ="<<getEN_transmit()<<std::endl;
  std::cout<<"DacDelay ="<<getDacDelay()<<std::endl;		
  for (int ch=0;ch<31;ch++)
      std::cout<<"InputDac["<<ch<<"]="<<(int) getInputDac(ch)<<std::endl;
  for (int ch=0;ch<31;ch++)
      std::cout<<"6bDac["<<ch<<"]="<<(int) get6bDac(ch)<<std::endl;
  for (int ch=0;ch<31;ch++)
      std::cout<<"MaskDiscriCharge["<<ch<<"]="<<(int) getMaskDiscriCharge(ch)<<std::endl;
  for (int ch=0;ch<31;ch++)
      std::cout<<"MaskDiscriTime["<<ch<<"]="<<(int) getMaskDiscriTime(ch)<<std::endl;
  for (int ch=0;ch<31;ch++)
      std::cout<<"InputDacCommand["<<ch<<"]="<<(int) getInputDacCommand(ch)<<std::endl;

  std::cout<<"VthDiscriCharge ="<<getVthDiscriCharge()<<std::endl;
  std::cout<<"VthTime ="<<getVthTime()<<std::endl;			

  
  for (int i=0;i<20;i++)
    printf("%d %x \n",i,_l[i]);
  /* getchar();
  */


    
  }
  static void loadAsics(std::string fname,PRSlow& s1, PRSlow& s2)
  {
    FILE *fp=fopen(fname.c_str(), "r");
    int ier;
    uint32_t nl=0;
    do
      {
	uint32_t n1,v1,n2,v2;
	ier=fscanf(fp,"%d,%d,%d,%d\n",&n1,&v1,&n2,&v2);
	printf("%d %d -> %d %d \n",ier,nl,n1,v1); 
	s1.setBitState(n1,v1);
	s2.setBitState(n2,v2);
	nl++;
      } while (ier!=EOF);
    fclose(fp);
    //loadInvert(s);
    //Print();
    getchar();

  }
  void loadConfig(std::string fname)
  {
    PRSlow s;
    uint8_t* buf=(uint8_t*) ptr();
    uint8_t slcBytes=0;
    FILE *fp=fopen(fname.c_str(), "r");
    int ier;
    do
      {
	char b;
	ier=fscanf(fp,"%x",&b);
	memcpy(&buf[79-slcBytes],&b,1);
	printf("%d %x %x\n",79-slcBytes,buf[79-slcBytes],_l[(79-slcBytes)/4]);
	slcBytes++;
      } while (ier!=EOF && slcBytes<80);
    fclose(fp);
    //loadInvert(s);
    //Print();
    //getchar();

  }
  uint32_t* ptr(){return _l;}
  // Now real access to register function
  bool getMaskDiscriCharge(int b){return getBit(b%32);}
  void setMaskDiscriCharge(int b,bool on)  { setBitState(b,on);}
  uint8_t getInputDac(uint8_t ch){return getByte(32+ch*9);}
  void setInputDac(uint8_t ch,uint8_t val){setByte(32+ch*9,val,8);}
  bool getInputDacCommand(int ch){return getBit(32+ch*9+8);}
  void setInputDacCommand(int ch,bool on)  { setBitState(32+ch*9+8,on);}

  bool getMaskDiscriTime(int b){return getBit(328+b);}
  void setMaskDiscriTime(int b,bool on)  { setBitState(328+b,on);}
  uint8_t get6bDac(uint8_t ch){return getByte(360+ch*6)&0x3F;}
  void set6bDac(uint8_t ch,uint8_t val){setByte(360+ch*6,val&0x3F,6);}
  bool getEN10bDac(){return getBit(552);}
  void setEN10bDac(bool on)  { setBitState(552,on);}
  bool getPP10bDac(){return getBit(553);}
  void setPP10bDac(bool on)  { setBitState(553,on);}

  uint16_t getVthDiscriCharge()
  {
    uint16_t r=0;
    for (int i=1;i<=10;i++)
      if (getBit(554+i-1)) r|=(1<<(10-i));
    return r;
  }
  void setVthDiscriCharge(uint16_t val)
  {
    uint16_t r=val&0x3FF;
    for (int i=1;i<=10;i++)
      setBitState(554+i-1,r&(1<<(10-i)));

  }
  uint16_t getVthTime()
  {
    uint16_t r=0;
    for (int i=1;i<=10;i++)
      if (getBit(564+i-1)) r|=(1<<(10-i));
    return r;
  }
  void setVthTime(uint16_t val)
  {
    uint16_t r=val&0x3FF;
    for (int i=1;i<=10;i++)
      setBitState(564+i-1,r&(1<<(10-i)));
   
  }

  bool getDacDelay(){return getByte(582);}
  void setDacDelay(uint8_t val){setByte(582,val,8);}
  void setEN_adc(bool t){setBitState(574,t);}
  void setPP_adc(bool t){setBitState(575,t);}
  void setsel_starb_ramp_adc_ext(bool t){setBitState(576,t);}
  void setusebcompensation(bool t){setBitState(577,t);}
  void setEN_bias_dac_delay(bool t){setBitState(578,t);}
  void setPP_bias_dac_delay(bool t){setBitState(579,t);}
  void setEN_bias_ramp_delay(bool t){setBitState(580,t);}
  void setPP_bias_ramp_delay(bool t){setBitState(581,t);}
  void setEN_discri_delay(bool t){setBitState(590,t);}
  void setPP_discri_delay(bool t){setBitState(591,t);}
  void setEN_temp_sensor(bool t){setBitState(592,t);}
  void setPP_temp_sensor(bool t){setBitState(593,t);}
  void setEN_bias_pa(bool t){setBitState(594,t);}
  void setPP_bias_pa(bool t){setBitState(595,t);}
  void setEN_bias_discri(bool t){setBitState(596,t);}
  void setPP_bias_discri(bool t){setBitState(597,t);}
  void setcmd_polarity(bool t){setBitState(598,t);}
  void setlatch(bool t){setBitState(599,t);}
  void setEN_bias_6bit_dac(bool t){setBitState(600,t);}
  void setPP_bias_6bit_dac(bool t){setBitState(601,t);}
  void setEN_bias_tdc(bool t){setBitState(602,t);}
  void setPP_bias_tdc(bool t){setBitState(603,t);}
  void setON_OFF_input_dac(bool t){setBitState(604,t);}
  void setEN_bias_charge(bool t){setBitState(605,t);}
  void setPP_bias_charge(bool t){setBitState(606,t);}
  void setCf3_100fF(bool t){setBitState(607,t);}
  void setCf2_200fF(bool t){setBitState(608,t);}
  void setCf1_2p5pF(bool t){setBitState(609,t);}
  void setCf0_1p25pF(bool t){setBitState(610,t);}
  void setEN_bias_sca(bool t){setBitState(611,t);}
  void setPP_bias_sca(bool t){setBitState(612,t);}
  void setEN_bias_discri_charge(bool t){setBitState(613,t);}
  void setPP_bias_discri_charge(bool t){setBitState(614,t);}
  void setEN_bias_discri_adc_time(bool t){setBitState(615,t);}
  void setPP_bias_discri_adc_time(bool t){setBitState(616,t);}
  void setEN_bias_discri_adc_charge(bool t){setBitState(617,t);}
  void setPP_bias_discri_adc_charge(bool t){setBitState(618,t);}
  void setDIS_razchn_int(bool t){setBitState(619,t);}
  void setDIS_razchn_ext(bool t){setBitState(620,t);}
  void setSEL_80M(bool t){setBitState(621,t);}
  void setEN_80M(bool t){setBitState(622,t);}
  void setEN_slow_lvds_rec(bool t){setBitState(623,t);}
  void setPP_slow_lvds_rec(bool t){setBitState(624,t);}
  void setEN_fast_lvds_rec(bool t){setBitState(625,t);}
  void setPP_fast_lvds_rec(bool t){setBitState(626,t);}
  void setEN_transmitter(bool t){setBitState(627,t);}
  void setPP_transmitter(bool t){setBitState(628,t);}
  void setON_OFF_1mA(bool t){setBitState(629,t);}
  void setON_OFF_2mA(bool t){setBitState(630,t);}
  void setON_OFF_otaQ(bool t){setBitState(631,t);}
  void setON_OFF_ota_mux(bool t){setBitState(632,t);}
  void setON_OFF_ota_probe(bool t){setBitState(633,t);}
  void setDIS_trig_mux(bool t){setBitState(634,t);}
  void setEN_NOR32_time(bool t){setBitState(635,t);}
  void setEN_NOR32_charge(bool t){setBitState(636,t);}
  void setDIS_triggers(bool t){setBitState(637,t);}
  void setEN_dout_oc(bool t){setBitState(638,t);}
  void setEN_transmit(bool t){setBitState(639,t);}
  bool getEN_adc(){return getBit(574);}
  bool getPP_adc(){return getBit(575);}
  bool getsel_starb_ramp_adc_ext(){return getBit(576);}
  bool getusebcompensation(){return getBit(577);}
  bool getEN_bias_dac_delay(){return getBit(578);}
  bool getPP_bias_dac_delay(){return getBit(579);}
  bool getEN_bias_ramp_delay(){return getBit(578);}
  bool getPP_bias_ramp_delay(){return getBit(579);}
  bool getEN_discri_delay(){return getBit(590);}
  bool getPP_discri_delay(){return getBit(591);}
  bool getEN_temp_sensor(){return getBit(592);}
  bool getPP_temp_sensor(){return getBit(593);}
  bool getEN_bias_pa(){return getBit(594);}
  bool getPP_bias_pa(){return getBit(595);}
  bool getEN_bias_discri(){return getBit(596);}
  bool getPP_bias_discri(){return getBit(597);}
  bool getcmd_polarity(){return getBit(598);}
  bool getlatch(){return getBit(599);}
  bool getEN_bias_6bit_dac(){return getBit(600);}
  bool getPP_bias_6bit_dac(){return getBit(601);}
  bool getEN_bias_tdc(){return getBit(602);}
  bool getPP_bias_tdc(){return getBit(603);}
  bool getON_OFF_input_dac(){return getBit(604);}
  bool getEN_bias_charge(){return getBit(605);}
  bool getPP_bias_charge(){return getBit(606);}
  bool getCf3_100fF(){return getBit(607);}
  bool getCf2_200fF(){return getBit(608);}
  bool getCf1_2p5pF(){return getBit(609);}
  bool getCf0_1p25pF(){return getBit(610);}
  bool getEN_bias_sca(){return getBit(611);}
  bool getPP_bias_sca(){return getBit(612);}
  bool getEN_bias_discri_charge(){return getBit(613);}
  bool getPP_bias_discri_charge(){return getBit(614);}
  bool getEN_bias_discri_adc_time(){return getBit(615);}
  bool getPP_bias_discri_adc_time(){return getBit(616);}
  bool getEN_bias_discri_adc_charge(){return getBit(617);}
  bool getPP_bias_discri_adc_charge(){return getBit(618);}
  bool getDIS_razchn_int(){return getBit(619);}
  bool getDIS_razchn_ext(){return getBit(620);}
  bool getSEL_80M(){return getBit(621);}
  bool getEN_80M(){return getBit(622);}
  bool getEN_slow_lvds_rec(){return getBit(623);}
  bool getPP_slow_lvds_rec(){return getBit(624);}
  bool getEN_fast_lvds_rec(){return getBit(625);}
  bool getPP_fast_lvds_rec(){return getBit(626);}
  bool getEN_transmitter(){return getBit(627);}
  bool getPP_transmitter(){return getBit(628);}
  bool getON_OFF_1mA(){return getBit(629);}
  bool getON_OFF_2mA(){return getBit(630);}
  bool getON_OFF_otaQ(){return getBit(631);}
  bool getON_OFF_ota_mux(){return getBit(632);}
  bool getON_OFF_ota_probe(){return getBit(633);}
  bool getDIS_trig_mux(){return getBit(634);}
  bool getEN_NOR32_time(){return getBit(635);}
  bool getEN_NOR32_charge(){return getBit(636);}
  bool getDIS_triggers(){return getBit(637);}
  bool getEN_dout_oc(){return getBit(638);}
  bool getEN_transmit(){return getBit(639);}

  void loadInvert(PRSlow r)
  {

    for (int i=639;i>=0;i--)
      setBitState(639-i,r.getBit(i)==1);

  }
     
private:
  uint32_t _l[20];
};
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
  

  s.loadConfig("./PR1.txt");
  // for (int ch=0;ch<32;ch++)
  //   {
  //     s.setInputDacCommand(ch,1);
  //     s.setMaskDiscriTime(ch,1);
  //     s.set6bDac(ch,37);
  //   }
  s.Print();

  PRSlow s1,s2;
  PRSlow::loadAsics("./slow_control_calib.csv",s1,s2);
  s1.Print();
  getchar();
  s2.Print();
  
}
