#include <Servo.h>

#define MOTOR_L_1 7
#define MOTOR_L_2 6
#define MOTOR_R_1 5
#define MOTOR_R_2 4

#define arm1_servo A2
#define arm2_servo A3
#define grip_servo A1
#define center_servo A4
#define flag_servo A0

Servo arm1;
Servo arm2;
Servo grip;
Servo center;
Servo flag;

float arm1_angle = 90;
float arm2_angle = 90;

bool is_grip = false;
float grip_angle = 120;
float ungrip_angle = 180;

float center_angle = 90;
float center_d = 3;

bool is_flag = false;
float flag_angle = 180;
float unflag_angle = 90;

void setup() {
  arm1.attach(arm1_servo);
  arm2.attach(arm2_servo);
  grip.attach(grip_servo);
  center.attach(center_servo);
  flag.attach(flag_servo);

  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);

  Serial.begin(9600);

  arm1.write(90);
  arm2.write(90);
  grip.write(ungrip_angle);
  center.write(center_angle);

}

int maxspeed = 255;
char code;
String message = "";

void loop() {
  if (Serial.available()) {
    message = Serial.readStringUntil('\n'); 
  if (message == "F") {  
    set_motor(maxspeed, maxspeed);
  } else if (message == "S") {
    set_motor(0, 0);
  } else if (message == "G") { 
    set_motor(-maxspeed, -maxspeed);
  } else if (message == "Q") { 
    set_motor(0, maxspeed);
  } else if (message == "E") { 
    set_motor(maxspeed, 0);
  } else if (message == "L") {
    set_motor(-maxspeed, maxspeed);
  } else if (message == "R") { 
    set_motor(maxspeed, -maxspeed); 
  } else if (message == "Z") { 
    set_motor(-maxspeed, maxspeed);
  } else if (message == "C") {
    set_motor(maxspeed, -maxspeed);
  } else if (message == "M") {
    if (is_grip){
      grip.write(ungrip_angle);
    }else{ 
      grip.write(grip_angle);
    }
    is_grip = !is_grip;
  } else if (message == "N") {
    if (is_flag){
      flag.write(unflag_angle);
    }else{
      flag.write(flag_angle);
    }
    is_flag = !is_flag;
  } else if (message == "X") { 
    center_angle = constrain(center_angle + center_d, 0, 180);
    center.write(center_angle);
  } else if (message == "Y") {
    center_angle = constrain(center_angle - center_d, 0, 180);
    center.write(center_angle);
  } else if (message[0] == 'J'){ 
    arm1_angle = message.substring(1).toInt();
    arm1.write(arm1_angle);
  } else if (message[0] == 'K'){
    arm2_angle = message.substring(1).toInt();
    arm2.write(arm2_angle);
  } 
  }
}


void setmotor(int speedA, int speedB) { 
  speedA = constrain(speedA, -maxspeed, maxspeed); 
  speedB = constrain(speedB, -maxspeed, maxspeed); 

  speedB = -speedB;
  speedA = -speedA;

  if (speedA > 0){ 
    digitalWrite(MOTOR_R_2, LOW); 
    analogWrite(MOTOR_R_1, speedA); 
  } else{ 
    digitalWrite(MOTOR_R_2, HIGH);
    analogWrite(MOTOR_R_1,255 + speedA);
  }
  if (speedB > 0){
    digitalWrite(MOTOR_L_1, LOW);
    analogWrite(MOTOR_L_2, speedB);
  } else{
    digitalWrite(MOTOR_L_1, HIGH);
    analogWrite(MOTOR_L_2, 255 + speedB);
  }
}

