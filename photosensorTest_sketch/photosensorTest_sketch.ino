#include <Servo.h>

int lightSensorPin = A0;
int servoPin = A1;
int servoValue = 0;
int analogValue = 0;
Servo servo;

void setup() {
    servo.attach(servoPin);
}
  

void loop(){
  analogValue = analogRead(lightSensorPin);
  Serial.println(analogValue);
  //Serial.println(servoValue);
  servo.write(0);
  servo.write(180);
  delay(200);
}
