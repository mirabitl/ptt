{

   const Json::Value& books = _jc["6bDac"];
   uint8_t ch=0;
   for (Json::ValueConstIterator it = books.begin(); it != books.end(); ++it)
   {
     const Json::Value& book = *it;
     uint8_t sid=(*it).asUInt();
     set6bDac(ch,sid);
     ch++;
   }
   books = _jc["InputDac"];
   ch=0;
   for (Json::ValueConstIterator it = books.begin(); it != books.end(); ++it)
   {
     const Json::Value& book = *it;
     uint8_t sid=(*it).asUInt();
     setInputDac(ch,sid);
     ch++;
   }
   books = _jc["InputDacCommand"];
   ch=0;
   for (Json::ValueConstIterator it = books.begin(); it != books.end(); ++it)
   {
     const Json::Value& book = *it;
     uint8_t sid=(*it).asUInt();
     setInputDacCommand(ch,sid);
     ch++;
   }
   books = _jc["MaskDiscriCharge"];
   ch=0;
   for (Json::ValueConstIterator it = books.begin(); it != books.end(); ++it)
   {
     const Json::Value& book = *it;
     uint8_t sid=(*it).asUInt();
     setMaskDiscriCharge(ch,sid);
     ch++;
   }
   books = _jc["MaskDiscriTime"];
   ch=0;
   for (Json::ValueConstIterator it = books.begin(); it != books.end(); ++it)
   {
     const Json::Value& book = *it;
     uint8_t sid=(*it).asUInt();
     setMaskDiscriTime(ch,sid);
     ch++;
   }
   setCf0_1p25pF(_jasic["Cf0_1p25pF"].asUInt());
   setCf1_2p5pF(_jasic["Cf1_2p5pF"].asUInt());
   setCf2_200fF(_jasic["Cf2_200fF"].asUInt());
   setCf3_100fF(_jasic["Cf3_100fF"].asUInt());
   setDIS_razchn_ext(_jasic["DIS_razchn_ext"].asUInt());
   setDIS_razchn_int(_jasic["DIS_razchn_int"].asUInt());
   setDIS_trig_mux(_jasic["DIS_trig_mux"].asUInt());
   setDIS_triggers(_jasic["DIS_triggers"].asUInt());
   setDacDelay(_jasic["DacDelay"].asUInt());
   setEN10bDac(_jasic["EN10bDac"].asUInt());
   setEN_80M(_jasic["EN_80M"].asUInt());
   setEN_NOR32_charge(_jasic["EN_NOR32_charge"].asUInt());
   setEN_NOR32_time(_jasic["EN_NOR32_time"].asUInt());
   setEN_adc(_jasic["EN_adc"].asUInt());
   setEN_bias_6bit_dac(_jasic["EN_bias_6bit_dac"].asUInt());
   setEN_bias_charge(_jasic["EN_bias_charge"].asUInt());
   setEN_bias_dac_delay(_jasic["EN_bias_dac_delay"].asUInt());
   setEN_bias_discri(_jasic["EN_bias_discri"].asUInt());
   setEN_bias_discri_adc_charge(_jasic["EN_bias_discri_adc_charge"].asUInt());
   setEN_bias_discri_adc_time(_jasic["EN_bias_discri_adc_time"].asUInt());
   setEN_bias_discri_charge(_jasic["EN_bias_discri_charge"].asUInt());
   setEN_bias_pa(_jasic["EN_bias_pa"].asUInt());
   setEN_bias_ramp_delay(_jasic["EN_bias_ramp_delay"].asUInt());
   setEN_bias_sca(_jasic["EN_bias_sca"].asUInt());
   setEN_bias_tdc(_jasic["EN_bias_tdc"].asUInt());
   setEN_discri_delay(_jasic["EN_discri_delay"].asUInt());
   setEN_dout_oc(_jasic["EN_dout_oc"].asUInt());
   setEN_fast_lvds_rec(_jasic["EN_fast_lvds_rec"].asUInt());
   setEN_slow_lvds_rec(_jasic["EN_slow_lvds_rec"].asUInt());
   setEN_temp_sensor(_jasic["EN_temp_sensor"].asUInt());
   setEN_transmit(_jasic["EN_transmit"].asUInt());
   setEN_transmitter(_jasic["EN_transmitter"].asUInt());
   setON_OFF_1mA(_jasic["ON_OFF_1mA"].asUInt());
   setON_OFF_2mA(_jasic["ON_OFF_2mA"].asUInt());
   setON_OFF_input_dac(_jasic["ON_OFF_input_dac"].asUInt());
   setON_OFF_otaQ(_jasic["ON_OFF_otaQ"].asUInt());
   setON_OFF_ota_mux(_jasic["ON_OFF_ota_mux"].asUInt());
   setON_OFF_ota_probe(_jasic["ON_OFF_ota_probe"].asUInt());
   setPP10bDac(_jasic["PP10bDac"].asUInt());
   setPP_adc(_jasic["PP_adc"].asUInt());
   setPP_bias_6bit_dac(_jasic["PP_bias_6bit_dac"].asUInt());
   setPP_bias_charge(_jasic["PP_bias_charge"].asUInt());
   setPP_bias_dac_delay(_jasic["PP_bias_dac_delay"].asUInt());
   setPP_bias_discri(_jasic["PP_bias_discri"].asUInt());
   setPP_bias_discri_adc_charge(_jasic["PP_bias_discri_adc_charge"].asUInt());
   setPP_bias_discri_adc_time(_jasic["PP_bias_discri_adc_time"].asUInt());
   setPP_bias_discri_charge(_jasic["PP_bias_discri_charge"].asUInt());
   setPP_bias_pa(_jasic["PP_bias_pa"].asUInt());
   setPP_bias_ramp_delay(_jasic["PP_bias_ramp_delay"].asUInt());
   setPP_bias_sca(_jasic["PP_bias_sca"].asUInt());
   setPP_bias_tdc(_jasic["PP_bias_tdc"].asUInt());
   setPP_discri_delay(_jasic["PP_discri_delay"].asUInt());
   setPP_fast_lvds_rec(_jasic["PP_fast_lvds_rec"].asUInt());
   setPP_slow_lvds_rec(_jasic["PP_slow_lvds_rec"].asUInt());
   setPP_temp_sensor(_jasic["PP_temp_sensor"].asUInt());
   setPP_transmitter(_jasic["PP_transmitter"].asUInt());
   setSEL_80M(_jasic["SEL_80M"].asUInt());
   setVthDiscriCharge(_jasic["VthDiscriCharge"].asUInt());
   setVthTime(_jasic["VthTime"].asUInt());
   setcmd_polarity(_jasic["cmd_polarity"].asUInt());
   setlatch(_jasic["latch"].asUInt());
   setsel_starb_ramp_adc_ext(_jasic["sel_starb_ramp_adc_ext"].asUInt());
   setusebcompensation(_jasic["usebcompensation"].asUInt());
}
