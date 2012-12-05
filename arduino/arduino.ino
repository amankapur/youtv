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
String message;
int buttonPin = BUTTON_PIN;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 333;    // the debounce time; increase if the output flickers
Encoder mEncoder(2, 3);
int ePos = 0;
int ePosOld = 0;
long start;
long now;
int firstPlay = 1;
float vidLen = 0.0;
int ticks = 3328;
int rightPos = 0;
int drive = 0;
int cPins[6] = {A0,A1,A2,A3,A4,A5};
int gnds[6] = {4,5,6,7,8,9};
int gBool[6][6] = {{1,1,1,1,1,1},{1,1,1,1,1,1},{0,1,1,1,1,0},{0,0,1,1,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int i;
int j;
int k;
long count = 0;

void setup()
{
  pinMode(buttonPin, OUTPUT);
  pinMode(mPin1,OUTPUT);     //initialize analog read pin
  pinMode(mPin2,OUTPUT);     //initialize analog read pin
  Serial.begin(9600);          //Open serial communication
  for (i = 0; i <6; i++){
    pinMode(cPins[i],OUTPUT);
  }
  for (i = 0; i <6; i++){
    pinMode(gnds[i],OUTPUT);
  }
  
  
  while(vidLen == 0){
     //Serial.println("WtTF");
     message = chkSer('-');
     handleMessage(message);
   }
  
}
 
void loop(){
  pos = getPos()/3328.0;
  Serial.print("POS IS : ");
  Serial.println(pos);
  motorControl();
  
  Serial.print(" ");
  Serial.print(state + ' ' );
  Serial.print(pos);
  Serial.println(" -");
 
  // incoming message from server
  if (Serial.available()) {
      message = chkSer('-');
      //Serial.println("message is : " + message);
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
      //Serial.println(state);
    } 
    else {
      state = "pause";
      //Serial.println(state);
    }
  }

  // slider position change
  if(userMovedSlider()){
    //int new_pos = getPos();
  }
  
  if (state == "play"){
    playMatrix();  
  }
  if (state == "pause"){
    pauseMatrix();  
  }
  if (state == "reset"){
    pauseMatrix();
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

  }
  if(mesg.indexOf('g') > 0){
    for (i = 0; i < mesg.length(); i++){
      msg[i] = mesg[i];
    }
     pch = strtok (msg," ");
     pch = strtok (NULL, " ");
     vidLen = atoi(pch) * 1000;
    
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
// return string of data recevied
String chkSer(char c){
  String content = "";
  char character;
  while(content.indexOf(c) <0){
    // Serial.println('content');
    if(Serial.available() >0){
      character = Serial.read();
      content.concat(character);
      //Serial.println(content);

    }
  }
  return content;
}

void pauseMatrix(){
  if (count % 2 == 0){
    for (i = 0; i < 6; i++){
      digitalWrite(gnds[i],LOW);
    }
    digitalWrite(A0,HIGH);
    digitalWrite(A1,HIGH);
    digitalWrite(A4,HIGH);
    digitalWrite(A5,HIGH);
  }
  else {
    clearPins();
  }
  count++;
}

void playMatrix(){
  for (i = 0; i < 6; i++){
    for (j = 0; j < 6; j++){
      for (k = 0; k < 6; k++){
        digitalWrite(cPins[k],LOW);
      }
      digitalWrite(cPins[i],HIGH);
      if (gBool[i][j] == 1){
        digitalWrite(gnds[j],LOW);
      }
      else {
        digitalWrite(gnds[j],HIGH);
      }
    }
  }
}

void clearPins(){
  for (i = 0; i <6; i++){
    digitalWrite(cPins[i],LOW);
    digitalWrite(gnds[i],HIGH);
  }
}
