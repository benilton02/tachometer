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


void leftRotation(int option){
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

  if(option == 4)
    analogWrite(velocity, 150);
 
  else
    analogWrite(velocity, 255);

  if(millis() - timeBuzzer > 100){
    digitalWrite(buzzerPin, (millis()/1000)%2);
    timeBuzzer = millis();
  }
  
}


void rightRotation(int option){
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

  if(option == 2)
    analogWrite(velocity, 150);

  else
      analogWrite(velocity, 255);
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
  analogWrite(velocity, 0);
}


void setPin(){
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(mla, OUTPUT);
  pinMode(mlb, OUTPUT);
  pinMode(opticalPin, INPUT_PULLUP);
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
    rightRotation(option);
    break;

  case 2:
    rightRotation(option);
    break;

  case 3:
    leftRotation(option);
    break;
    
  case 4:
    leftRotation(option);
    break;
  
  case 5:
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
  return currentDistance != 0 ? false : true;
}
