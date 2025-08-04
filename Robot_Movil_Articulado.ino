#include <ESP32Servo.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

//Right motor
int rightMotorPin1=33;
int rightMotorPin2=25;

//Left motor
int leftMotorPin1=27;
int leftMotorPin2=14;

//Servo
Servo servoEE;
Servo servoE2;
Servo servoE1;
Servo servoBase;

// Pinout Servo
int servoEEPin = 15;
int servoE2Pin = 16;
int servoE1Pin = 14;
int servoBasePin = 14;

// Published values for SG90 servos
int minUs = 500;
int maxUs = 2500;

bool flagSelection = false;
int rotation = 90;
int positionE = 90;
int reader = 0;

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
}

void moveArm(int positions){
  int oldValue, newValue;
  oldValue = servoE1.read();
  reader = servoE2.read();

  servoE1.write(positions);
  newValue = servoE1.read();

  if (oldValue < newValue){
    servoE2.write(reader + 45/2);
  }
  else{
    servoE2.write(reader - 45/2);
  }
}

void rotationArm(int angle){
  servoBase.write(angle);
}

void setUpPinModes()
{
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  rotateMotor(0,0);

  servoEE.setPeriodHertz(50);
  servoE2.setPeriodHertz(50);
  servoE1.setPeriodHertz(50);
  servoBase.setPeriodHertz(50);

  servoEE.attach(servoEEPin, minUs, maxUs);
  servoE2.attach(servoE2Pin, minUs, maxUs);
  servoE1.attach(servoE1Pin, minUs, maxUs);
  servoBase.attach(servoBasePin, minUs, maxUs);

  //Position Initial
  servoEE.write(0);
  servoE2.write(0);
  servoE1.write(90);
  servoBase.write(90);
}

void setup()
{
  setUpPinModes();
  Dabble.begin("MyBluetoothCar"); 
}

void loop()
{
  int rightMotorSpeed=0;
  int leftMotorSpeed=0;

  Dabble.processInput();

  if (GamePad.isUpPressed())
  {
    rightMotorSpeed = -1;
    leftMotorSpeed = 1;
  }

  if (GamePad.isDownPressed())
  {
    rightMotorSpeed = 1;
    leftMotorSpeed = -1;
  }

  if (GamePad.isLeftPressed())
  {
    rightMotorSpeed = 1;
    leftMotorSpeed = 1;
  }

  if (GamePad.isRightPressed())
  {
    rightMotorSpeed = -1;
    leftMotorSpeed = -1;
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);

  if (GamePad.isSelectPressed()) //Take an object
  {
    if (!flagSelection)
    {
      servoEE.write(135);
    }
    else
    {
      servoEE.write(0);
    }
    flagSelection = !flagSelection;  
  }

  reader = servoE2.read();

  if (GamePad.isTrianglePressed()) //Forward Arm
  {
    if (positionE>0){
      positionE -= 45;
    }
    else{
      if (reader>0){
        servoE2.write(reader - 45);
      }
    }
  }

  if (GamePad.isCrossPressed()) //Backward Arm
  {
    if (positionE<180){
      positionE += 45;
    }
    else{
      if (reader<180){
        servoE2.write(reader + 45);
      }
    }
  }

  if (GamePad.isCirclePressed()) //Rotation Rigth
  {
    if (rotation<180){
      rotation += 10;
    }
  }

  if (GamePad.isSquarePressed()) //Rotation Left
  {
    if (rotation>0){
      rotation -= 10;
    } 
  }

  moveArm(positionE);
  rotationArm(rotation);
}
