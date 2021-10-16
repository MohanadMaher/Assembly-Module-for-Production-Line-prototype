#include <AccelStepper.h>  // Stepper library
#define dirPin 8    // motor direction pin
#define stepPin 7   // motor signal pin
#define motorInterfaceType 1
const int IRSensor = 13;    // IR sensor signal pin
int limitSwitch1 = 12;      // Slider switch
int valvePin = 2;           // Relay output pin
int count = 0;
int flag=0;
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
void homing()  //reseting the system to its initial position
  {
    while(digitalRead(limitSwitch1) == HIGH)
    {
      stepper.setSpeed(-500);
      stepper.runSpeed();
    if(digitalRead(limitSwitch1) == LOW)
    {
      stepper.setSpeed(0);
      stepper.runSpeed();
      delay(10000);
      break;
    }
  }
}
void setup() {
  stepper.setMaxSpeed(1000);             // Set the maximum speed in steps per second
  pinMode(IRSensor,INPUT_PULLUP);        // IRSensor
  pinMode(limitSwitch1,INPUT);           // limitSwitch1
  pinMode(valvePin,OUTPUT);              // valvePin
  pinMode(9,OUTPUT);                     // Assembly conveyor connection
  pinMode(10,OUTPUT);                    // lcd connection
  digitalWrite (valvePin, HIGH);         
  digitalWrite(9,LOW); 
  homing();  
}
void loop() { 
  
if (digitalRead(IRSensor) == LOW && count == 0)  // wait for the upper cover to touch in
{
   digitalWrite(9,HIGH);
   digitalWrite(10,HIGH);
   delay(150);
   digitalWrite(9,LOW);
   count++;
   stepper.setCurrentPosition(0);
while(stepper.currentPosition() != 1150) // setting specific steps for stepper motor
{
    stepper.setSpeed(500);
    stepper.runSpeed();
}
    digitalWrite (valvePin, LOW);  //turning the solenoid valve ON
    delay(1000);
    flag=1;
} 
while(digitalRead(limitSwitch1) == HIGH && flag==1)
{
      stepper.setSpeed(-500);  //run motor
      stepper.runSpeed();
if(digitalRead(limitSwitch1) == LOW )  //checking if the limit switch is pressed
{
        stepper.setSpeed(0);  //stop the motor
        stepper.runSpeed();
        flag=0;
        break;
}
}
if (digitalRead(IRSensor) == LOW && count==1) //wait for the lower part to touch in
{
    digitalWrite(9,HIGH);
    delay(150);
    digitalWrite(9,LOW);
    count ++;
if(count == 2)
{
      stepper.setCurrentPosition(0);
while(stepper.currentPosition() != 520) //assemble the parts together
{
    stepper.setSpeed(400);
    stepper.runSpeed();
}
      delay(1000);
if (stepper.currentPosition()> 500)
{
       digitalWrite (valvePin, HIGH);  //turning the solenoid valve OFF
       count=0;
       delay(1000);
} 
}
      digitalWrite(10,LOW);
}
      homing();
}
    
