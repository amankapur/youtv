int motor = 13;
 
void setup()
{
  pinMode(motor,OUTPUT);     //initialize analog read pin
  Serial.begin(9600);          //Open serial communication
  Serial.println("pause 0.12 -");
}
 
void loop()
{
  
  Serial.println(chkSer('-'));
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
