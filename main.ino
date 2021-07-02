#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define pushButton 11
#define opticalPin 2
#define buzzerPin 8

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
                      currentDistance = 0,
                      intensity = 150,
                      option = 0,
                      rpm = 0,
                      rev = 0;

volatile unsigned long int timeToShow = millis(),
                           buttonTime = millis(),
                           timeBuzzer = millis(),
                           rpmTime = millis();


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
    calculateRpm();
  }
  else
    stopMotor();
    delay(500);
}


void leftRotation(){
  if(millis() - timeToShow > 500){
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("RPM: ");
    lcd.print(rpm);
    lcd.setCursor(0, 1);
    lcd.print("Rotation: LEFT");
  }
  digitalWrite(mla, LOW);
  digitalWrite(mlb, HIGH);
  analogWrite(velocity, intensity);

  if(millis() - timeBuzzer > 100){
    digitalWrite(buzzerPin, rev%2);
    timeBuzzer = millis();
  }
  
}


void rightRotation(){
  digitalWrite(buzzerPin, LOW);
  if(millis() - timeToShow > 500){
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("RPM: ");
    lcd.print(rpm);
    lcd.setCursor(0, 1);
    lcd.print("Rotation: RIGHT");
    timeToShow = millis();
  }

  digitalWrite(mla, HIGH);
  digitalWrite(mlb, LOW);
  analogWrite(velocity, intensity);
}


void stopMotor(){
  digitalWrite(buzzerPin, LOW);
  if(millis() - timeToShow > 500){
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("RPM: ---");
    lcd.setCursor(0, 1);
    lcd.print("Motor: STOP!");
  }
  digitalWrite(mla, LOW);
  digitalWrite(mlb, LOW);
  analogWrite(velocity, intensity);
}


void setPin(){
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(mla, OUTPUT);
  pinMode(mlb, OUTPUT);
  pinMode(opticalPin, opticalPin);
  pinMode(velocity, OUTPUT);
}


void calculateRpm(){
  attachInterrupt(digitalPinToInterrupt(opticalPin), revolution, RISING);
  if(millis() - rpmTime > 1000 ){
    detachInterrupt(digitalPinToInterrupt(opticalPin));
    rpm =rev*3;
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
    break;

  case 1:
    rightRotation();
    break;

  case 2:
    leftRotation();
    break;
    
  case 3:
    resetOption();
    break;
  }
}


void resetOption(){
  option = 0;  
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
  //Serial.println(currentDistance);
  return currentDistance != 0 ? false : true;
}