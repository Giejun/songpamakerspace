/**
 *
 * Design for operate Smart Farm using LED, FAN, and Sensors.
 * Board: Mega2560
 * Sensors: DHT22, DS18B20, Water Levels, DS3231
 * Actuators: Fan, Water Motor, LED Light Relay
 * Display : 2004 LCD, Rotary Encoder
 *
 */

//******************************* Include libraries *****************************-//
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EEPROM.h>

#define BUZZER         13                           // buzzer alarm, 9, 12 lack of water
#define WATER_TANK     12                           // water level in tank
#define ONE_WIRE_BUS   /*3*/ 7                      // DS18B20 water Temperature
#define DHTPIN         /*4*/ 8                      // DHT22 Temp & Humi Sensor
#define FAN            /*8*/ 39                     // to Control temp & Humidity use Fan FAN for Temp & Humi
#define LEDRelay       /*9*/ 31                     // LED Light & Light Switch
#define TOUCHSW        10                           // for LED Light ON OFF
#define OXY            /*1*/ 41                     // Supply Oxygent to water
#define WATER_PUMP     /*2*/ 37                     // Supply Water from Tank  to Root

#define ENCODER_PIN_DT  2
#define ENCODER_PIN_CLK 3
#define ENCODER_PIN_SW  4

#define EEPROM_SETTINGS_ADDR  0


// LCD&DS3231 using I2C interfacing so, using SDA(20), SCL(21)
// DHT22 using PIN 8
// LCD I2C address 0X27
// ENCODER(DT,CLK,SW) 2, 3, 4

/************ Menu ************/
enum TMenu_Items_Ndx {ITEM_MAIN, ITEM_MENU, ITEM_LED, ITEM_PUMP, ITEM_FAN, ITEM_OXY, ITEM_BACK,
                      ITEM_LED_SET, ITEM_LED_ON, ITEM_LED_OFF, ITEM_LED_BACK,
					             ITEM_LED_ON_H_SET, ITEM_LED_ON_M_SET, ITEM_LED_ON_S_SET, ITEM_LED_OFF_H_SET, ITEM_LED_OFF_M_SET, ITEM_LED_OFF_S_SET, 
                      ITEM_PUMP_SET, ITEM_PUMP_ON, ITEM_PUMP_OFF, ITEM_PUMP_BACK,
					             ITEM_PUMP_ON_M_SET, ITEM_PUMP_OFF_M_SET,
                      ITEM_FAN_SET, ITEM_FAN_ON, ITEM_FAN_OFF, ITEM_FAN_BACK,
					              ITEM_FAN_ON_M_SET, ITEM_FAN_OFF_M_SET,
                      ITEM_OXY_SET, ITEM_OXY_ON, ITEM_OXY_OFF, ITEM_OXY_BACK,
					             ITEM_OXY_ON_M_SET, ITEM_OXY_OFF_M_SET,
					             END};
typedef struct{
  enum TMenu_Items_Ndx item_parent;
  enum TMenu_Items_Ndx item_child;
  enum TMenu_Items_Ndx item_prev;
  enum TMenu_Items_Ndx item_next;
  enum TMenu_Items_Ndx item_start;
  uint8_t items_qty;
  uint16_t * param;
  char * text;
} 
TMenu_Item_Strct;

typedef struct{
	enum TMenu_Items_Ndx current_ndx;
	uint8_t selected_ndx;
	enum TMenu_Items_Ndx item_prev;
} 
TMenu_Aux_Struct;

typedef struct
{
  uint16_t OnHour;                           // LED Light On Hour
  uint16_t OnMin;                            // LED Light On Minute
  uint16_t OnSec;                            // LED Light On Second
  uint16_t OffHour;                          // LED Light Off Hour
  uint16_t OffMin;                           // LED Light Off Minute
  uint16_t OffSec;                           // LED Light Off Second

  unsigned int pumpOnTime;                 // Control Water Pump ON TIME, SET TIME USE ENCODER
  uint16_t pumpOnMin;
  unsigned int pumpOffTime;                // Control Water Pump OFF TIME, SET TIME USE ENCODER
  uint16_t pumpOffMin;
  unsigned long pumpPreMillis;
  boolean pumpState;

  uint16_t fanOnMin;
  uint16_t fanOffMin;

  unsigned int OxyOnTime;                 // Control Oxygen relay ON TIME, SET TIME USE ENCODER
  uint16_t oxyOnMin;
  unsigned int OxyOffTime;                // Control Oxygen relay OFF TIME, SET TIME USE ENCODER
  uint16_t oxyOffMin;
  unsigned long OxyPreMillis;
  boolean OxyState;
} TSettings_Struct;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(ENCODER_PIN_DT, ENCODER_PIN_CLK);

LiquidCrystal_I2C lcd(0x27,20,4);             // set the LCD address to 0x27 for a 16 chars and 2 line display
#define DS3231_I2C_ADDRESS 104                // DS3231's general addres ; 0x68, 0x57 to change decimal 0x57 is 87, 0x68 is 104

int BUZZER_State = LOW;                       //D13 Buzzer & Buzzer sound
const int onDuration = 1000;
const int periodDuration = 5000;
unsigned long lastPeriodStart;

OneWire oneWire(ONE_WIRE_BUS);                //D7, DS18B20 Temperature Sensor
DallasTemperature waterTemp(&oneWire);

