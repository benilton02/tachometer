#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define pushButton 11

#define velocity 6 
#define mla 3
#define mlb 5

#define address 0x27       
#define row 2
#define column 16

#define triggerPin  13  
#define echoPin     12  
#define safetyDistance 5


LiquidCrystal_I2C lcd(address, column, row);
NewPing sonar(triggerPin, echoPin, safetyDistance);

volatile unsigned int currentDistance;
volatile unsigned long int buttonTime = 0;
volatile unsigned int statusPushButton = 0;
volatile unsigned int option = 0;


void setup() {
  Serial.begin(115200);
  setPin();
  stopMotor();
  setLcd();
}


void loop() {
  if(checkSafetyDistance()){
    checkButton();
    motorStatus();
  }
  else
    stopMotor();
}


void leftRotation(){
  digitalWrite(mla, LOW);
  digitalWrite(mlb, HIGH);
  analogWrite(velocity, 255);
}


void rightRotation(){
  digitalWrite(mla, HIGH);
  digitalWrite(mlb, LOW);
  analogWrite(velocity, 255);
}


void stopMotor(){
  digitalWrite(mla, LOW);
  digitalWrite(mlb, LOW);
  analogWrite(velocity, 255);
}


void setPin(){
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(velocity, OUTPUT);
  pinMode(mla, OUTPUT);
  pinMode(mlb, OUTPUT);
}


void motorStatus(){
  switch(option){
  
  case 0:
    stopMotor(); 
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Motor: STOP!");
    break;

  case 1:
    rightRotation();
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Rotation: RIGHT");
    break;

  case 2:
    leftRotation();
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Rotation: LEFT");
    break;

  case 3:
    option = 0;
    break;
  }
}


void setLcd(){
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
}


void checkButton(){
  statusPushButton = digitalRead(pushButton);
  if(!statusPushButton){
    if (millis() - buttonTime >= 3000){
      option++;
      buttonTime = millis();
    }
  }
}


bool checkSafetyDistance(){
  delay(50);                     
  currentDistance = sonar.ping_cm();
  Serial.println(currentDistance);
  return currentDistance != 0 ? false : true;
}