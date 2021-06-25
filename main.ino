#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define pushButton 11
#define opticalPin 2

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

volatile unsigned int statusPushButton = 0,
                      currentDistance,
                      buttonTime = 0,
                      rpmTime = 0,
                      option = 0,
                      rpm = 0,
                      timeToShow = millis(),
                      rev = 0;


void setup() {
  Serial.begin(115200);
  setPin();
  stopMotor();
  setLcd();
}


void loop() {
  //if(checkSafetyDistance()){
   if(true){
    checkButton();
    motorStatus();
    calculateRpm();
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
  analogWrite(velocity, 180);
}


void stopMotor(){
  digitalWrite(mla, LOW);
  digitalWrite(mlb, LOW);
  analogWrite(velocity, 0);
}


void setPin(){
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(mla, OUTPUT);
  pinMode(mlb, OUTPUT);
  pinMode(opticalPin, opticalPin);
  pinMode(velocity, OUTPUT);
  rpmTime = millis();
}

void calculateRpm(){
  attachInterrupt(digitalPinToInterrupt(opticalPin), revolution, RISING);
  if(millis() - rpmTime > 1000 ){
    detachInterrupt(digitalPinToInterrupt(opticalPin));
    rpm =rev*3;
    //Serial.print("RPM ---> ");
    //Serial.println(rpm);
    rev = 0;
    rpmTime = millis();
    attachInterrupt(digitalPinToInterrupt(opticalPin), revolution, RISING);
  }
}

void revolution(){
  rev++;  
}

void motorStatus(){
  switch(option){
  
  case 0:
    stopMotor(); 
    lcd.print("                ");
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("RPM: 0");
    lcd.setCursor(0, 1);
    lcd.print("Motor: STOP!");
    break;

  case 1:
    rightRotation();
    
    if(millis() - timeToShow > 100){
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("RPM: ");
      lcd.print(rpm);
      lcd.setCursor(0, 1);
      lcd.print("Rotation: RIGHT");
      timeToShow = millis();
    }
    break;

  case 2:
    leftRotation();
    lcd.print("                ");
    delay(50);
    lcd.setCursor(0, 0);
    lcd.print("RPM: ");
    lcd.print(rpm);
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