#include <stdlib.h>

#define BUTTON_PIN 3

int motor = 13;
String state = "pause";
float pos = 0.0;
char pos2[4];
String message;
int buttonPin = BUTTON_PIN;

void setup()
{
  pinMode(motor,OUTPUT);     //initialize analog read pin
  Serial.begin(9600);          //Open serial communication
  Serial.println("pause 0.12 -");
}
 
void loop()
{
  dtostrf (pos, '4', '2', pos2);
  Serial.println(state + ' ' + pos2 + " -");
  
  if (Serial.available() >0){
    message = chkSer('-'); 
  }
  
  if (buttonPress(buttonPin)) {
   if (state == "pause") {
      state == "play";
   } 
   else {state == "pause";}
  }
  
  if(userMovedSlider()){
   int new_pos = getPos(motor);
   }
  
}

// Gets position from slider
int getPos(int motor){
  return 0;
}

// checks if slider has been moved by user
boolean userMovedSlider(){
  
 return false; 
}



boolean buttonPress(int buttonPin){
 if (digitalRead(buttonPin) == HIGH) 
   {return true ;}
 return false;
  
}

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