#define DHTTYPE DHT22                         //D8, DHT22 Temp & Humi sensor
DHT dht(DHTPIN, DHTTYPE);


/****************************** Review & develop ******************************/
//temperature and humidity is over maxTemp or maxHumi, then Fan is ON

int FAN_readValue;                            // Fan for controlling temp & humi, and supply oxygen to leaf.
int maxHum = 30;                              // TO BE SET VALUE BY USED ROTARY ENCODER
int maxTemp = 25;



// LED Light set and restriced by touch switch
int LED_readValue;                            //D31 Relay for LED Light controlling

boolean TOUCHSW_currentState = LOW;           //LED Light is ON/OFF
boolean TOUCHSW_lastState = LOW;
boolean LEDRelay_state = LOW;

uint16_t OnHour = 18;                           // LED Light On Hour
uint16_t OnMin = 00;                            // LED Light On Minute
uint16_t OnSec = 00;                            // LED Light On Second
uint16_t OffHour = 22;                          // LED Light Off Hour
uint16_t OffMin = 03;                           // LED Light Off Minute
uint16_t OffSec = 00;                           // LED Light Off Second

byte seconds, minutes, hours, day, date, month, year;
char weekDay[4];
byte tMSB, tLSB;
float my_temp;
char my_array[100];                                      // Character array for printing something.


/*********** develop PUMP, OXY set on off time with encoder  *****************/

const unsigned int pumpOnTime = 10*1000;                 // Control Water Pump ON TIME, SET TIME USE ENCODER
uint16_t pumpOnMin = 10;
const unsigned int pumpOffTime = 30*1000;                // Control Water Pump OFF TIME, SET TIME USE ENCODER
uint16_t pumpOffMin = 15;
unsigned long pumpPreMillis = 0;
int pumpInterval = pumpOnTime;
boolean pumpState = true;

uint16_t fanOnMin = 20;
uint16_t fanOffMin = 25;

const unsigned int OxyOnTime = 5*1000;                 // Control Oxygen relay ON TIME, SET TIME USE ENCODER
uint16_t oxyOnMin = 30;
const unsigned int OxyOffTime = 20*1000;                // Control Oxygen relay OFF TIME, SET TIME USE ENCODER
uint16_t oxyOffMin = 35;
unsigned long OxyPreMillis = 0;
int OxyInterval = OxyOnTime;
boolean OxyState = true;

TSettings_Struct settings;
uint8_t settings_changed;

