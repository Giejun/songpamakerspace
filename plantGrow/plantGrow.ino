
#include <Encoder.h>
#include <EEPROM.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "OneButton.h"
#include <RTClib.h>


#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

Encoder myEnc(3, 2);                     //Rotary Encoder Pins
OneButton button(A0, true);

RTC_DS3231 rtc;
char Month[13][12] = {" ", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

#define ONE_WIRE_BUS 7                   //DS18B Water Temperature Sensor Pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS_temp(&oneWire);
float ds_temp = 0;

#define DHTPIN 4                        //DHT22 Temperatuer & Humidity Sensor Pins
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float Temp = 0; float Humi = 0;
float temp_val = 0, humi_val = 0;

int LED_LIGHT = 9;                    // LED Light Relay Pin
int COOLING_FAN = 10;                 // Cooling Fan Pin
int AIR_FAN = 11;                     // Air Circulation Pin
int OXY_PUMP = 12;                    // Water Oxygen Pump Pin

int optn = 0, optn_old = 0;
int t_pos = 0, t_pos_old = 0;
int hr_set = 0, hr_set_old = 0;
int min_set = 0, min_set_old = 0;
int sec_set = 0, sec_set_old = 0;
int HR_set = 0, HR_set_old = 0;
int MIN_set = 0, MIN_set_old = 0;
int SEC_set = 0, SEC_set_old = 0;

int pg = 0;                           // Default Page
int led_pg = 0;                       // Led light page
int c_fan_pg = 0;                     // cooling fan page
int air_fan_pg = 0;                   // air fan page
int ox_motor_pg = 0;                  // Oxy Pump
int t_set = 0;

/*
   temp_pos = encoder pos for
*/
int temp_pos = 0; int temp_pos_old = 0;
int temp_set = 0; int temp_set_old;
int humi_set = 0; int humi_set_old = 0;
int temp_dec_set = 0; int temp_dec_set_old;
int humi_dec_set = 0; int humi_dec_set_old = 0;
unsigned int ontime_in_sec = 0, offtime_in_sec = 0, curr_time_in_sec = 0;

int time1_pos = 0, time1_pos_old = 0;
int min1_set = 0, min1_set_old = 0;
int sec1_set = 0, sec1_set_old = 0;
int MIN1_set = 0, MIN1_set_old = 0;
int SEC1_set = 0, SEC1_set_old = 0;

int time2_pos = 0, time2_pos_old = 0;
int min2_set = 0, min2_set_old = 0;
int sec2_set = 0, sec2_set_old = 0;
int MIN2_set = 0, MIN2_set_old = 0;
int SEC2_set = 0, SEC2_set_old = 0;

int c_fan_active = 0;
unsigned int time_in_sec_1 = 0;
unsigned int time_in_sec_2 = 0;
unsigned int time_in_sec_3 = 0;
unsigned int time_in_sec_4 = 0;
int fan_state_1 = 0;
int fan_state_2 = 0;
int tick_1 = 0;                          // for air fan sec counting
int tick_2 = 0;                          // for ox sec counting
int tick_3 = 0;                          // for temp sec counting


void setup() {

  pinMode(20, INPUT);                    // Mega2560 I2C interfacing
  pinMode(21, INPUT);                    // Mega2560 I2C interfaceing
  Serial.begin(9600);

  if(! rtc.begin()){
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2021, 7, 30, 02, 40, 0));
  }
   rtc.adjust(DateTime(2021, 8, 28, 8, 54, 0));

  dht.begin();
  DS_temp.begin();
  button.attachClick(sw);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);   // initialize OLED with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  hr_set = EEPROM.read(0);
  min_set = EEPROM.read(1);
  sec_set = EEPROM.read(2);
  HR_set = EEPROM.read(3);
  MIN_set = EEPROM.read(4);
  SEC_set = EEPROM.read(5);

  temp_set = EEPROM.read(6);
  humi_set = EEPROM.read(7);
  temp_dec_set = EEPROM.read(16);
  humi_dec_set = EEPROM.read(17);

  min1_set = EEPROM.read(8);
  sec1_set = EEPROM.read(9);
  MIN1_set = EEPROM.read(10);
  SEC1_set = EEPROM.read(11);

  min2_set = EEPROM.read(12);
  sec2_set = EEPROM.read(13);
  MIN2_set = EEPROM.read(14);
  SEC2_set = EEPROM.read(15);

  time_in_sec_1 = min1_set * 60 + sec1_set;
  time_in_sec_2 = MIN1_set * 60 + SEC1_set;
  time_in_sec_3 = min2_set * 60 + sec2_set;
  time_in_sec_4 = MIN2_set * 60 + SEC2_set;

  if(time_in_sec_1 > 0){
    fan_state_1 = 1;
    Serial.println(fan_state_1);
  }

  if(time_in_sec_3 > 0){
    fan_state_2 = 1;
    Serial.println(fan_state_2);
  }

  delay(1000);

  pinMode(LED_LIGHT, OUTPUT);
  pinMode(COOLING_FAN, OUTPUT);
  pinMode(AIR_FAN, OUTPUT);
  pinMode(OXY_PUMP, OUTPUT);

  digitalWrite(LED_LIGHT, LOW);
  digitalWrite(COOLING_FAN, LOW);
  digitalWrite(AIR_FAN, LOW);
  digitalWrite(OXY_PUMP, LOW);
}

