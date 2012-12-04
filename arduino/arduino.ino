#include <stdlib.h>
 
#define BUTTON_PIN 12
#define MOTOR_PIN 13
 
int motor = MOTOR_PIN;
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
long debounceDelay = 50;    // the debounce time; increase if the output flickers
 
void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(motor,OUTPUT);     //initialize analog read pin
  Serial.begin(9600);          //Open serial communication
  //Serial.println("pause 0.12 -");
  
  // spin till we get video length
  while(vid_length == 0){
    message = chkSer('-');
    Serial.println("message is " + message);
    handleMessage(message);
    Serial.print("length: ");
    Serial.println(vid_length);
  }
  
  Serial.println("exit");
}
 
void loop()
{
  //Serial.println("WTF");
  // send state and pos to server
  //  dtostrf (pos, '4', '2', pos2);  
  Serial.print(state + ' ' );
  Serial.print(pos2 );
  Serial.println(" -");
 
  // incoming message from server
    if (Serial.available() >0){
      message = chkSer('-');
      handleMessage(message);
  }
 
  // play/pause button press
  if (buttonPress(buttonPin)) {
    if (state == "pause") {
      state = "play";
      //Serial.println("state changed to : " + state);
    }
    else {
      state = "pause";
      //Serial.println("state changed to : " + state);
    }
  }
 
  // slider position change
  if(userMovedSlider()){
    int new_pos = getPos(motor);
  }
 
}
 
// get position from mesg in string and float
// this is JANKETY but it works for now
void handleMessage(String mesg) {
  int i;
  char *pch;
  char msg[15];
//  Serial.println(mesg);
  if (mesg.indexOf("l") > 0) return; //life is good, don't do anything
  if (mesg.indexOf("y") >0) {
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
     Serial.print("pch is ");
     vid_length = atoi(pch);
    
  }
}
// Gets position from slider
// VERY IMPORTANT FUNCTION!!!
 
int getPos(int motor){
  return 0;
}
 
// checks if slider has been moved by user
boolean userMovedSlider(){
 
  return false;
}
 
 
// was button pressed?
boolean buttonPress(int buttonPin){
  pressed = 0;
  if (digitalRead(buttonPin) == HIGH)
  {
    return true ;
  }
  return false;
//  int reading = digitalRead(buttonPin);
////  Serial.println(reading);
//
//  if (reading != lastButtonState) {
//    lastDebounceTime = millis();
//  }
// 
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    buttonState = reading;
//    pressed = 1;
//  }
//  lastButtonState = reading;
//  if (pressed == 1){
//    return true;
//  }
//  else{
//    return false; 
//  }
}
 
// waits for all incoming bytes till char c
// return string of data recevied
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
  Serial.print("content is : ");
  Serial.println(content);
 
  return content;
}
