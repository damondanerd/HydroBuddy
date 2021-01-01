//Links to component guides
//https://cdn-learn.adafruit.com/downloads/pdf/adafruit-apds9960-breakout.pdf
//https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide/all

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

float calibrated_empty_weight;
float calibrated_empty_distance;
float calibrated_color;

float calibrated_full_weight;
float calibrated_full_distance;

float current_weight;
float current_color;

//calibrate these with drinks we can obtain
float water_color = 0;
float orange_juice_color = 1;
float gatorade_color = 2;
float cola_color = 3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("set cllor");
}


//***float values being compared with == and != will probably need to be set to be in a range***
void loop() {
  // put your main code here, to run repeatedly:

  //Check for bottle (change in weight)
  if((current_color != calibrated_color) && (current_weight != calibrated_empty_weight)){
    //===Bottle detected===
    Serial.println("Bottle detected");
    //Use color to determine drink
    if(current_color == water_color){
      Serial.println("Water detected");
    }
    else if(current_color == orange_juice_color){
      Serial.println("Orange Juice detected");
    }
    else if(current_color == gatorade_color){
      Serial.println("Gatorade detected");
    }
    else if(current_color == cola_color){
      Serial.println("Cola detected");
    }
    else{
      Serial.println("Unknown drink");
    }
    //Use weight to determine amount of liquid consumed
    
    //Use distance to determine amount of liquid consumed??
  }
}