/*
   pg(sw) = default screen shift
   optn/optn_old = setting value page
   led_pg/led_pg = LED_Light subpage
*/

int period = 2000;
unsigned long time_now_1 = 0;
unsigned long time_now_2 = 0;
unsigned long time_now_3 = 0;
unsigned long time_now_4 = 0;
String string_temp, string_hum;

void loop(){

  DateTime now = rtc.now();

  if(pg == 0 && (millis() >= time_now_1 + period)){
    time_now_1 += period;
    Temp = dht.readTemperature();
    Humi = dht.readHumidity();
    Serial.println(String(Temp) + "||" + String(Humi));
    Serial.println(humi_val);
    Serial.println(temp_val);
    
    //    unsigned long timeBegin = micros();
    DS_temp.setWaitForConversion(false);      // makes it async
    DS_temp.requestTemperatures();
    DS_temp.setWaitForConversion(true);
    ds_temp = DS_temp.getTempCByIndex(0);
    
    //    unsigned long timeEnd = micros();
    //    unsigned long duration = timeEnd - timeBegin;
    //    Serial.println(duration / 1000);
  }

  if(EEPROM.read(16) > 9){
    string_temp = String(EEPROM.read(6)) + "." + String(EEPROM.read(16));
    temp_val = string_temp.toFloat();
  }

  else{
    string_temp = String(EEPROM.read(6)) + ".0" + String(EEPROM.read(16));
    temp_val = string_temp.toFloat();
  }
  //Serial.println(temp_val);

  if(EEPROM.read(17) > 9){
    string_hum = String(EEPROM.read(7)) + "." + String(EEPROM.read(17));
    humi_val = string_hum.toFloat();
  }
  else{
    string_hum = String(EEPROM.read(7)) + ".0" + String(EEPROM.read(17));
    humi_val = string_hum.toFloat();
  }

  //Serial.println(humi_val);
  //Serial.println(String(hr_set) + "|" + String(min_set) + "|" + String(sec_set));
  // Serial.println("RTC: " + String(now.hour()) + "|" + String(now.minute()) + "|" + String(now.second()));


  /*************** LED Light Time Control *******
   *   LED Light on / off time is set 24hr.  
   **********************************************/
  
  ontime_in_sec = hr_set * 60 + min_set ;
  offtime_in_sec = HR_set * 60 + MIN_set ;
  curr_time_in_sec = now.hour() * 60 + now.minute();
  
  if(offtime_in_sec = offtime_in_sec){
    digitalWrite(LED_LIGHT,LOW);
  } 
  
  if(ontime_in_sec < offtime_in_sec){
    
    if (curr_time_in_sec >= ontime_in_sec && curr_time_in_sec < offtime_in_sec){
      digitalWrite(LED_LIGHT, HIGH);
    }
    
    else if(curr_time_in_sec >= offtime_in_sec){
      digitalWrite(LED_LIGHT, LOW);
    }
    
    else{
      digitalWrite(LED_LIGHT, LOW);
    }
  }
  
  if(ontime_in_sec > offtime_in_sec){
    
    if (curr_time_in_sec >= ontime_in_sec && curr_time_in_sec <= 1440){
      digitalWrite(LED_LIGHT, HIGH);
    }
    
    else if(curr_time_in_sec < offtime_in_sec){
      digitalWrite(LED_LIGHT, HIGH);
    }
    
    else if(curr_time_in_sec >= offtime_in_sec && curr_time_in_sec < offtime_in_sec){
      digitalWrite(LED_LIGHT, LOW);
    }
  }


  /***** Cooling Fan Control ********************
  *
  *   Cooling Fan is operated based on Temperature & Humidity
  *   If temp or humi is over than setting temp or humi, then Cooling fan is on.
  *  
  ************************************************/
  
  if( Temp > temp_val || Humi > humi_val){
    digitalWrite(COOLING_FAN, HIGH);
    Serial.println("COOLING FAN_ON");
    //  c_fan_active = 1;
  }
  else{
    digitalWrite(COOLING_FAN, LOW);
  }

  /****** + 1min
    if(c_fan_active == 1 && ((millis() / 1000) >= time_now_2 + 1)){
    time_now_2 += 1;
    tick_3 += 1;

    if(tick_3 <= 300){
      digitalWrite(COOLING_FAN, HIGH);
      Serial.println("COOLING FAN_ON");
    }
    if(tick_3 == 300){
      c_fan_active = 0;
      tick_3 = 0;
      digitalWrite(COOLING_FAN, LOW);
      Serial.println("COOLING FAN_OFF");
     // delay(1000);
    }
    }

  ***/

  /*************** Air fan ********************
  *
  *    
  ************************************************/

  time_in_sec_1 = min1_set * 60 + sec1_set;
  time_in_sec_2 = MIN1_set * 60 + SEC1_set;
  time_in_sec_3 = min2_set * 60 + sec2_set;
  time_in_sec_4 = MIN2_set * 60 + SEC2_set;

  if(fan_state_1 == 1 && ((millis() / 1000) >= time_now_3 + 1)){
    time_now_3 += 1;
    tick_1 += 1;
    // Serial.println(tick_1);
    
    if(tick_1 <= time_in_sec_1){
      digitalWrite(AIR_FAN, HIGH);
      // Serial.println("air fan on");
    }
    
    if(tick_1 == time_in_sec_1){
      fan_state_1 = 0;
      tick_1 = 0;
    }
  }
  
  if(fan_state_1 == 0 && ((millis() / 1000) >= time_now_3 + 1)){
    time_now_3 += 1;
    tick_1 += 1;
    Serial.println(tick_1);
    
    if(tick_1 <= time_in_sec_2){
      digitalWrite(AIR_FAN, LOW);
      //Serial.println("air fan off");
    }
    
    if(tick_1 == time_in_sec_2){
      fan_state_1 = 1;
      tick_1 = 0;
    }
  }

  /*************** Oxygen Pump Control ***********
  *
  *     
  ************************************************/

  if(fan_state_2 == 1 && ((millis() / 1000) >= time_now_4 + 1)){
    time_now_4 += 1;
    tick_2 += 1;
    //Serial.println(tick_2);
    
    if(tick_2 <= time_in_sec_3){
      digitalWrite(OXY_PUMP, HIGH);
      // Serial.println("ox_on");
    }
    
    if (tick_2 == time_in_sec_3){
      fan_state_2 = 0;
      tick_2 = 0;
    }
  }
  
  if(fan_state_2 == 0 && ((millis() / 1000) >= time_now_4 + 1)){
    time_now_4 += 1;
    tick_2 += 1;
    // Serial.println(tick_2);
    
    if(tick_2 <= time_in_sec_4){
      digitalWrite(OXY_PUMP, LOW);
      // Serial.println("ox_off");
    }
    
    if(tick_2 == time_in_sec_4){
      fan_state_2 = 1;
      tick_2 = 0;
    }
  }
  
  button.tick();
  enc_config();
  def_page();
  setting_pg();
  led_light_pg();
  cooling_fan_pg();
  Air_fan_pg();
  oxy_motor_pg();
  eeprom_update;

}

void eeprom_update(){
  hr_set = EEPROM.read(0);
  min_set = EEPROM.read(1);
  sec_set = EEPROM.read(2);
  HR_set = EEPROM.read(3);
  MIN_set = EEPROM.read(4);
  SEC_set = EEPROM.read(5);

  temp_set = EEPROM.read(6);
  humi_set = EEPROM.read(7);
  temp_dec_set = EEPROM.read(16);
  humi_dec_set = EEPROM.read(17);

  min1_set = EEPROM.read(8);
  sec1_set = EEPROM.read(9);
  MIN1_set = EEPROM.read(10);
  SEC1_set = EEPROM.read(11);

  min2_set = EEPROM.read(12);
  sec2_set = EEPROM.read(13);
  MIN2_set = EEPROM.read(14);
  SEC2_set = EEPROM.read(15);
}
