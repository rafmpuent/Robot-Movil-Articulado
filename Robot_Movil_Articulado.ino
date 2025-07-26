#include <ESP32Servo.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

//Right motor
int enableRightMotor=32; 
int rightMotorPin1=33;
int rightMotorPin2=25;

//Left motor
int enableLeftMotor=26;
int leftMotorPin1=27;
int leftMotorPin2=14;

//Servo
Servo servoEE, servoE2, servoE1, servoBase;

#define MAX_MOTOR_SPEED 255

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

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
  
  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));  
}

void rotationArm(int angle){
  if (angle > 0){
    
  }
  else{

  }
}

void setUpPinModes()
{
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  //Set up PWM for speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);  
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel); 

  rotateMotor(0,0);

  servoEE.attach(23, 500, 2500);
  servoEE.setPeriodHertz(50);

  servoE1.attach(22, 500, 2500);
  servoE1.setPeriodHertz(50);
  
  servoE2.attach(21, 500, 2500);
  servoE2.setPeriodHertz(50);
  
  servoBase.attach(19, 500, 2500);
  servoBase.setPeriodHertz(50);

  //Position Initial
  servoEE.writeMicroseconds(500);
  servoE1.writeMicroseconds(500);
  servoE2.writeMicroseconds(500);
  servoBase.writeMicroseconds(500);

  bool flagSelection = false;
  int rotation = 0;
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
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  if (GamePad.isDownPressed())
  {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isLeftPressed())
  {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isRightPressed())
  {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);

  if (GamePad.isSelectPressed()) //Take an object
  {
    if (!flagSelection)
    {
      servoEE.writeMicroseconds(2500);
    }
    else
    {
      servoEE.writeMicroseconds(500);
    }
    flagSelection = !flagSelection;  
  }

  if (GamePad.isTrianglePressed()) //Forward Arm
  {
    moveArm();
  }

  if (GamePad.isCirclePressed()) //Rotation Rigth
  {
    rotation += 45;
  }

  if (GamePad.isCrossPressed()) //Backward Arm
  {
    moveArm(); 
  }

  if (GamePad.isSquarePressed()) //Rotation Left
  {
    rotation -= 45; 
  }

  rotationArm(rotation);
}