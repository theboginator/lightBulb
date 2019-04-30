#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int interlock = 2;
int relayPin = 3;  // use the built in LED on pin 13 of the Uno
int state = 0;
int flag = 0;        // make sure that you return the state only once

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("hi everyone");
  lcd.setCursor(1,1);
  lcd.print("how to do this?");
  delay(1000);
  pinMode(interlock, OUTPUT);
  pinMode(relayPin, OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Testing... Wait");
  digitalWrite(interlock, LOW);
  delay(600);
  digitalWrite(interlock, HIGH);
  digitalWrite(relayPin, LOW);
  delay(600);
  Serial.begin(9600);
  digitalWrite(relayPin, HIGH);
  digitalWrite(interlock, LOW);
  
  
}


void loop()
{
  int ampVal = analogRead(A0);
   if(Serial.available() > 0){
      state = Serial.read();
      flag=0;
    }
    // if the state is 0 the led will turn off
    if (state == '0') {
        digitalWrite(relayPin, HIGH);
        if(flag == 0){
          Serial.println("LED: off");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Relay OFF");
          lcd.setCursor(0, 1);
          lcd.print("Amps: ");
          lcd.print(ampVal);
          flag = 1;
        }
    }
    // if the state is 1 the led will turn on
    else if (state == '1') {
        digitalWrite(relayPin, LOW);
        if(flag == 0){
          Serial.println("LED: on");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Relay ON");
          lcd.setCursor(0, 1);
          lcd.print("Amps: ");
          lcd.print(ampVal);
          flag = 1;
        }
    }
}
