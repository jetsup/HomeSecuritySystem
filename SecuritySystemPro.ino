#include <Servo.h>

unsigned long buzzerRunTime = 0;
unsigned long buzzerDelayOff = 100;
unsigned long buzzerDelayON = 300;
byte buzzerState = LOW;

unsigned long ultraRunTime = 0;
unsigned long ultraPreviousTime = 0;
byte ultraState = HIGH;

unsigned long previousTime = 0;

// LEDs
#define securityLight 2
#define indoorLight 3
#define ldr A0
// servos
#define doorLock1 4
#define doorLock2 5
// HC-SR04
#define sensorTrig1 6
#define sensorEcho1 7
#define sensorTrig2 8 //
#define sensorEcho2 9 //

#define buzzer 10

Servo lock1, lock2;

float duration, distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(securityLight, OUTPUT);
  pinMode(indoorLight, OUTPUT);
  pinMode(ldr, INPUT);

  lock1.attach(doorLock1);
  lock1.attach(doorLock2);
  
  pinMode(sensorTrig1, OUTPUT);
  pinMode(sensorEcho1, INPUT);
  
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buzzerRunTime = millis();
  ultraRunTime = millis();
  
  int darknessLevel = analogRead(ldr);
  Serial.println(darknessLevel);
  if(darknessLevel > 25){
    digitalWrite(securityLight, HIGH);
  } else {
    digitalWrite(securityLight, LOW);
  }
  senseDistance();
}

void senseDistance(){
  if(ultraState == HIGH){
    if((unsigned long) ultraRunTime - ultraPreviousTime >= 0.01){
      ultraState = LOW;
      digitalWrite(sensorTrig1, ultraState);
    }
  } else {
    if((unsigned long) ultraRunTime - ultraPreviousTime >= 0.002){
      ultraState = HIGH;
      digitalWrite(sensorTrig1, ultraState);
    }
  }
  digitalWrite(sensorTrig1, LOW);

  duration = pulseIn(sensorEcho1, HIGH);
  distance = (duration / 2) * 0.0343;

  Serial.print("Distance: ");
  if (distance >= 400 || distance <= 2)
  {
    Serial.println("Out of range");
  }
  else if(distance <= 150)
  {
    makeSound();
    Serial.print(distance);
    Serial.println(" cm -- Alarm");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    stopSound();
  }
}

void makeSound()
{
  if(buzzerState == LOW){
    if((unsigned long) buzzerRunTime - previousTime >= 600){
      buzzerState = HIGH;
      digitalWrite(buzzer, buzzerState);
    }else {
      if((unsigned long) buzzerRunTime - previousTime == 400){
        buzzerState = LOW;
        digitalWrite(buzzer, buzzerState);
      }
    }
  }
}

void stopSound(){
  digitalWrite(buzzer, LOW);
}
