/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in color mode and reads the RGB and clear values.

  Designed specifically to work with the Adafruit APDS9960 breakout
  ----> http://www.adafruit.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_APDS9960.h"
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

//create the APDS9960 object
Adafruit_APDS9960 apds;

//the pin that the interrupt is attached to
#define INT_PIN 3

//calibrated when bottle is empty
float calibrated_empty_weight;
int calibrated_empty_distance;
int calibrated_color_r;
int calibrated_color_g;
int calibrated_color_b;
int calibrated_color_c;

//calibrated when bottle is full
float calibrated_full_weight;
int calibrated_full_distance;

//currently detected values
float current_weight;
int current_distance;
int current_color_r;
int current_color_g;
int current_color_b;
int current_color_c;

//calibrate these with drinks we can obtain
float water_color = 0;
float orange_juice_color = 1;
float gatorade_color = 2;
float cola_color = 3;

//storage variables
int prox_val = 0;
char line0[21];
char line1[21];
char* line1_text;

//create some variables to store the color data in
uint16_t r, g, b, c;

void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  pinMode(INT_PIN, INPUT_PULLUP);

  //Set detection LED pin
  pinMode(7, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 255);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable color sensign mode
  apds.enableColor(true);
  //enable proximity mode
  apds.enableProximity(true);

  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 1);

  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {
  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(INT_PIN)){
    prox_val = apds.readProximity();
    Serial.print("Proximity: ");
    Serial.println(prox_val);

    //clear the interrupt
    apds.clearInterrupt();
  }
  else{
    prox_val = 0;
  }
  
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }

  //LED DETECTION
  if(prox_val > 1){
    digitalWrite(7, HIGH);
  }
  else{
    digitalWrite(7, LOW);
  }

  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);
  Serial.print("red: ");
  Serial.print(r);
  
  Serial.print(" green: ");
  Serial.print(g);
  
  Serial.print(" blue: ");
  Serial.print(b);
  
  Serial.print(" clear: ");
  Serial.println(c);
  Serial.println();

  //Drink Detection Test Logic
  if(prox_val > 1){
    sprintf(line0, "Proximity: %-10d", prox_val);
    if (c > 40){
      sprintf(line1, "%-11s", "CLEAR drink");
    }
    else if(r > g && r > b){
      sprintf(line1, "%-13s", "RED drink");
    }
    else if(g > r && g > b){
      sprintf(line1, "%-11s", "GREEN drink");
    }
    else if(b > r && b > g){
      sprintf(line1, "%-12s", "BLUE drink");
    }
    else{
      sprintf(line1, "%-9s", "UNKNOWN drink");
    }
  }
  else{
    sprintf(line0, "Proximity: %-10d", prox_val);
    sprintf(line1, "%-13s", "NO drink");
  }

//  sprintf(line0, "Proximity: %-10d", prox_val);
//  sprintf(line1, "R:%dG:%dB:%dC:%d", r, g, b, c);
  updateDisplay();

//  uint16_t map_r, map_g, map_b;
//
//  map_r = map(r, 0, 500, 0, 255);
//  map_g = map(g, 0, 500, 0, 255);
//  map_b = map(b, 0, 500, 0, 255);
//
//  Serial.print("Mapped red: ");
//  Serial.println(map_r);
//  Serial.print("Mapped green: ");
//  Serial.println(map_g);
//  Serial.print("Mapped blue: ");
//  Serial.println(map_b);
  
//  lcd.setRGB(map_r, map_g, map_b);
  
  delay(500);
}

void updateDisplay() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line0);
  lcd.setCursor(0, 1);
  lcd.print(line1);
  //delay(10);
}
