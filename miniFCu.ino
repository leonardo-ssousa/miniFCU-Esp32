#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Fonts/muHeavy8ptBold.h"
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);


//About
String setSSID = "";
String setPassword = "";
int encreaseValue = 5;
int debounceEncoder = 50;
bool isConnected = false;
bool isSleeping = false;

//Important
long lastMillis;
volatile int cursor = 0;
volatile int encoderValue = 0;
enum Screens {mixer, outputDevice, flightSim, sleep, config};

//Encoder
int CLK = 2;
int DT = 0;
int BTN = 16;
bool encoderIsValue = false; // False -> Cursor | True -> value


void IRAM_ATTR rotaryEncoderCLK(){
  int data = digitalRead(DT);

  if(millis() - lastMillis > debounceEncoder){
    //Sair do modo Sleep:
    isSleeping = false; 

    if(data == 1){

      switch (encoderIsValue) {
      case false: 
        cursor--; 
        break;
      case true:
        encoderValue = encoderValue - encreaseValue;
      }    
    } else {

      switch (encoderIsValue) {
      case false: 
        cursor++; 
        break;
      case true:
        encoderValue = encoderValue + encreaseValue;
      } 
    }
    
    lastMillis = millis();
  }
}

void IRAM_ATTR rotaryEncoderDT(){
  int clk = digitalRead(CLK);

  if(millis() - lastMillis > 50){
    //Sair do modo Sleep:
    isSleeping = false;

    if(clk == 0){

      switch (encoderIsValue) {
      case false: 
        cursor--; 
        break;
      case true:
        encoderValue = encoderValue - encreaseValue;
      }    
    } else {

      switch (encoderIsValue) {
      case false: 
        cursor++; 
        break;
      case true:
        encoderValue = encoderValue + encreaseValue;
      } 
    }
    
    lastMillis = millis();
  }
}


