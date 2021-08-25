int enc_config(){  
  if (pg == 0){
    myEnc.write(4 * 0);
  }
  
  if(pg == 1 && led_pg == 0 && c_fan_pg == 0 && air_fan_pg == 0 && ox_motor_pg == 0){

    //Serial.println("1: " + String(optn));

    if (optn != optn_old){
      optn_old = optn;
      
      if(optn > 5){
        myEnc.write(4 * 1);
        }
        
      if(optn < 1){
        myEnc.write(4 * 5);
        }
    }
    
    // Serial.println("2: " + String(optn));
    optn = myEnc.read() / 4;
    // Serial.println("3: " + String(optn));
  }

/////////////////////////////// led light time_set ////////////////////////

  if(led_pg == 1 && t_set == 0){                   
    if (t_pos != t_pos_old){
      t_pos_old = t_pos;
      if (t_pos > 7){               //7
        myEnc.write(4 * 1);
      }
      if(t_pos < 1){
        myEnc.write(4 * 7);         //7
      }
    }
    
    t_pos = myEnc.read() / 4;
  }

  if(t_set == 1 && t_pos == 1){                    // hr= 24hr format
    
     hr_set = EEPROM.read(0) + myEnc.read() / 4;

    if(hr_set != hr_set_old){

      hr_set_old = hr_set;
      if (hr_set > 23){
        myEnc.write(4 * 0);
        EEPROM.write(0, 0);
      }
      else if (hr_set < 0){ 
        myEnc.write(4 * 23);
        EEPROM.write(0, 0);
      }
    }
  }

  if(t_set == 1 && t_pos == 2){                 // min
  
    min_set = EEPROM.read(1) + myEnc.read() / 4;

    if (min_set != min_set_old){

      min_set_old = min_set;
      if (min_set > 59){
        myEnc.write(4 * 0);
        EEPROM.write(1, 0);
      }
      else if (min_set < 0){
        myEnc.write(4 * 59);
        EEPROM.write(1, 0);
      }
    }

  }

 // if (t_set == 1 && t_pos == 3){            // sec
 //
 //   sec_set = EEPROM.read(2) + myEnc.read() / 4;
 //
 //   if (sec_set != sec_set_old){
 //   
 //   sec_set_old = sec_set;
 //   if (sec_set > 59){
 //     myEnc.write(4 * 0);
 //     EEPROM.write(2, 0);
 //   }
 //   else if (sec_set < 0){      
 //     myEnc.write(4 * 59);
 //     EEPROM.write(2, 0);
 //   }
 // }
 //
 //}
   
  if(t_set == 1 && t_pos == 4){                 //4   hr= 24hr format
  
    HR_set = EEPROM.read(3) + myEnc.read() / 4;

    if (HR_set != HR_set_old){
     HR_set_old = HR_set;
      if (HR_set > 23){
        myEnc.write(4 * 0);
        EEPROM.write(3, 0);
      }
      else if (HR_set < 0){
        myEnc.write(4 * 23);
        EEPROM.write(3, 0);
      }
    }
  }

  if(t_set == 1 && t_pos == 5){               //5  min
  
    MIN_set = EEPROM.read(4) + myEnc.read() / 4;

    if (MIN_set != MIN_set_old){
        MIN_set_old = MIN_set;
      if (MIN_set > 59){
        myEnc.write(4 * 0);
        EEPROM.write(4, 0);
      }
      else if (MIN_set < 0){
        myEnc.write(4 * 59);
        EEPROM.write(4, 0);
      }
    }
  }

 // if(t_set == 1 && t_pos == 6){               // SEC
 // 
 //   SEC_set = EEPROM.read(5) + myEnc.read() / 4;
 //   if(SEC_set != SEC_set_old){
 //
 //   SEC_set_old = SEC_set;
 //   if(SEC_set > 59){
 //   myEnc.write(4 * 0);
 //     EEPROM.write(5, 0);
 //   }
 //   else if(SEC_set < 0){
 //     myEnc.write(4 * 59);
 //     EEPROM.write(5, 0);
 //   }
 // }
 //}
 
  if(t_set == 1 && t_pos == 7){         //7
    led_pg = 0;
    t_set = 0;
    t_pos = 0;
    t_pos_old = 0;
    myEnc.write(4 * 1);
  }

  ///////////////////////////////////////////////////////////////cooling fan///////////////////////////////////////////////////////////////////
  if(c_fan_pg == 1 && t_set == 0){ //temp pos
  
    if (temp_pos != temp_pos_old){
      temp_pos_old = temp_pos;
      if(temp_pos > 5){
        myEnc.write(4 * 1);
      }
      if(temp_pos < 1){
        myEnc.write(4 * 5);
      }
    }
    temp_pos = myEnc.read()/4;
  }
  if(t_set == 1 && temp_pos == 1){ // temp
  
    temp_set = EEPROM.read(6) + myEnc.read() / 4;
    if(temp_set != temp_set_old){

      temp_set_old = temp_set;
      if(temp_set > 100){
        myEnc.write(4 * 0);
        EEPROM.write(6, 0);
      }
      else if (temp_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(6, 0);
      }
    }
  }
  if(t_set == 1 && temp_pos == 2){ // temp_dec
  
    temp_dec_set = EEPROM.read(16) + myEnc.read() / 4;

    if(temp_dec_set != temp_dec_set_old){

      temp_dec_set_old = temp_dec_set;
      if (temp_dec_set > 99){
        myEnc.write(4 * 0);
        EEPROM.write(16, 0);
      }
      else if(temp_dec_set < 0){
        myEnc.write(4 * 99);
        EEPROM.write(16, 0);
      }
    }
  }
  if(t_set == 1 && temp_pos == 3){ // humi
  
    humi_set = EEPROM.read(7) + myEnc.read() / 4;

    if(humi_set != humi_set_old){
      humi_set_old = humi_set;
      
      if(humi_set > 100){
        myEnc.write(4 * 0);
        EEPROM.write(7, 0);
      }
      else if (humi_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(7, 0);
      }
    }
  }
  if(t_set == 1 && temp_pos == 4){  // humi
  
    humi_dec_set = EEPROM.read(17) + myEnc.read() / 4;

    if(humi_dec_set != humi_dec_set_old){

      humi_dec_set_old = humi_dec_set;
      if(humi_dec_set > 99){
        myEnc.write(4 * 0);
        EEPROM.write(17, 0);
      }
      else if(humi_dec_set < 0){
        myEnc.write(4 * 99);
        EEPROM.write(17, 0);
      }
    }
  }
  if(t_set == 1 && temp_pos == 5){
    c_fan_pg = 0;
    t_set = 0;
    temp_pos = 0;
    temp_pos_old = 0;
    myEnc.write(4 * 2);
  }

  ////////////////////////////////////////////////fan_on/off/////////////////////////////////
  if(air_fan_pg == 1 && t_set == 0){             // Fan Duratio time Set
  
    if(time1_pos != time1_pos_old){
      time1_pos_old = time1_pos;
      
      if(time1_pos > 5){
        myEnc.write(4 * 1);
      }
      if(time1_pos < 1){
        myEnc.write(4 * 5);
      }
    }
    time1_pos = myEnc.read() / 4;
  }
  
  if(t_set == 1 && time1_pos == 1){            // min
  
    min1_set = EEPROM.read(8) + myEnc.read() / 4;

    if(min1_set != min1_set_old){
      min1_set_old = min1_set;
      if(min1_set > 100){
        myEnc.write(4 * 0);
        EEPROM.write(8, 0);
      }
      else if (min1_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(8, 0);
      }
    }
  }
 //if(t_set == 1 && time1_pos == 2){    // sec
 // 
 // sec1_set = EEPROM.read(9) + myEnc.read() / 4;
 //
 // if(sec1_set != sec1_set_old){
 //   sec1_set_old = sec1_set;
 //   if(sec1_set > 59){
 //     myEnc.write(4 * 0);
 //     EEPROM.write(9, 0);
 //   }
 //   else if(sec1_set < 0){
 //     myEnc.write(4 * 59);
 //     EEPROM.write(9, 0);
 //    }
 // }
 //}

  if(t_set == 1 && time1_pos == 3){   // min
  
    MIN1_set = EEPROM.read(10) + myEnc.read() / 4;

    if(MIN1_set != MIN1_set_old){

      MIN1_set_old = MIN1_set;
      if(MIN1_set > 100){
        myEnc.write(4 * 0);
        EEPROM.write(10, 0);
      }
      else if(MIN1_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(10, 0);
      }
    }
  }
  
//  if(t_set == 1 && time1_pos == 4){    // sec
//  
//    SEC1_set = EEPROM.read(11) + myEnc.read() / 4;
//
//    if (SEC1_set != SEC1_set_old){
//      SEC1_set_old = SEC1_set;
//      if (SEC1_set > 59){
//        myEnc.write(4 * 0);
//        EEPROM.write(11, 0);
//      }
//      else if(SEC1_set < 0){
//        myEnc.write(4 * 59);
//        EEPROM.write(11, 0);
//      }
//    }
//  }

  if(t_set == 1 && time1_pos == 5){
    air_fan_pg = 0;
    t_set = 0;
    time1_pos = 0;
    time1_pos_old = 0;
    myEnc.write(4 * 3);
  }

  
  ////////////////////////////////////////////////oxygen motor_on/off/////////////////////////////////
  if(ox_motor_pg == 1 && t_set == 0){       // led light time_set
  
    if(time2_pos != time2_pos_old){
      time2_pos_old = time2_pos;
      if(time2_pos > 5){
        myEnc.write(4 * 1);
      }
      
      if (time2_pos < 1){
        myEnc.write(4 * 5);
      }
    }
    time2_pos = myEnc.read() / 4;
  }
  
  if(t_set == 1 && time2_pos == 1){       // min
  
    min2_set = EEPROM.read(12) + myEnc.read() / 4;
    if(min2_set != min2_set_old){

      min2_set_old = min2_set;
      if(min2_set > 100)      {
        myEnc.write(4 * 0);
        EEPROM.write(12, 0);
      }
      else if(min2_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(12, 0);
      }
    }
  }
  
 // if(t_set == 1 && time2_pos == 2){      // sec
 // 
 //   sec2_set = EEPROM.read(13) + myEnc.read() / 4;
 //
 //   if(sec2_set != sec2_set_old){
 //    sec2_set_old = sec2_set;
 //     if (sec2_set > 59){
 //     myEnc.write(4 * 0);
 //     EEPROM.write(13, 0);
 //     }
 //     else if (sec2_set < 0){
 //       myEnc.write(4 * 59);
 //       EEPROM.write(13, 0);
 //     }
 //   }
 // }

  if(t_set == 1 && time2_pos == 3){       // min
  
    MIN2_set = EEPROM.read(14) + myEnc.read() / 4;

    if(MIN2_set != MIN2_set_old){

      MIN2_set_old = MIN2_set;
      if (MIN2_set > 100){
        myEnc.write(4 * 0);
        EEPROM.write(14, 0);
      }
      else if(MIN2_set < 0){
        myEnc.write(4 * 100);
        EEPROM.write(14, 0);
      }
    }
  }
  
 // if(t_set == 1 && time2_pos == 4){ // sec
 // 
 //   SEC2_set = EEPROM.read(15) + myEnc.read() / 4;
 //
 //   if(SEC2_set != SEC2_set_old){
 //     SEC2_set_old = SEC2_set;
 //     
 //     if (SEC2_set > 59){
 //       myEnc.write(4 * 0);
 //       EEPROM.write(15, 0);
 //     }
 //     else if (SEC2_set < 0){
 //       myEnc.write(4 * 59);
 //       EEPROM.write(15, 0);
 //     }
 //    }
 //  }
  
  if (t_set == 1 && time2_pos == 5){
    ox_motor_pg = 0;
    t_set = 0;
    time2_pos = 0;
    time2_pos_old = 0;
    myEnc.write(4 * 4);
  }

  //  Serial.println("page= " + String(pg) + "||" + "optn= " + String(optn) + "||" +
  //                 "t_pos= " + String(time1_pos) + "||" + "tset= " + String(t_set) + "||" + "hr_set= " + String(min1_set) + "||" + "min_set= " + String(sec1_set));
}
