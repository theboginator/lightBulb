#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int powerSwitch = 12; //Digital input pin shows Power Switch status
int autoPreference = 11; //Digital input pin to show user preference for auto light control
int doorSwitch = 10; //Digital input pin to report status of garage door
boolean powerStatus; //Boolean to show whether power switch is on or off
boolean autoLight = true; //Shows the mode of the outdoor light: automatic control by photocell reading or manual control by switch

int ammeter = A0; //Analog input pin to recieve amp draw reading

int photoCell = A1; //Analog input pin to report current light reading

int interlock = 2; //Digital output pin to operate the power interlock relay
int garageDoor = 3; //Digital output pin to signal the garage door to operate
int kitchenLight = 4; //Digital output pin to activate the kitchen light
int denLight = 5; //Digital output pin to activate the den light
int outdoorLight = 6; //Digital output pin to activate the outdoor light

int ampReading; //Stores the current value of the ammeter
double amps; //The current amp draw of the system calculated by ampReading

int ambient; //The current light reading outside (used to control outdoor light in auto)
int threshold; //The level of ambient light used to control the outdoor light in auto

int numOutputs = 5; //Number of controllable outputs
int currentStatus[] = {0, 0, 0, 0, 0}; //Current output status
int newStatus[] = {0, 0, 0, 0, 0}; //Switch requests from bluetooth connection

int request = 0;
int flag = 0;

void initializePins(){
  pinMode(interlock, OUTPUT); //configure the interlock relay output pin
  pinMode(garageDoor, OUTPUT); //configure the garage door output pin
  pinMode(kitchenLight, OUTPUT); //configure the kitchen light output pin
  pinMode(denLight, OUTPUT); //configure the den light output pin
  pinMode(outdoorLight, OUTPUT); //configure the outdoor light output pin
  //Set all pins to HIGH so their relays will be OFF:
  setAllOff();
}
void testRelays(){
  digitalWrite(outdoorLight, LOW);
  Serial.println("\nTesting Outdoor light... OK");
  delay(1000);
  digitalWrite(outdoorLight, HIGH);
  delay(1000);

  digitalWrite(denLight, LOW);
  Serial.println("Testing Den light... OK");
  delay(1000);
  digitalWrite(denLight, HIGH);
  delay(1000);

  digitalWrite(kitchenLight, LOW);
  Serial.println("Testing Kitchen light... OK");
  delay(1000);
  digitalWrite(kitchenLight, HIGH);
  delay(1000);

  digitalWrite(garageDoor, LOW);
  Serial.println("Testing Garage door... OK");
  delay(1000);
  digitalWrite(garageDoor, HIGH);
  delay(1000);

  digitalWrite(interlock, LOW);
  Serial.println("Testing Interlock... OK");
  delay(1000);
  digitalWrite(interlock, HIGH);
  delay(1000);
}

void setup() {
  Serial.begin(9600); //Setup serial link for communicating with PC or Bluetooth
  // put your setup code here, to run once:
  // SETUP bluetooth
  lcd.init(); // initialize the lcd 
  lcd.backlight(); //Turn on the LCD backlight
  lcd.setCursor(0,0);
  lcd.print("SmartKontrolv0.1");
  lcd.setCursor(0,1);
  delay(1000);
  lcd.print("Hi Everyone");
  pinMode(powerSwitch, INPUT); //A SPDT switch that interrupts the power relay and signals the control it's status
  pinMode(autoPreference, INPUT); //A SPST switch that signals the user's preference for auto/manual outdoor light operation
  pinMode(doorSwitch, INPUT); //A SPST button that signals the garage door being open or closed
  //pinMode(A5, INPUT); //configure the ammeter on analog pin a5
  //pinMode(A4, INPUT); //configure the garage door status sensor on analog pin a4
  //pinMode(A3, INPUT); //configure the ambient light sensor on pin a3
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing... ");
  lcd.print("Wait");
  initializePins();
  
  lcd.setCursor(0, 1);
  lcd.print("..............");
  testRelays();
  lcd.print("OK");
  delay(2000);
  Serial.println("Enter a switch to activate");
  lcd.clear();
}