// Draws
  const unsigned char PROGMEM icon_speaker_device_20_20 [] = {0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x08, 0x01, 0x00, 0x08, 0xf1, 0x00, 0x09, 0x09, 0x00, 0x09,	0xf9, 0x00, 0x09, 0x09, 0x00, 0x28, 0xf1, 0x40, 0x48, 0x01, 0x20, 0x88, 0xf1, 0x10, 0x89, 0x09, 0x10, 0xaa, 0xf5, 0x50, 0x8a, 0x95, 0x10, 0x8a, 0x95, 0x10, 0x4a, 0xf5, 0x20, 0x29, 0x09, 0x40, 0x08, 0xf1, 0x00, 0x08, 0x01, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00};
  const unsigned char PROGMEM icon_volume_20_20 []  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x01, 0x21, 0x00, 0x02, 0x20, 0x80, 0x04, 0x22, 0x40, 0x78, 0x21, 0x40, 0x40, 0x29, 0x20, 0x40, 0x24, 0xa0, 0x40, 0x24, 0xa0, 0x40, 0x24, 0xa0, 0x40, 0x24, 0xa0, 0x40, 0x29, 0x20, 0x78, 0x21, 0x40, 0x04, 0x22, 0x40, 0x02, 0x20, 0x80, 0x01, 0x21, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const unsigned char PROGMEM icon_plane_20_20 [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x1c, 0x02, 0x40, 0x23, 0x84, 0x40, 0x30, 0x68, 0x80, 0x0c, 0x11, 0x00, 0x03, 0x02, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x42, 0x00, 0x00, 0xb2, 0x00, 0x01, 0x51, 0x00, 0x1e, 0x89, 0x00, 0x23, 0x09, 0x00, 0x19, 0x04, 0x80, 0x05, 0x04, 0x80, 0x05, 0x02, 0x80, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const unsigned char PROGMEM icon_wifi_not_connected_19_16[] = {0x21,0xf0,0x00,0x16,0x0c,0x00,0x08,0x03,0x00,0x25,0xf0,0x80,0x42,0x0c,0x40,0x89,0x02,0x20,0x10,0xa1,0x00,0x23,0x58,0x80,0x04,0x24,0x00,0x08,0x52,0x00,0x01,0xa8,0x00,0x02,0x04,0x00,0x00,0x42,0x00,0x00,0xa1,0x00,0x00,0x40,0x80,0x00,0x00,0x00};
  const unsigned char PROGMEM icon_wifi_full_19_16[] = {0x01,0xf0,0x00,0x07,0xfc,0x00,0x1e,0x0f,0x00,0x39,0xf3,0x80,0x77,0xfd,0xc0,0xef,0x1e,0xe0,0x5c,0xe7,0x40,0x3b,0xfb,0x80,0x17,0x1d,0x00,0x0e,0xee,0x00,0x05,0xf4,0x00,0x03,0xb8,0x00,0x01,0x50,0x00,0x00,0xe0,0x00,0x00,0x40,0x00,0x00,0x00,0x00};
  const unsigned char PROGMEM icon_reset_11_16[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x19,0xe0,0x21,0xc0,0x41,0xc0,0x81,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x40,0x40,0x20,0x80,0x1f,0x00,0x00,0x00,0x00,0x00};
  const unsigned char PROGMEM icon_left_arrow_3_5 [] = {0x20,0x60,0xe0,0x60,0x20};
  const unsigned char PROGMEM icon_right_arrow_3_5 [] = {0x80,0xc0,0xe0,0xc0,0x80};
  const unsigned char PROGMEM icon_down_arrow_5_3 [] = {0xf8,0x70,0x20};
  const unsigned char PROGMEM icon_up_arrow_5_3 [] = {0x20,0x70,0xf8};
  const unsigned char PROGMEM icon_settings_20_20 [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x04, 0x92, 0x00, 0x0b, 0x0d, 0x00, 0x10, 0x00, 0x80, 0x08, 0xf1, 0x00, 0x09, 0x09, 0x00, 0x32, 0x04, 0xc0, 0x22, 0x04, 0x40, 0x22, 0x04, 0x40, 0x32, 0x04, 0xc0, 0x09, 0x09, 0x00, 0x08, 0xf1, 0x00, 0x10, 0x00, 0x80, 0x0b, 0x0d, 0x00, 0x04, 0x92, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const unsigned char PROGMEM icon_sleep_mode_20_20[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x07, 0x01, 0xc0, 0x0e, 0x00, 0x40, 0x1e, 0x00, 0x80, 0x1c, 0x3d, 0xc0, 0x3c, 0x08, 0x00, 0x3c, 0x10, 0x00, 0x3c, 0x3c, 0x00, 0x3e, 0x00, 0x80, 0x3e, 0x01, 0x80, 0x1f, 0x87, 0x00, 0x0f, 0xfe, 0x00, 0x07, 0xfc, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const unsigned char PROGMEM draw_volumeBorderSeletor_122_18 []  = {0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};
  const unsigned char PROGMEM bg_screenVolumeValue []  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x7d, 0xf0, 0x00, 0x3e, 0xf8, 0x00, 0x1f, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x80, 0x00, 0x0d, 0x80, 0x00, 0x30, 0xd8, 0x00, 0x1b, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x80, 0x00, 0x3d, 0xe0, 0x00, 0x3c, 0xd8, 0x00, 0x03, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x80, 0x00, 0x60, 0x30, 0x00, 0x06, 0xd8, 0x00, 0x07, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x80, 0x00, 0x60, 0x30, 0x00, 0x06, 0xd8, 0x00, 0x0e, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x7d, 0xf0, 0x00, 0x3e, 0xf8, 0x00, 0x0c, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x02, 0x00, 0x10, 0x02, 0x00, 0x10, 0x01, 0x00, 0x04, 0x00, 0x80, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x50, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x90, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x50, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x90, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x50, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x90, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x50, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x90, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x50, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  //Inicia Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }  

  // Configura encoder
  attachInterrupt(digitalPinToInterrupt(CLK), rotaryEncoderCLK, FALLING);
  attachInterrupt(digitalPinToInterrupt(DT), rotaryEncoderDT, FALLING);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(BTN, INPUT);

  //TEMP Splash Screen
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  centerText("Splash Screen", 28);
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println("");
  Serial.println("-----------------------");
  Serial.println("--- miniFCU Started ---");
  Serial.println("-----------------------");
  Serial.println("");
  
}

void loop() {

  // while(!isConnected){
  //   screenChangeWifi();
  // }
  screenMainMenu(0);
  Serial.println("loop end...");
}
