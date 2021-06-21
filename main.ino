#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define pushButton 16
#define address 0x27       
#define row 2
#define column 16

//#define triger 11
#define safetyDistance 5
// #define ultrasonicPin 2 // interrupção 0


#define opticalPin 3 // interrupção 1


// NewPing ultrasonic(, maxDistance)
LiquidCrystal_I2C lcd(address, column, row);

volatile int currentDistance,rpm;
volatile int statusPushButton = 0;
volatile int timeRpm = 0;
volatile int cont = 0;
volatile int myStatus = 0;
unsigned long int buttonTime = 0;


void setup(){    
    setLcd();

    pinMode(opticalPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(opticalPin), revolution, RISING);


    // pinMode(ultrasonicPin, INPUT);
    // attchInterrupt(
    //     digitalPinToInterrupt(ultrasonicPin),
    //     stopMotor,
    //     RISING
    // );

}

void loop(){
    if (stopMotor){
        checkButton();

        timeRpm = millis();
        if (millis() - timeRpm >= 500){
            showRotation(); 
            timeRpm = millis();
        }


    }
}

    
void checkButton(){
    statusPushButton = digitalRead(pushButton);
    if(statusPushButton){
      if (millis() - buttonTime >= 5000){
        myStatus++;
        buttonTime = millis();
      }
    }
}

void showRotation(){
    rpm = cont*3;
    cont = 0;
    
    lcd.print("");
    lcd.setCursor(0, 0);
    lcd.print("RPM: ");
    lcd.print(rpm);
    lcd.setCursor(0, 1);  
    
    // if (alguma_coisa)
    //     lcd.print("Rotation: Right");

    // else{
    //     lcd.print("Rotation: Left");
    // }
}

void setLcd(){
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
}

void revolution(){
    cont++;
}

bool stopMotor(){
    //determinar a distancia medida pelo senso ultrassonico
    currentDistance <= safetyDistance ? true : false;
}
    