double toAmps(int ampReading){ //function that converts raw amp draw sensor data into amp value and returns as a double
  double newamps = 0;
  //Convert ampReading into an amp value store in newamps
  return newamps;
}

/*
void updatePins(int[] newStatus){ //Update the outputs with the new requested sensor data recieved over bluetooth link
  int outputPin = 9;
  for(int ctr = 0; ctr < newStatusLength; ctr++){
    if(newStatus[ctr] = 1){
      digitalWrite((outputPin + ctr), LOW);
    }
    else{
      digitalWrite((outputPin + ctr), HIGH);
    }
  }
}
*/
void readPins(){ //Get the status of each pin
  powerStatus = digitalRead(powerSwitch); //Read power switch status
  autoLight = digitalRead(autoPreference); //Read auto light switch setting
  ampReading = analogRead(ammeter); //Read amp draw information
  ambient = analogRead(photoCell); //Read photocell value
}

void setAllOff(){ //Sets each output to OFF
  digitalWrite(interlock, HIGH);
  digitalWrite(garageDoor, HIGH);
  digitalWrite(kitchenLight, HIGH);
  digitalWrite(denLight, HIGH);
  digitalWrite(outdoorLight, HIGH);
}

void setAutoOutputs(){ //Set automatic outputs (based off of sensor readings)
  if(!powerStatus){
    lcd.clear();
  }
  while(!powerStatus){ //If the power swith is off, display an appropriate message
    //LCD PRINT("Power Switch OFF");  
    lcd.setCursor(0, 0);
    lcd.print("Power Switch OFF");
    delay(100);
    digitalWrite(interlock, LOW);
    setAllOff();
    powerStatus = digitalRead(powerSwitch);
  }
  if(autoLight){ //If the light control is automatic, turn it on or off depending on what the threshold is
    if(ambient > threshold){
      digitalWrite(outdoorLight, LOW); //Turn it on if the ambient light is above threshold
    }
    else{
      digitalWrite(outdoorLight, HIGH); //Turn it off if the ambient light is below threshold
    }
  }
  
}

void updateDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: AUTO");
  lcd.setCursor(0,1);
  lcd.print("Amps: ");
  lcd.print(amps);  
}

void setRelay(int state){
  if(state == 1){
    if(digitalRead(garageDoor) > 1){
      digitalWrite(garageDoor, LOW);
      Serial.println("Garage Light ON");
    }
    else{
      digitalWrite(garageDoor, HIGH);
      Serial.println("Garage Light OFF");
    }
  }
  else if (state == 2){
    if(digitalRead(kitchenLight) > 1){
      digitalWrite(kitchenLight, LOW);
      Serial.println("Kitchen Light ON");
    }
    else{
      digitalWrite(kitchenLight, HIGH);
      Serial.println("Kitchen Light OFF");
    }   
  }
  else if (state == 3){
    if(digitalRead(denLight) > 1){
      digitalWrite(denLight, LOW);
      Serial.println("Den Light ON");
    }
    else{
      digitalWrite(denLight, HIGH);
      Serial.println("Den Light OFF");
    }
  }
  else if (state == 3 && !autoLight){
   if(digitalRead(outdoorLight) > 1){
      digitalWrite(outdoorLight, LOW);
      Serial.println("Outside Light ON");
    }
    else{
      digitalWrite(outdoorLight, HIGH);
      Serial.println("Outside Light OFF");
    }
  }
  flag = 1;
}
void loop() {
  //LCD Clear
  readPins(); //Read input data
  setAutoOutputs(); //Set automatic outputs
  // put your main code here, to run repeatedly:
  amps = toAmps(ampReading);
  // Send pin status and current data
  // Check for an instruction in the input buffer
  if(Serial.available()>0){ //If there's a new request load it in
    request = Serial.read();
    flag = 0;
  }
  if( flag == 0){
    setRelay(request);
  }
  lcd.clear();
  updateDisplay();
  
  digitalWrite(interlock, LOW);
  delay(1000);
  //LCD Clear
}