TMenu_Item_Strct menu[] = {{.item_parent = ITEM_MAIN, .item_child = ITEM_MENU, .item_prev = ITEM_MAIN, .item_next = ITEM_MENU, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_MAIN, .item_child = END, .item_prev = END, .item_next = END, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_MAIN, .item_child = ITEM_LED_SET, .item_prev = ITEM_BACK, .item_next = ITEM_PUMP, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Set LED1 Light time"},
                           {.item_parent = ITEM_MAIN, .item_child = ITEM_PUMP_SET, .item_prev = ITEM_LED, .item_next = ITEM_FAN, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Set Pump"},
                           {.item_parent = ITEM_MAIN, .item_child = ITEM_FAN_SET, .item_prev = ITEM_PUMP, .item_next = ITEM_OXY, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Set Fan"},
                           {.item_parent = ITEM_MAIN, .item_child = ITEM_OXY_SET, .item_prev = ITEM_FAN, .item_next = ITEM_BACK, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Set Oxy"},
                           {.item_parent = ITEM_MAIN, .item_child = ITEM_MAIN, .item_prev = ITEM_OXY, .item_next = ITEM_LED, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Back"},
                           {.item_parent = ITEM_LED, .item_child = END, .item_prev = END, .item_next = END, .item_start = ITEM_LED_ON, .items_qty = 3, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_LED_ON, .item_child = ITEM_LED_ON_H_SET, .item_prev = ITEM_LED_BACK, .item_next = ITEM_LED_OFF, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "LED1 ON:    :  :"},
                           {.item_parent = ITEM_LED_OFF, .item_child = ITEM_LED_OFF_H_SET, .item_prev = ITEM_LED_ON, .item_next = ITEM_LED_BACK, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "LED1 OFF:   :  :"},
                           {.item_parent = ITEM_LED, .item_child = ITEM_MENU, .item_prev = ITEM_LED_OFF, .item_next = ITEM_LED_ON, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Back"},
						   {.item_parent = ITEM_LED_ON, .item_child = ITEM_LED_ON_M_SET, .item_prev = ITEM_LED_ON, .item_next = ITEM_LED_ON, .item_start = ITEM_LED_ON_H_SET, .items_qty = 1, .param = &(settings.OnHour), .text = NULL},
						   {.item_parent = ITEM_LED_ON, .item_child = ITEM_LED_ON_S_SET, .item_prev = ITEM_LED_ON, .item_next = ITEM_LED_ON, .item_start = ITEM_LED_ON_M_SET, .items_qty = 1, .param = &(settings.OnMin), .text = NULL},
						   {.item_parent = ITEM_LED_ON, .item_child = ITEM_LED_SET, .item_prev = ITEM_LED_ON, .item_next = ITEM_LED_ON, .item_start = ITEM_LED_ON_S_SET, .items_qty = 1, .param = &(settings.OnSec), .text = NULL},
						   {.item_parent = ITEM_LED_OFF, .item_child = ITEM_LED_OFF_M_SET, .item_prev = ITEM_LED_OFF, .item_next = ITEM_LED_ON, .item_start = ITEM_LED_OFF_H_SET, .items_qty = 1, .param = &(settings.OffHour), .text = NULL},
						   {.item_parent = ITEM_LED_OFF, .item_child = ITEM_LED_OFF_S_SET, .item_prev = ITEM_LED_OFF, .item_next = ITEM_LED_ON, .item_start = ITEM_LED_OFF_M_SET, .items_qty = 1, .param = &(settings.OffMin), .text = NULL},
						   {.item_parent = ITEM_LED_OFF, .item_child = ITEM_LED_SET, .item_prev = ITEM_LED_OFF, .item_next = ITEM_LED_OFF, .item_start = ITEM_LED_OFF_S_SET, .items_qty = 1, .param = &(settings.OffSec), .text = NULL},
                           {.item_parent = ITEM_PUMP, .item_child = END, .item_prev = END, .item_next = END, .item_start = ITEM_PUMP_ON, .items_qty = 3, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_PUMP, .item_child = ITEM_PUMP_ON_M_SET, .item_prev = ITEM_BACK, .item_next = ITEM_PUMP_OFF, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "ON:      min"},
                           {.item_parent = ITEM_PUMP, .item_child = ITEM_PUMP_OFF_M_SET, .item_prev = ITEM_PUMP_ON, .item_next = ITEM_BACK, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "OFF:     min"},
                           {.item_parent = ITEM_PUMP, .item_child = ITEM_MENU, .item_prev = ITEM_PUMP_OFF, .item_next = ITEM_PUMP_ON, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Back"},
						   {.item_parent = ITEM_PUMP_ON, .item_child = ITEM_PUMP_SET, .item_prev = ITEM_PUMP_ON, .item_next = ITEM_PUMP_ON, .item_start = ITEM_PUMP_ON_M_SET, .items_qty = 1, .param = &(settings.pumpOnMin), .text = NULL},
						   {.item_parent = ITEM_PUMP_OFF, .item_child = ITEM_PUMP_SET, .item_prev = ITEM_PUMP_OFF, .item_next = ITEM_PUMP_OFF, .item_start = ITEM_PUMP_OFF_M_SET, .items_qty = 1, .param = &(settings.pumpOffMin), .text = NULL},
                           {.item_parent = ITEM_FAN, .item_child = END, .item_prev = END, .item_next = END, .item_start = ITEM_FAN_ON, .items_qty = 3, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_FAN, .item_child = ITEM_FAN_ON_M_SET, .item_prev = ITEM_BACK, .item_next = ITEM_FAN_OFF, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "ON:      min"},
                           {.item_parent = ITEM_FAN, .item_child = ITEM_FAN_OFF_M_SET, .item_prev = ITEM_FAN_ON, .item_next = ITEM_BACK, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "OFF:     min"},
                           {.item_parent = ITEM_FAN, .item_child = ITEM_MENU, .item_prev = ITEM_FAN_OFF, .item_next = ITEM_FAN_ON, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Back"},
						   {.item_parent = ITEM_FAN_ON, .item_child = ITEM_FAN_SET, .item_prev = ITEM_FAN_ON, .item_next = ITEM_FAN_ON, .item_start = ITEM_FAN_ON_M_SET, .items_qty = 1, .param = &(settings.fanOnMin), .text = NULL},
						   {.item_parent = ITEM_FAN_OFF, .item_child = ITEM_FAN_SET, .item_prev = ITEM_FAN_OFF, .item_next = ITEM_FAN_OFF, .item_start = ITEM_FAN_OFF_M_SET, .items_qty = 1, .param = &(settings.fanOffMin), .text = NULL},
                           {.item_parent = ITEM_OXY, .item_child = END, .item_prev = END, .item_next = END, .item_start = ITEM_OXY_ON, .items_qty = 3, .param = NULL, .text = NULL},
                           {.item_parent = ITEM_OXY, .item_child = ITEM_OXY_ON_M_SET, .item_prev = ITEM_BACK, .item_next = ITEM_OXY_OFF, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "ON:      min"},
                           {.item_parent = ITEM_OXY, .item_child = ITEM_OXY_OFF_M_SET, .item_prev = ITEM_OXY_ON, .item_next = ITEM_BACK, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "OFF:     min"},
                           {.item_parent = ITEM_OXY, .item_child = ITEM_MENU, .item_prev = ITEM_OXY_OFF, .item_next = ITEM_OXY_ON, .item_start = ITEM_LED, .items_qty = 5, .param = NULL, .text = "Back"},
						   {.item_parent = ITEM_OXY_ON, .item_child = ITEM_OXY_SET, .item_prev = ITEM_OXY_ON, .item_next = ITEM_OXY_ON, .item_start = ITEM_OXY_ON_M_SET, .items_qty = 1, .param = &(settings.oxyOnMin), .text = NULL},
						   {.item_parent = ITEM_OXY_OFF, .item_child = ITEM_OXY_SET, .item_prev = ITEM_OXY_OFF, .item_next = ITEM_OXY_OFF, .item_start = ITEM_OXY_OFF_M_SET, .items_qty = 1, .param = &(settings.oxyOffMin), .text = NULL}
                          };
TMenu_Aux_Struct menu_aux_struct = {.current_ndx = ITEM_MAIN, .selected_ndx = ITEM_MAIN, .item_prev = ITEM_MAIN};

float h, t, f;

void printMenuStruct (void)
{
  for (uint8_t i = 0; i < sizeof (menu) / sizeof(TMenu_Item_Strct); i++)
  {
    Serial.print("\r\n");
    Serial.print(i);
    Serial.print(": Parent=");
    Serial.print(menu[i].item_parent);
    Serial.print(" Child=");
    Serial.print(menu[i].item_child);
    Serial.print(" Prev=");
    Serial.print(menu[i].item_prev);
    Serial.print(" Next=");
    Serial.print(menu[i].item_next);
    Serial.print(' ');
    Serial.println(menu[i].text);
  }

  return;
}

int settingsLoad (void)
{
  int result = 0;

  EEPROM.get(EEPROM_SETTINGS_ADDR, settings);

  return result;
}

int settingsSave (void)
{
  int result = 0;

  EEPROM.put(EEPROM_SETTINGS_ADDR, settings);

  return result;
}

int settingsCheck (void)
{
  int result = 0;
  if ( (settings.fanOffMin > 500) || (settings.fanOnMin > 500) || (settings.oxyOffMin > 500) || (settings.oxyOnMin > 500) || (settings.pumpOffMin > 500) || (settings.pumpOnMin > 500) )
	result = -1;
	
  if ((settings.OffHour > 23) || (settings.OnHour > 23))
	result = -2;
	
  if ((settings.OffMin > 59) || (settings.OnMin > 59) || (settings.OffSec > 59) || (settings.OnSec > 59))
    result = -3;

  return result;
}

void settingsInitDefault (void)
{
	settings.fanOffMin = fanOffMin;
	settings.fanOnMin = fanOnMin;
	settings.OffHour = OffHour;
	settings.OffMin = OffMin;
	settings.OffSec = OffSec;
	settings.OnHour = OnHour;
	settings.OnMin = OnMin;
	settings.OnSec = OnSec;
	settings.oxyOffMin = oxyOffMin;
	settings.OxyOffTime = OxyOffTime;
	settings.oxyOnMin = oxyOnMin;
	settings.OxyOnTime = OxyOnTime;
	settings.OxyPreMillis = OxyPreMillis;
	settings.OxyState = OxyState;
	settings.pumpOffMin = pumpOffMin;
	settings.pumpOffTime = pumpOffTime;
	settings.pumpOnMin = pumpOnMin;
	settings.pumpOnTime = pumpOnTime;
	settings.pumpPreMillis = pumpPreMillis;
	settings.pumpState = pumpState;
	
	return;
}

/*
void settingsPrint (void)
{
	Serial.print("\r\nSettings:");
	Serial.print("\r\n\t fanOffMin=");
	Serial.print(settings.fanOffMin);
	Serial.print("\r\n\t fanOnMin=");
	Serial.print(settings.fanOnMin);
	Serial.print("\r\n\t OffHour=");
	Serial.print(settings.OffHour);
	Serial.print("\r\n\t OffMin=");
	Serial.print(settings.OffMin);
	Serial.print("\r\n\t OffSec=");
	Serial.print(settings.OffSec);
	Serial.print("\r\n\t OnHour=");
	Serial.print(settings.OnHour);
	Serial.print("\r\n\t OnMin=");
	Serial.print(settings.OnMin);
	Serial.print("\r\n\t OnSec=");
	Serial.print(settings.OnSec);
	Serial.print("\r\n\t oxyOffMin=");
	Serial.print(settings.oxyOffMin);
	Serial.print("\r\n\t OxyOffTime=");
	Serial.print(settings.OxyOffTime);
	Serial.print("\r\n\t oxyOnMin=");
	Serial.print(settings.oxyOnMin);
	Serial.print("\r\n\t OxyOnTime=");
	Serial.print(settings.OxyOnTime);
	Serial.print("\r\n\t OxyPreMillis=");
	Serial.print(settings.OxyPreMillis);
	Serial.print("\r\n\t OxyState=");
	Serial.print(settings.OxyState);
	Serial.print("\r\n\t pumpOffMin=");
	Serial.print(settings.pumpOffMin);
	Serial.print("\r\n\t pumpOffTime=");
	Serial.print(settings.pumpOffTime);
	Serial.print("\r\n\t pumpOnMin=");
	Serial.print(settings.pumpOnMin);
	Serial.print("\r\n\t pumpOnTime=");
	Serial.print(settings.pumpOnTime);
	Serial.print("\r\n\t pumpPreMillis=");
	Serial.print(settings.pumpPreMillis);
	Serial.print("\r\n\t pumpState=");
	Serial.print(settings.pumpState);
	
	return;
}

*/

void displayProcess (void)
{
  int32_t encoder_value = myEnc.read();
  uint32_t button_timer_ms, blink_timer_ms;
  static int8_t encoder_sw_prev = 0, menu_changed = 0, first_line = 0, blinkEn = 0;
  uint8_t encoder_sw;
  uint8_t menu_ndx = 0;
  uint8_t encoder_active = 0;
  int8_t encoder_step;

  if (millis() >= button_timer_ms)
  {
	encoder_sw = digitalRead(ENCODER_PIN_SW) ? 0 : 1;
	if (!encoder_sw || encoder_sw_prev)
  {
    button_timer_ms = millis() + 200;
	  encoder_sw = 0;
  }
  else
    button_timer_ms = millis() + 500;
	encoder_sw_prev = digitalRead(ENCODER_PIN_SW) ? 0 : 1;
  }
  encoder_value /= 4;
  if (encoder_value)
    myEnc.write(0);

  switch (menu_aux_struct.current_ndx)
  {
    case ITEM_MAIN:
		if (!encoder_sw)
		{
			//---------------------------------------- Display LCD 2004 ----------------------------------------//
	        sprintf(my_array, "%d:%d:%d", hours, minutes, seconds);
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(0, 0);
			lcd.print(weekDay);
			lcd.print(",");
			lcd.print(date, DEC);
			lcd.print("/");
			lcd.print(month, DEC);
			lcd.print("/");
			lcd.print(year, DEC);
			lcd.setCursor(12, 0);
			lcd.print(my_array);
			lcd.setCursor(1, 2);
			lcd.print("T:");
			lcd.print(t);
			lcd.print("C");
			lcd.setCursor(11, 2);
			lcd.print("H:");
			lcd.print(h);
			lcd.print("%");
			lcd.setCursor(1, 3);
			lcd.print("WT:");
			lcd.print(waterTemp.getTempCByIndex(0));
			delay(1000);
		}
		else
		{
			lcd.clear();
			lcd.backlight();
			menu_aux_struct.current_ndx = menu[menu_aux_struct.current_ndx].item_child;
			menu_aux_struct.selected_ndx = menu[menu_aux_struct.current_ndx].item_start;
			//menu_current_ndx = menu[menu_current_ndx].item_child;
			menu_changed = 1;
			Serial.print("\r\nNext ndx=");
			Serial.print(menu_aux_struct.current_ndx);
      encoder_sw = 0;
		}
		break;

	case ITEM_MENU:
  case ITEM_LED_SET:
  case ITEM_PUMP_SET:
  case ITEM_FAN_SET:
  case ITEM_OXY_SET:
			encoder_active = 0;
			encoder_step = (encoder_value > 0) ? 1 : -1;
			while (encoder_value)
			{
				/*
				Serial.print("\r\nencoder_value=");
				Serial.print(encoder_value);
				*/
				if (!encoder_active)
				{
					lcd.setCursor(0, menu_aux_struct.selected_ndx - menu[menu_aux_struct.current_ndx].item_start - first_line);
					lcd.print(' ');
					encoder_active = 1;
				}
				menu_aux_struct.selected_ndx += encoder_step;
				if (menu_aux_struct.selected_ndx < menu[menu_aux_struct.current_ndx].item_start)
					menu_aux_struct.selected_ndx += menu[menu_aux_struct.selected_ndx].items_qty;
				else
				{
					if (menu_aux_struct.selected_ndx >= (menu[menu_aux_struct.current_ndx].item_start + menu[menu_aux_struct.current_ndx].items_qty))
						menu_aux_struct.selected_ndx -= menu[menu_aux_struct.current_ndx].items_qty;
				}
				encoder_value -= encoder_step;
				/*
				Serial.print("\r\nSelected ndx=");
				Serial.print(menu_aux_struct.selected_ndx);
				*/
			}

			if ((menu_aux_struct.selected_ndx -  menu[menu_aux_struct.current_ndx].item_start) >= (first_line + 4))
			{
				first_line++;
				menu_changed = 1;
			}
			else
			{
				if (first_line)
				{
				  if ((menu_aux_struct.selected_ndx -  menu[menu_aux_struct.current_ndx].item_start) < first_line)
				  {
				    first_line--;
				    menu_changed = 1;
				  }
				}
			}
			if (encoder_active && !menu_changed)
			{
				lcd.setCursor(0, menu_aux_struct.selected_ndx - menu[menu_aux_struct.current_ndx].item_start - first_line);
				lcd.print('>');
			}
			if (menu_changed)
			{
				lcd.clear();
				menu_ndx = menu[menu_aux_struct.current_ndx].item_start + first_line;
				uint8_t child_ndxes[3];
				for (uint8_t i = first_line, line = 0; i < (first_line + 4); i++, line++)
				{
					lcd.setCursor(0, line);
					if (menu_aux_struct.selected_ndx == (menu[menu_aux_struct.current_ndx].item_start + i))
					lcd.print('>');
					else
					lcd.print(' ');
					/*
					Serial.print("\r\nMenu[");
					Serial.print(menu_ndx);
					Serial.print("]=");
					Serial.print(menu[menu_ndx].text);
					*/
					lcd.print(menu[menu_ndx].text);
					child_ndxes[0] = menu[menu_ndx].item_child;
					child_ndxes[1] = menu[child_ndxes[0]].item_child;
					child_ndxes[2] = menu[child_ndxes[1]].item_child;
					if ((menu[child_ndxes[0]].param != NULL) && (menu[child_ndxes[1]].param != NULL))
					{
						lcd.setCursor(11, line);
						if ((*(menu[child_ndxes[0]].param) / 10) == 0)
							lcd.print('0');
						lcd.print(*(menu[child_ndxes[0]].param));
						lcd.print(':');
						if ((*(menu[child_ndxes[1]].param) / 10) == 0)
							lcd.print('0');
						lcd.print(*(menu[child_ndxes[1]].param));
						lcd.print(':');
						if ((*(menu[child_ndxes[2]].param) / 10) == 0)
							lcd.print('0');
						lcd.print(*(menu[child_ndxes[2]].param));
					}
					else
					{
						if (menu[child_ndxes[0]].param != NULL)
						{
							lcd.setCursor(6, line);
              if ((*(menu[child_ndxes[0]].param) / 100) == 0)
               lcd.print('0');
							if ((*(menu[child_ndxes[0]].param) / 10) == 0)
								lcd.print('0');
							lcd.print(*(menu[child_ndxes[0]].param));
						}
					}
					menu_ndx = menu[menu_ndx].item_next;
					if ((menu_ndx == END) || (i >= (menu[menu_aux_struct.current_ndx].items_qty - 1)))
						break;
				}
				menu_changed = 0;
			}
		  break;

    case ITEM_LED_ON_H_SET:
	case ITEM_LED_ON_M_SET:
	case ITEM_LED_ON_S_SET:
    case ITEM_LED_OFF_H_SET:
	case ITEM_LED_OFF_M_SET:
	case ITEM_LED_OFF_S_SET:
    case ITEM_PUMP_ON_M_SET:
    case ITEM_PUMP_OFF_M_SET:
    case ITEM_FAN_ON_M_SET:
    case ITEM_FAN_OFF_M_SET:
    case ITEM_OXY_ON_M_SET:
    case ITEM_OXY_OFF_M_SET:
      encoder_active = 0;
      encoder_step = (encoder_value > 0) ? 1 : -1;
      int8_t line, column, thr;
      int16_t param_value = 0, upper_limit = 500;
      uint8_t param_value_8 = 1;
      param_value = *(menu[menu_aux_struct.current_ndx].param);
      while (encoder_value)
      {
		/*
        Serial.print("\r\nencoder_value=");
        Serial.print(encoder_value);
		*/
        encoder_active = 1;
        if (menu[menu_aux_struct.current_ndx].param)
        {
          param_value += encoder_step;
          if (param_value < 0)
            param_value = 0;
          if ( (menu_aux_struct.current_ndx == ITEM_LED_ON_H_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_H_SET) )
            upper_limit = 23;
          if ( (menu_aux_struct.current_ndx == ITEM_LED_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_M_SET) || 
               (menu_aux_struct.current_ndx == ITEM_LED_ON_S_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_S_SET) )
            upper_limit = 59;
          if (param_value > upper_limit)
            param_value = upper_limit;
          *(menu[menu_aux_struct.current_ndx].param) = param_value;
        }
        encoder_value -= encoder_step;
		/*
        Serial.print("\r\nParameter value=");
        Serial.print(param_value);
		*/
      }
      if ( (menu_aux_struct.current_ndx == ITEM_LED_ON_H_SET) || (menu_aux_struct.current_ndx == ITEM_LED_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_LED_ON_S_SET) ||
			(menu_aux_struct.current_ndx == ITEM_PUMP_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_FAN_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_OXY_ON_M_SET) )
        line = 0;
      else
        line = 1;
	  if ((menu_aux_struct.current_ndx == ITEM_LED_ON_H_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_H_SET))
		column = 11;
	  else if ((menu_aux_struct.current_ndx == ITEM_LED_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_M_SET))
	    column = 14;
	  else if ((menu_aux_struct.current_ndx == ITEM_LED_ON_S_SET) || (menu_aux_struct.current_ndx == ITEM_LED_OFF_S_SET))
		column = 17;
	  else
	    column = 6;
    if ( (menu_aux_struct.current_ndx == ITEM_PUMP_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_PUMP_OFF_M_SET) || (menu_aux_struct.current_ndx == ITEM_FAN_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_FAN_OFF_M_SET) ||
         (menu_aux_struct.current_ndx == ITEM_OXY_ON_M_SET) || (menu_aux_struct.current_ndx == ITEM_OXY_OFF_M_SET) )
         param_value_8 = 0;
	  if (!blinkEn)
	  {
        if (column != 6)
	  	    lcd.setCursor(column+1, line);
        else
          lcd.setCursor(column+2, line);
	    lcd.blink();
	    blinkEn = 1;
	  }
      if (encoder_active && menu[menu_aux_struct.current_ndx].param != NULL)
      {
        lcd.noBlink();
        blinkEn = 0;
        lcd.setCursor(column, line);
        if (0 == param_value_8)
        {
          if ((*(menu[menu_aux_struct.current_ndx].param) / 100) == 0)
            lcd.print('0');
        }
        if ((*(menu[menu_aux_struct.current_ndx].param) / 10) == 0)
          lcd.print('0');
        lcd.print(*(menu[menu_aux_struct.current_ndx].param));
      }
	  settings_changed = 1;
      break;
    

    default: break;
  }
  
  if (encoder_sw)
  {
	  /*
      Serial.print("\r\nCurrent ndx=");
      Serial.print(menu_aux_struct.current_ndx);
	  */
      menu_aux_struct.current_ndx = menu[menu_aux_struct.selected_ndx].item_child;
      menu_aux_struct.selected_ndx = menu[menu_aux_struct.current_ndx].item_start;
      first_line = 0;
      menu_changed = 1;
      lcd.noBlink();
      blinkEn = 0;
	  /*
      Serial.print("\r\nNext ndx=");
      Serial.print(menu_aux_struct.current_ndx);
	  */
      encoder_sw = 0;
  }
  
  if (settings_changed)
  {
    settingsSave();
	  settings_changed = 0;
  }

  return;
}

void setup(){
  Serial.begin(57600);

  pinMode(BUZZER, OUTPUT);
  pinMode(WATER_TANK, INPUT_PULLUP);
  pinMode(FAN, OUTPUT);
  pinMode(LEDRelay,OUTPUT);
  pinMode(TOUCHSW, INPUT);
  pinMode(OXY, OUTPUT);
  pinMode(WATER_PUMP,OUTPUT);
  pinMode(ENCODER_PIN_SW, INPUT_PULLUP);
  digitalWrite(LEDRelay, LOW);

  Wire.begin();
  waterTemp.begin();
  dht.begin();

  lcd.begin();                             // initialize the lcd
  lcd.backlight();
  lcd.setCursor(2,1);
  lcd.print("www.DIYALL.TECH");
  lcd.setCursor(10,3);
  lcd.print("ver: 0.9");
  delay(3000);
  lcd.clear();

  settingsLoad();
  if (settingsCheck())
  {
    Serial.print("\r\nSettings check failed. Init default settings");
    settingsInitDefault();
    settingsSave();
  }
 // settingsPrint();
  settings_changed = 0;

  //printMenuStruct();
  }

uint32_t dht_read_timer_ms = 0;   //00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
void loop(){
  //---------------------------------------------- DS18B20 ------------------------------------------------------//

  waterTemp.requestTemperatures();
  Serial.print("Temperature is: ");
  Serial.println(waterTemp.getTempCByIndex(0));                        // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.println(myEnc.read());
  Serial.println(digitalRead(ENCODER_PIN_SW));

  displayProcess();

  //---------------------------------------------- DHT22 ------------------------------------------------------//
  if (millis() >= dht_read_timer_ms)
  {
    dht_read_timer_ms = millis() + 3000; //00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f))
    {                                                      // Check if any reads failed and exit early (to try again).
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    if(h > maxHum || t > maxTemp)
    {                                    // FAN CONTROLED
      digitalWrite(FAN, HIGH);
    }
    else
    {
      // 정기적 펜 동작 코드 삽입*************************************************************************************
      digitalWrite(FAN, LOW);
    }

    Serial.print("T:");
    Serial.print(t);
    Serial.println("C");
    Serial.print("H:");
    Serial.print(h);
    Serial.println(" %");
  }
  //---------------------------------------------- DS3231 RTC ------------------------------------------------------//
  watchConsole();
  get3231Date();
  my_temp = (float)get3231Temp();

  //---------------------------------------------- Serial.print -------------------------------------------------//
  Serial.print(weekDay);
  Serial.print(", ");
  Serial.print(date, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" - ");
  Serial.print(hours, DEC);
  Serial.print(":");
  Serial.print(minutes, DEC);
  Serial.print(":");
  Serial.println(seconds, DEC);

  Serial.print("RTC Temp: ");
  Serial.println(my_temp);

 Serial.print("DS18B20:");
 Serial.println(waterTemp.getTempCByIndex(0));

  // NOTE: Arduino does NOT implement printing floats to a string.
  // If you use the std C function : sprintf(my_array, "Temp: %4.2f", my_temp), It will NOT CONVERT.
  // So I abandoned this, since I don't need to print the float to the LCD anyway.


//---------------------------- LED Relay ; ON OFF by DS3231 RTC CLOCK -------------------------------//

/**
 * I'd like to control LED Relay 2 ways. one is TOUCHSW, the other is RTC CLOCK setting
 * 1. With TOUCHSW(10), I want to ON / OFF LED Realy, at any time. in other word,without rtc clock setting condition, when ledrelay is on, touch sw, then led is off
 *    when led is off, touch switch(10) then led relay ON.
 * 2. LED realy is operated based on rtc setting value. when I set 18:00 led relay off, and 09:00 led relay On, then led is on 09:00  to 18:00
 *
 * This issue is explained at this link :  https://arduino.stackexchange.com/questions/17477/creating-a-timer-using-a-real-time-clock-with-start-and-stop-times?answertab=votes#tab-top
 *
 */


/**
  TOUCHSW_currentState = digitalRead(TOUCHSW);
  if (TOUCHSW_currentState == HIGH && TOUCHSW_lastState == LOW){
     Serial.println("pressed");
     delay(1);

    if (LEDRelay_state == HIGH){
      digitalWrite(LEDRelay, LOW);
      LEDRelay_state = LOW;
     }
     else{
      digitalWrite(LEDRelay, HIGH);
      LEDRelay_state = HIGH;
    }
  }
  TOUCHSW_lastState = TOUCHSW_currentState;
**/

  if(hours == OnHour && minutes == OnMin){                        //using RTC colck, control LED Light
    digitalWrite(LEDRelay, HIGH);
    Serial.println("LIGHT ON");
  }
  else if(hours ==OffHour && minutes == OffMin){
    digitalWrite(LEDRelay, LOW);
    Serial.println("LIGHT OFF");
  }

//-------------------------------------------------- WATER TANK ALRAM -----------------------------------//

  int WATER_TANK_VAL = digitalRead(WATER_TANK);

 if(WATER_TANK_VAL == LOW){
    if(millis() - lastPeriodStart >= periodDuration) {
    lastPeriodStart += periodDuration;
    tone(BUZZER, 550, onDuration);
  }
}
  Serial.println(WATER_TANK_VAL);

//---------------------------------------- D37 Relay for water Pump --------------------//

digitalWrite(WATER_PUMP, pumpState);
unsigned long pumpCurrentMillis = millis();
if((unsigned long)(pumpCurrentMillis - pumpPreMillis) >= pumpInterval){
  if(pumpState){
    pumpInterval = pumpOffTime;
    Serial.println(pumpState);
  }
  else{
    pumpInterval = pumpOnTime;
  }
  pumpState = !(pumpState);
  pumpPreMillis = pumpCurrentMillis;
}

//---------------------------------------- D41 Relay for Oxygen --------------------//

digitalWrite(OXY, OxyState);
unsigned long OxyCurrentMillis = millis();
if((unsigned long)(OxyCurrentMillis - OxyPreMillis) >= pumpInterval){
  if(OxyState){
    OxyInterval = OxyOffTime;
    Serial.println(OxyState);
  }
  else{
   OxyInterval = OxyOnTime;
  }
  OxyState = !(OxyState);
  OxyPreMillis = OxyCurrentMillis;
}



//---------------------------------------- Display LCD 2004 ----------------------------------------//
 sprintf(my_array, "%d:%d:%d", hours, minutes, seconds);
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(weekDay);
  lcd.print(",");
  lcd.print(date, DEC);
  lcd.print("/");
  lcd.print(month, DEC);
  lcd.print("/");
  lcd.print(year, DEC);
  lcd.setCursor(12, 0);
  lcd.print(my_array);
  lcd.setCursor(1, 2);
  lcd.print("T:");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(11, 2);
  lcd.print("H:");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(1, 3);
  lcd.print("WT:");
  lcd.print(waterTemp.getTempCByIndex(0));
  delay(1000);
}


// ------------------------------ DS3231 RCT ; Convert normal decimal numbers to binary coded decimal ------------------//
byte decToBcd(byte val){
  return ((val/10*16) + (val%10));
  }

void watchConsole(){
  if (Serial.available()){                                 // Look for char in serial queue and process if found
    if (Serial.read() == 84) {                             //If command = "T" Set Date
      set3231Date();
      get3231Date();
      Serial.println(" ");
      }
     }
}

void set3231Date(){
    //T(sec)(min)(hour)(dayOfWeek)(dayOfMonth)(month)(year)
    //T(00-59)(00-59)(00-23)(1-7)(01-31)(01-12)(00-99)
    //Example: 02-Feb-09 @ 19:57:11 for the 3rd day of the week -> T1157193020209
    // T1124154091014
  seconds = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));              // Use of (byte) type casting and ascii math to achieve result.
  minutes = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  hours   = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  day     = (byte) (Serial.read() - 48);
  date    = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  month   = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  year    = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(decToBcd(seconds));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(date));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void get3231Date(){
  // send request to receive data starting at register 0
  Wire.beginTransmission(DS3231_I2C_ADDRESS);                 // 104 is DS3231 device address
  Wire.write(0x00); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);                    // request seven bytes

  if(Wire.available()){
    seconds = Wire.read(); // get seconds
    minutes = Wire.read(); // get minutes
    hours   = Wire.read();   // get hours
    day     = Wire.read();
    date    = Wire.read();
    month   = Wire.read(); //temp month
    year    = Wire.read();

    seconds = (((seconds & B11110000)>>4)*10 + (seconds & B00001111)); // convert BCD to decimal
    minutes = (((minutes & B11110000)>>4)*10 + (minutes & B00001111)); // convert BCD to decimal
    hours   = (((hours & B00110000)>>4)*10 + (hours & B00001111)); // convert BCD to decimal (assume 24 hour mode)
    day     = (day & B00000111); // 1-7
    date    = (((date & B00110000)>>4)*10 + (date & B00001111)); // 1-31
    month   = (((month & B00010000)>>4)*10 + (month & B00001111)); //msb7 is century overflow
    year    = (((year & B11110000)>>4)*10 + (year & B00001111));
  }
  else{
    //oh noes, no data!
  }

  switch (day) {
    case 1:
      strcpy(weekDay, "Sun");
      break;
    case 2:
      strcpy(weekDay, "Mon");
      break;
    case 3:
      strcpy(weekDay, "Tue");
      break;
    case 4:
      strcpy(weekDay, "Wed");
      break;
    case 5:
      strcpy(weekDay, "Thu");
      break;
    case 6:
      strcpy(weekDay, "Fri");
      break;
    case 7:
      strcpy(weekDay, "Sat");
      break;
  }
}

//-------------------------------- DS3231 Temperature Sensor -------------------------------//
float get3231Temp(){
  float temp3231;

  //temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

  if(Wire.available()){
    tMSB = Wire.read();                                             //2's complement int portion
    tLSB = Wire.read();                                             //fraction portion

    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
  }
  else{
    //oh noes, no data!
  }
  return temp3231;
}
