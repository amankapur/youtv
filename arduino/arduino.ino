// Approximately 6.5 turns for 3328 encoder ticks

#include <stdlib.h>
#include <Encoder.h>

#define BUTTON_PIN 13
#define M_PIN_1 10
#define M_PIN_2 11
#define M_PIN_3 12

int mPin1 = M_PIN_1;
int mPin2 = M_PIN_2;
int mPin3 = M_PIN_3;
 
String state = "pause";
float pos = 0.0;
char *pos2;
String message = " ";
int buttonPin = BUTTON_PIN;
int vid_length = 0;
int pressed = 0;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 333;    // the debounce time; increase if the output flickers
Encoder mEncoder(2, 3);
int ePos = 0;
int ePosOld = 0;
long start;
long now;
int firstPlay = 1;
float vidLen = 10000;
int ticks = 3328;
int rightPos = 0;
int drive = 0;

void setup()
{
  pinMode(buttonPin, OUTPUT);
  pinMode(mPin1,OUTPUT);     //initialize analog read pin
  pinMode(mPin2,OUTPUT);     //initialize analog read pin
  Serial.begin(9600);          //Open serial communication

  // spin till we get video length
  while(vid_length == 0){
    message = chkSer('-');
    //Serial.println("message is " + message);
    handleMessage(message);
    //Serial.print("length: ");
    //Serial.println(vid_length);
  }
  
}
 
void loop()
{
  Serial.print(" ");
  Serial.print(state + ' ' );
  Serial.print(pos);
  Serial.println(" -");
  
 
  // incoming message from server
  if (Serial.available()){
      message = chkSer('-');
      handleMessage(message);
      
    }
  
  // play/pause button press
  if (buttonPress(buttonPin)) {
    if (state == "pause") {
      if (firstPlay == 1){
         start = millis();
         firstPlay = 0;
      }
      state = "play";

      // Serial.println(state);
    } 
    else {
      state = "pause";
      // Serial.println(state);
   
    }
  }
 
  // slider position change
  if(userMovedSlider()){
    int new_pos = getPos();
  }
  pos = getPos();
  motorControl();
 
}
 
// get position from mesg in string and float
// this is JANKETY but it works for now
void handleMessage(String mesg) {
  int i;
  char *pch;
  char msg[15];
//  Serial.println(mesg);
  if (mesg.indexOf('u') > 0) return; //life is good, don't do anything
  if (mesg.indexOf('s') >0) {
    for (i = 0; i < mesg.length(); i++){
      msg[i] = mesg[i];
    }
    pch = strtok (msg," ");
    pch = strtok (NULL, " ");
    pos2 = pch;
    pos = atof(pos2);
//    Serial.println(pos);
//    Serial.println(pos2);
  }
 
  if(mesg.indexOf('g') > 0){
    for (i = 0; i < mesg.length(); i++){
       msg[i] = mesg[i];
    }
     pch = strtok (msg," ");
     pch = strtok (NULL, " ");
     //Serial.print("pch is ");
     vid_length = atoi(pch);
    
  }
}
// Gets position from slider
// VERY IMPORTANT FUNCTION!!!

int getPos(){
  ePosOld = ePos;
  ePos = mEncoder.read();
  //Serial.println(ePos);
  if (ePos > ticks){
    state = "reset";
  }
  else if (ePos < 0){
    state = "play";
    while(ePos < 0){
      motorControl();
    }
    state = "pause";
  }
  return ePos;
}
 
// checks if slider has been moved by user
boolean userMovedSlider(){
 
  return false;
}

void motorControl(){
  now = millis();
  ePos = mEncoder.read();
  rightPos = floor((((now - start)/vidLen)*ticks));
  if (state == "play"){
    if (ePos < rightPos){
//      Serial.println(ePos);
//      Serial.println(rightPos);
      drive = 1;
    }
    else {
      drive = 0;
    }
    if (drive == 1){
      digitalWrite(mPin1,LOW);
      digitalWrite(mPin2,HIGH);
      digitalWrite(mPin3,HIGH);
    }
    else{
      digitalWrite(mPin1,LOW);
      digitalWrite(mPin2,LOW);
      digitalWrite(mPin3,LOW);
    }
  }
  if (state == "reset"){
    digitalWrite(mPin1,HIGH);
    digitalWrite(mPin2,LOW);
    digitalWrite(mPin3,HIGH);
  }
  if (state == "pause"){
    digitalWrite(mPin1,LOW);
    digitalWrite(mPin2,LOW);
    digitalWrite(mPin3,LOW);
  }
}

// was button pressed?
boolean buttonPress(int buttonPin){

  if ((millis()-lastDebounceTime) > debounceDelay){
    if (digitalRead(buttonPin) == HIGH) 
    {
      lastDebounceTime = millis();
      return true ;
    }
      return false;    
      }
    return false;
}
 
// waits for all incoming bytes till char c
// return string of data recevied22
String chkSer(char c){
  String content = "";
  char character;
  while(content.indexOf(c) <0){
    // Serial.println('content');
    if(Serial.available() >0){
      character = Serial.read();
      content.concat(character);
     
    }
  }
  //Serial.print("content is : ");
  //Serial.println(content);
 
  return content;
}
