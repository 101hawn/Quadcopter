
#include <Servo.h>

Servo servo;
int i;
void setup() {
servo.attach(9);
servo.write(0); 
delay(500);
Serial.begin(9600);
i = 0;
}

void loop() {
  i = i + 1;
  if(i>50)
  {
    i = 0;
  }
  
  servo.write(56); 
}
