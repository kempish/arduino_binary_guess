#include "LED.h"
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// settings
const int RECV_PIN = 10;              // remote pin
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD


// delay variables
int shortDelay = 750;
int longDelay = 3000;


// define remote values
const long ACCEPT_INPUT     = 3158572800;
const long CLEAR_INPUT      = 4127850240;
const long NUM_ZERO         = 3910598400;
const long NUM_ONE          = 4077715200;
const long NUM_TWO          = 3877175040;
const long NUM_THREE        = 2707357440;
const long NUM_FOUR         = 4144561920;
const long NUM_FIVE         = 3810328320;
const long NUM_SIX          = 2774204160;
const long NUM_SEVEN        = 3175284480;
const long NUM_EIGHT        = 2907897600;
const long NUM_NINE         = 3041591040;


// define LED
LED led1(2);
LED led2(4);
LED led3(6);
LED led4(8);
LED ledGreen(12);

int LEDpinNumbers[4] = {2, 4, 6, 8};
const int NUMBER_OF_LEDS = 4;


// variables
String userInput;        // for tracking user input and printing on lcd screen
int userChoice = 0;      // answer confirmed by user


// METHODS / - - - - -

// convert int into LED-binary
void calculate(int targetVal){
  int temp = targetVal;

  if(temp-8 >= 0){
    led4.turnON();
    temp = temp-8;
  }

  if(temp-4 >= 0) {
    led3.turnON();
    temp = temp-4;
  }

  if(temp-2 >= 0) {
    led2.turnON();
    temp = temp-2;
  }

  if(temp-1 >= 0) {
    led1.turnON();
    temp = temp-1;
  }

  if(temp>0){
    led1.turnOFF();
    led2.turnOFF();
    led3.turnOFF();
    led4.turnOFF();
  }

}

// clear LCD input and turn off LEDs
void clearInput(){
  lcd.clear();
  led1.turnOFF();
  led2.turnOFF();
  led3.turnOFF();
  led4.turnOFF();
}

// clear user input, clear LCD input and add new input in both rows
void twoLinesClear(String firstLine, String secondLine){
  userInput = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

// clear LCD input and add new input (without deleting user input)
void twoLines(String firstLine, String secondLine){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0,1);
  lcd.print(secondLine);   // current user input (not confirmed)
}


int takeRemoteInput(){

  if(IrReceiver.decode()){
    
    // raw int input value (eg. 3041591040)
    long intVal = IrReceiver.decodedIRData.decodedRawData;

    switch(intVal) {
      case ACCEPT_INPUT: userChoice=userInput.toInt();
        break;
      case CLEAR_INPUT: twoLinesClear("YOUR ANSWER:", "");
        break;
      case NUM_ZERO: twoLines("YOUR ANSWER:", userInput+="0");
        break;
      case NUM_ONE: twoLines("YOUR ANSWER:", userInput += "1");
        break;
      case NUM_TWO: twoLines("YOUR ANSWER:", userInput += "2");
        break;
      case NUM_THREE: twoLines("YOUR ANSWER:", userInput += "3");
        break;
      case NUM_FOUR: twoLines("YOUR ANSWER:", userInput += "4");
        break;
      case NUM_FIVE: twoLines("YOUR ANSWER:", userInput += "5");
        break;
      case NUM_SIX: twoLines("YOUR ANSWER:", userInput += "6");
        break;
      case NUM_SEVEN: twoLines("YOUR ANSWER:", userInput += "7");
        break;
      case NUM_EIGHT: twoLines("YOUR ANSWER:", userInput += "8");
        break;
      case NUM_NINE: twoLines("YOUR ANSWER:", userInput += "9");
        break;
    }

    IrReceiver.resume();    // continue to collect data from user

  }
}

// support for wrongAnswer() method
bool isLEDpin(int pinToCheck){
  for(int i=0; i<NUMBER_OF_LEDS; i++) {               
    if (LEDpinNumbers[i] == pinToCheck) return true;
  }
}


void correctAnswer() {
  clearInput();
  // show message and turn on green LED
  lcd.setCursor(0, 0);
  lcd.print("CORRECT!");
  ledGreen.turnON();
  delay(longDelay);
  ledGreen.turnOFF();
}


void wrongAnswer(int correctVal) {
  int areOn[10] = {};

  // clear screen and show message with correct answer on LCD
  lcd.clear();                                    
  lcd.setCursor(0,0);
  lcd.print("WRONG!");
  lcd.setCursor(0,1);
  lcd.print("CORRECT:");
  lcd.setCursor(9,1);
  lcd.print(correctVal);

  // check which LEDs are on 
  for(int i=0; i<=10; i++) {                      
    if(digitalRead(i) == HIGH && isLEDpin(i)){    // check which LEDs are on (isLEDpin() to check only LED pins)
      areOn[i] = i;                               // add their pin numbers to array (necessary to blink simultaneously)
    }
  }
 
  for(int i=0; i<4; i++){                         // blink 4 times to accent correct answer
    for(int v: areOn) digitalWrite(v, LOW);
    delay(shortDelay);  
    for(int v: areOn) digitalWrite(v, HIGH);
    delay(shortDelay);
  }
}
// - - - - - / METHODS


// main setup
void setup() {
  lcd.begin();
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  //Let's make it more random
  randomSeed(analogRead(A0)); 
}

// main loop
void loop() {
  userChoice = -1;

  // generate random number to guess
  int randomNumber = random(0, 15);
  
  // clear led, diods and user input before new input
  clearInput();
  userInput = "";

  // show LED-binary and ask user for int value
  calculate(randomNumber);
  lcd.setCursor(0, 0);
  lcd.print("YOUR ANSWER:");

  // wait for user's input
  while(userChoice==-1) {
    takeRemoteInput();
  } 

  // check user's answer
  if(userChoice == randomNumber) {
    correctAnswer();
  }
  else{
    wrongAnswer(randomNumber);
  }
}















