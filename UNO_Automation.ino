/*
  Arduino Starter Kit Project:
  Building Automation

  Parts required:
  - 1x TMP36 temperature sensor
  - 1x red LED
  - 1x green LED
  - 1x blue LED
  - 4x 220 ohm resistors
  - 220 ohm resistor
  - 1x 10 kilohm resistor
  - 1x 10 kilohm potentiometer
  - 1x 16x2 LCD screen
  - 1x 9V battery with pigtail
  - 1x pushbotton switch
  - 1x IRF520 MOSFET
  - 1x 1N4007 diode
  - 1x 9V DC motor
  - jumperwires

  Credits to Scott Fitzgerald for sample code in Project 3, 9, & 11
  
  Revision 7 AUG 2023

 The source code is under MIT license.
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 6, 10, 8, 9, 7);

// variable to hold the value of the switchPin
//int switchState1 = 0;

// variable to hold previous value of the switchpin
//int prevSwitchState = 0;

// a variable to choose which reply from the crystal ball
//int reply;

// named constant for the pin the sensor is connected to
const int sensorPin = A0;
// room temperature in Celsius
const float baselineTemp = 28.0;

// day-night illumination indicator
const int dayNightLightPin = 12;

// named constants for the switch and motor pins
const int switchPin = 4;// the number of the switch pin
const int motorPin =  11; // the number of the motor pin
int switchState = 0;  // variable for reading the switch's status

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // initialize digital pin dayNightLightPin as an output.
  pinMode(dayNightLightPin, OUTPUT);
  
  // initialize the motor pin as an output:
  pinMode(motorPin, OUTPUT);      
  // initialize the switch pin as an input:
  pinMode(switchPin, INPUT);
 
  // set up the number of columns and rows on the LCD 
  lcd.begin(16, 2); 
  
  // Print a message to the LCD.
  lcd.print("Welcome to the");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  // print to the second line
  lcd.print("FPGA BMS demo!");
  delay(2000);
  // clean up the screen before printing a new reply
  //lcd.clear();
  //lcd.print("Building Automation demo!");
  
  // open a serial connection to display values
  Serial.begin(9600);
  

  
  // set the LED pins as outputs
  // the for() loop saves some extra coding
  for (int pinNumber = 2; pinNumber < 4; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  
  // system status indicator
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(500);                       // wait for .5 second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);                       // wait for a second
 

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("building status");  
  
  // read the value on AnalogIn pin 0 and store it in a variable
  int sensorVal = analogRead(sensorPin);

  // send the 10-bit sensor value out the serial port
  Serial.print("sensor Value: ");
  Serial.print(sensorVal);

  // convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  // Send the voltage level out the Serial port
  Serial.print(", Volts: ");
  Serial.print(voltage);

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((voltage - 500 mV) times 100)
  Serial.print(", degrees C: ");
  float temperature = (voltage - .5) * 100;
  Serial.println(temperature);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature is:");
  lcd.setCursor(0, 1);
  lcd.println(temperature);// TMP sensor might need calibration, estimating offset 
  lcd.print("C");
  
  //delay(10000);
  
  // if the current temperature is lower than the baseline turn on the green LED, pin2 hi
  if (temperature <= baselineTemp) {
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
    switchState = LOW;
  
  } // if the temperature rises above the baseline, turn on the red LED, pin3 hi
  else if (temperature > baselineTemp) {
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    switchState = HIGH;
  } 
 
    // read the state of the switch value:
  //switchState = digitalRead(switchPin);
  
  
  // check if the switch is pressed.
  if (switchState == HIGH) {     
    // turn motor on:    
    digitalWrite(motorPin, HIGH);  
  }//end if 
  else {
    // turn motor off:
    digitalWrite(motorPin, LOW);  
  }//end else 
  
  // day-night illumination indicator
  digitalWrite(dayNightLightPin, HIGH); // turn the LED on at night
  delay(10000);                         // wait for 60 second
  digitalWrite(dayNightLightPin, LOW);  // turn the LED off during the day
  delay(10000);
  
}//loop()
