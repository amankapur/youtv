#include <stdlib.h>

#define BUTTON_PIN 3
#define MOTOR_PIN 13

int motor = MOTOR_PIN;
String state = "pause";
float pos = 0.0;
char *pos2;
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
  // send state and pos to server
  //  dtostrf (pos, '4', '2', pos2);
  //  Serial.println(state + ' ' + pos2 + " -");

  // incoming message from server
  //  if (Serial.available() >0){
  message = "Synch 0.21 -"; 
  handleMessage(message);
  //}

  // play/pause button press
  if (buttonPress(buttonPin)) {
    if (state == "pause") {
      state == "play";
    } 
    else {
      state == "pause";
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
  Serial.println(mesg);
  if (mesg.indexOf("l") > 0) return; //life is good, don't do anything
  if (mesg.indexOf("y") >0) {
    for (i = 0; i < mesg.length(); i++){
      msg[i] = mesg[i];
    }
    pch = strtok (msg," ");
    pch = strtok (NULL, " ");
    pos2 = pch;
    pos = atof(pos2);
    Serial.println(pos);
    Serial.println(pos2);
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
  if (digitalRead(buttonPin) == HIGH) 
  {
    return true ;
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




