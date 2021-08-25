void refresh(){
  display.display();
  delay(10);
  display.clearDisplay();
}

void def_page(){
  DateTime now = rtc.now();
  if(pg == 0){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 5);
    display.print(String(Month[now.month()]) + "." + String(now.day()) + " | " + String(now.hour()) + ":" + String(now.minute()));
    display.drawRect (3, 18 , 110, 45, WHITE);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 25); 
    display.print( "Air:");
    display.setCursor(70, 25);    
    display.print( String(Temp) + "C");
    display.setCursor(70, 35); 
    display.print(String(Humi) + "%");
    display.setCursor(10, 50);
    display.print( "Water:" );
    display.setCursor(70, 50);
    display.print( String(ds_temp) + "C ");
    refresh();
  }
}

void setting_pg(){

  if (pg == 1 && led_pg == 0 && c_fan_pg == 0 && air_fan_pg == 0 && ox_motor_pg == 0){
    display.setCursor(30, 0);  display.print("SET MENU");
    display.drawRect (0, 10 , 128, 2, WHITE);
    refresh();

    if (optn <= 1 && t_pos == 0 ){
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 15);  display.print (" LED Light");
      display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 25);  display.print (" Cooling Fan");
      display.setCursor(0, 35);  display.print (" Fan On/Off");
      display.setCursor(0, 45);  display.print (" Oxygen Motor");
      display.setCursor(0, 55);  display.print (" Back");
      // refresh();
    }
    if (optn == 2 && t_pos == 0 ){
      display.setCursor(0, 15);  display.print (" LED Light");
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 25);  display.print (" Cooling Fan");
      display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 35);  display.print (" Fan On/Off");
      display.setCursor(0, 45);  display.print (" Oxygen Motor");
      display.setCursor(0, 55);  display.print (" Back");
      //refresh();
    }
    if (optn == 3 && t_pos == 0 ){
      display.setCursor(0, 15);  display.print (" LED Light");
      display.setCursor(0, 25);  display.print (" Cooling Fan");
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 35);  display.print (" Fan On/Off");
      display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 45);  display.print (" Oxygen Motor");
      display.setCursor(0, 55);  display.print (" Back");
      //refresh();
    }
    if (optn == 4 && t_pos == 0 ){
      display.setCursor(0, 15);  display.print (" LED Light");
      display.setCursor(0, 25);  display.print (" Cooling Fan");
      display.setCursor(0, 35);  display.print (" Fan On/Off");
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 45);  display.print (" Oxygen Motor");
      display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 55);  display.print (" Back");
      // refresh();
    }
    if (optn >= 5 && t_pos == 0 ){
      display.setCursor(0, 15);  display.print (" LED Light");
      display.setCursor(0, 25);  display.print (" Cooling Fan");
      display.setCursor(0, 35);  display.print (" Fan On/Off");
      display.setCursor(0, 45);  display.print (" Oxygen Motor");
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 55);  display.print (" Back");
      display.setTextColor (WHITE, BLACK);
      //refresh();
    }
  }
}
void led_light_pg(){
  char digit[80];
  if (led_pg == 1){
    display.setCursor(27, 0);  display.print("LED LIGHT");
    display.drawRect (0, 10 , 128, 2, WHITE);
    refresh();
    
      if (t_pos <= 1){
      display.setCursor(10, 15);
      display.print (" ON :");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", hr_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit);
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setCursor(0, 55); 
      display.print (" Back");
    }
    if (t_pos == 2){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", min_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit);
     // display.print(":");
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setCursor(0, 55); 
      display.print (" Back");
    }

 
    if (t_pos == 3){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit); 
      //display.print(":");
      //display.setTextColor (BLACK, WHITE);
      //sprintf(digit, "%02d", sec_set);
      //display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.setCursor(10, 25);
      display.print (" OFF:"); 
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit); 
      //display.print(":");
      //sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print (" Back");
    }


    if (t_pos == 4){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", HR_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print (" Back");
    }

    
    if (t_pos == 5){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", MIN_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
     // display.print(":");
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print (" Back");
    }


    if (t_pos == 6){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit); 
     // display.print(":");
     // display.setTextColor (BLACK, WHITE);
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 55);  
      display.print (" Back");
    }
 

    
    if (t_pos >= 7){
      display.setCursor(10, 15);
      display.print (" ON :");
      sprintf(digit, "%02d", hr_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", min_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", sec_set);
     // display.print(digit);
      display.setCursor(10, 25);
      display.print (" OFF:");
      sprintf(digit, "%02d", HR_set);
      display.print(digit); 
      display.print(":");
      sprintf(digit, "%02d", MIN_set);
      display.print(digit); 
     // display.print(":");
     // sprintf(digit, "%02d", SEC_set);
     // display.print(digit);
      display.setTextColor (BLACK, WHITE);
      display.setCursor(0, 55);  
      display.print (" Back");
      display.setTextColor (WHITE, BLACK);
    }
  }
}

