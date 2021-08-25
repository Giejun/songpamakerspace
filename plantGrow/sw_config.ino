int sw(){
  if(pg == 0 && optn == 0)
    pg = !pg;
  if(pg == 1 && led_pg == 0 && c_fan_pg == 0 && air_fan_pg == 0 && ox_motor_pg == 0){
    myEnc.write(4 * 1);
  }
  if(led_pg == 1){
    EEPROM.write(0, hr_set);
    EEPROM.write(1, min_set);
   // EEPROM.write(2, sec_set);
    EEPROM.write(3, HR_set);
    EEPROM.write(4, MIN_set);
   // EEPROM.write(5, SEC_set);
   
    t_set = !t_set;
    myEnc.write(4 * 0);
/*
    if((t_pos == 1 || t_pos == 2 || t_pos == 3 || t_pos == 4 || t_pos == 5 || t_pos == 6 ) && t_set == 0){
      myEnc.write(4 * t_pos);
    } 
*/    
    if((t_pos == 1 || t_pos == 2 || t_pos == 4 || t_pos == 5 ) && t_set == 0){    // *******t_pos reset
      myEnc.write(4 * t_pos);
    }   
  }

  if(optn == 1 && t_pos == 0){
    led_pg = 1;
    myEnc.write(4 * 1);
  }
  if(optn == 5){
    pg = 0;
    optn = 0;
    optn_old = 0;
  }

  
  ////////////////////////////////////////////////cooling fan//////////////////////////////////////////////////
  if(c_fan_pg == 1){
    EEPROM.write(6, temp_set);
    EEPROM.write(7, humi_set);
    EEPROM.write(16, temp_dec_set);
    EEPROM.write(17, humi_dec_set);
    
    t_set = !t_set;
    myEnc.write(4 * 0);
    if((temp_pos == 1 || temp_pos == 2 || temp_pos == 3 || temp_pos == 4 ) && t_set == 0){
      myEnc.write(4 * temp_pos);
    }
  }

  if(optn == 2 && temp_pos == 0){
    c_fan_pg = 1;
    myEnc.write(4 * 1);
  }


  ////////////////////////////////////////////////air fan//////////////////////////////////////////////////
  if (air_fan_pg == 1){
    EEPROM.write(8, min1_set);
   // EEPROM.write(9, sec1_set);
    EEPROM.write(10, MIN1_set);
   // EEPROM.write(11, SEC1_set);

    if(t_set == 1){
      t_set = 0;
    }
    else
      t_set = 1;
    Serial.println(String(t_set) + " got it");
    myEnc.write(4 * 0);
    
    if((time1_pos == 1 || time1_pos == 2 || time1_pos == 3 || time1_pos == 4  ) && t_set == 0){
      myEnc.write(4 * time1_pos);
    }
  }

  if(optn == 3 && time1_pos == 0){
    air_fan_pg = 1;
    myEnc.write(4 * 1);
  }
  

  ////////////////////////////////////////////////ox_motor//////////////////////////////////////////////////
  if (ox_motor_pg == 1){
    EEPROM.write(12, min2_set);
   // EEPROM.write(13, sec2_set);
    EEPROM.write(14, MIN2_set);
   // EEPROM.write(15, SEC2_set);

    if (t_set == 1){
      t_set = 0;
    }
    else
      t_set = 1;
    Serial.println(String(t_set) + " got it");
    myEnc.write(4 * 0);
    
    if((time2_pos == 1 || time2_pos == 2 || time2_pos == 3 || time2_pos == 4  ) && t_set == 0){
      myEnc.write(4 * time2_pos);
    }
  }

  if(optn == 4 && time2_pos == 0){
    ox_motor_pg = 1;
    myEnc.write(4 * 1);
  }

  Serial.println("click1");
  Serial.println(air_fan_pg);
}