void cooling_fan_pg(){
  char digit [80];
  if(c_fan_pg == 1){
    display.setCursor(27, 0);  
    display.print("AIR COOLING");
    display.drawRect (0, 10 , 128, 2, WHITE);
    refresh();

    if(temp_pos <= 1){
      display.setCursor(10, 15);
      display.print ("T :");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", temp_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(".");
      sprintf(digit, "%02d", temp_dec_set);
      display.print(digit);
      display.print("C");
      display.setCursor(10, 25);
      display.print ("H :");
      sprintf(digit, "%02d", humi_set);
      display.print(digit); 
      display.print(".");
      sprintf(digit, "%02d", humi_dec_set);
      display.print(digit); 
      display.print("%");
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    
    if (temp_pos == 2){
      display.setCursor(10, 15);
      display.print ("T :");
      sprintf(digit, "%02d", temp_set);
      display.print(digit);
      display.print(".");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", temp_dec_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print("C");
      display.setCursor(10, 25);
      display.print ("H :");
      sprintf(digit, "%02d", humi_set);
      display.print(digit); 
      display.print(".");
      sprintf(digit, "%02d", humi_dec_set);
      display.print(digit); 
      display.print("%");
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    
    if (temp_pos == 3){
      display.setCursor(10, 15);
      display.print ("T :");
      sprintf(digit, "%02d", temp_set);
      display.print(digit);
      display.print(".");
      sprintf(digit, "%02d", temp_dec_set);
      display.print(digit);
      display.print("C");
      display.setCursor(10, 25);
      display.print ("H :");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", humi_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK); 
      display.print(".");
      sprintf(digit, "%02d", humi_dec_set);
      display.print(digit); 
      display.print("%");
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    
    if (temp_pos == 4){
      display.setCursor(10, 15);
      display.print ("T :");
      sprintf(digit, "%02d", temp_set);
      display.print(digit);
      display.print(".");
      sprintf(digit, "%02d", temp_dec_set);
      display.print(digit);
      display.print("C");
      display.setCursor(10, 25);
      display.print ("H :");
      sprintf(digit, "%02d", humi_set);
      display.print(digit); 
      display.print(".");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%02d", humi_dec_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK); 
      display.print("%");
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    if (temp_pos >= 5){
      display.setCursor(10, 15);
      display.print ("T :");
      sprintf(digit, "%02d", temp_set);
      display.print(digit);
      display.print(".");
      sprintf(digit, "%02d", temp_dec_set);
      display.print(digit);
      display.print("C");
      display.setCursor(10, 25);
      display.print ("H :");
      sprintf(digit, "%02d", humi_set);
      display.print(digit); display.print(".");
      sprintf(digit, "%02d", humi_dec_set);
      display.print(digit);
      display.print("%");
      display.setCursor(0, 55);
      display.setTextColor (BLACK, WHITE);
      display.print ("Back");
      display.setTextColor (WHITE, BLACK);
    }
  }
}


void Air_fan_pg(){
  char digit [80];
  if (air_fan_pg == 1){
    display.setCursor(27, 0);  
    display.print("AIR FAN ");
    display.drawRect (0, 10 , 128, 2, WHITE);
    refresh();

    if (time1_pos <= 1){
      display.setCursor(20, 15);
      display.print ("ON :");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%03d", min1_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec1_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN1_set);
      display.print(digit); 
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC1_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    if (time1_pos == 2){

      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min1_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // display.setTextColor (BLACK, WHITE);
     // sprintf(digit, "%02d", sec1_set);
     // display.print(digit);
     // display.setTextColor (WHITE, BLACK);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN1_set);
      display.print(digit); 
      display.setTextColor (WHITE, BLACK);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC1_set);
     // display.print(digit);
      display.setCursor(0, 55); 
      display.print ("Back");
    }

    
    if (time1_pos == 3){

      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min1_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec1_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%03d", MIN1_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC1_set);
     // display.print(digit);
      display.setCursor(0, 55); 
      display.print ("Back");
    }
    
    if (time1_pos == 4){
      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min1_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec1_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN1_set);
      display.print(digit);
      display.print(" mins");
     //display.print(":");
     // display.setTextColor (BLACK, WHITE);
     // sprintf(digit, "%02d", SEC1_set);
     // display.print(digit);
     // display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 55); 
      display.print ("Back");
    }


    
    if (time1_pos >= 5){

      display.setCursor(20,15);
      display.print ("ON :");
      sprintf(digit, "%03d", min1_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec1_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN1_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC1_set);
     // display.print(digit);
      display.setCursor(0, 55);
      display.setTextColor (BLACK, WHITE);
      display.print ("Back");
      display.setTextColor (WHITE, BLACK);
    }

    time_in_sec_1 = min1_set * 60 + sec1_set;
    time_in_sec_2 = MIN1_set * 60 + SEC1_set;
    if (time_in_sec_1 > 0){
      fan_state_1 = 1;
      //Serial.println(fan_state_1);
    }
    tick_1 = 0;

  }
}



void oxy_motor_pg(){
  char digit [80];
  if(ox_motor_pg == 1){
    display.setCursor(27, 0);  
    display.print("OXYGEN");
    display.drawRect (0, 10 , 128, 2, WHITE);
    refresh();

    if(time2_pos <= 1){
      display.setCursor(20, 15);
      display.print ("ON :");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%03d", min2_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec2_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN2_set);
      display.print(digit); 
       display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC2_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    
    if(time2_pos == 2){

      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // display.setTextColor (BLACK, WHITE);
     // sprintf(digit, "%02d", sec2_set);
     // display.print(digit);
     // display.setTextColor (WHITE, BLACK);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN2_set);
      display.print(digit); 
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC2_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    if(time2_pos == 3){
      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec2_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      display.setTextColor (BLACK, WHITE);
      sprintf(digit, "%03d", MIN2_set);
      display.print(digit);
      display.setTextColor (WHITE, BLACK);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC2_set);
     // display.print(digit);
      display.setCursor(0, 55);  
      display.print ("Back");
    }
    
    if(time2_pos == 4){
      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec2_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // display.setTextColor (BLACK, WHITE);
     // sprintf(digit, "%02d", SEC2_set);
     // display.print(digit);
     // display.setTextColor (WHITE, BLACK);
      display.setCursor(0, 55); 
      display.print ("Back");
    }
    
    if (time2_pos >= 5){
      display.setCursor(20, 15);
      display.print ("ON :");
      sprintf(digit, "%03d", min2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", sec2_set);
     // display.print(digit);
      display.setCursor(20, 25);
      display.print ("OFF:");
      sprintf(digit, "%03d", MIN2_set);
      display.print(digit);
      display.print(" mins");
     // display.print(":");
     // sprintf(digit, "%02d", SEC2_set);
     // display.print(digit);
      display.setCursor(0, 55);
      display.setTextColor (BLACK, WHITE);
      display.print ("Back");
      display.setTextColor (WHITE, BLACK);
    }

    time_in_sec_3 = min2_set * 60 + sec2_set;
    time_in_sec_4 = MIN2_set * 60 + SEC2_set;

    if (time_in_sec_3 > 0){
      fan_state_2 = 1;
      //Serial.println(fan_state_2);
    }
    tick_2 = 0;
  }
}
