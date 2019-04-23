int powerSwitch = 8;
int ammeter = 5;
int interlock = 13;
int garageDoor = 12;
int kitchenLight = 11;
int denLight = 10;
int outdoorLight = 9;
int ampReading;
int amps;
int numOutputs = 5;
int currentStatus[] = {0, 0, 0, 0, 0};
int newStatus[] = {0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  // SETUP bluetooth
  // SETUP LCD
  pinMode(powerSwitch, INPUT);
  pinMode(A5, INPUT); //configure the ammeter on analog pin a5
  pinMode(interlock, OUTPUT);
  pinMode(garageDoor, OUTPUT);
  pinMode(kitchenLight, OUTPUT);
  pinMode(denLight, OUTPUT);
  pinMode(outdoorLight, OUTPUT);

}

int toAmps(int ampReading){
  int newamps = 0;
  //Convert ampReading into an amp value store in newamps
  return newamps;
}

void updatePins(int[] newStatus){
  int outputPin = 9
  for(int ctr = 0; ctr < newStatusLength; ctr++){
    if(newStatus[ctr] = 1){
      digitalWrite((outputPin + ctr), HIGH);
    }
    else{
      digitalWrite((outputPin + ctr), LOW);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ampReading = analogRead(A5);
  amps = toAmps(ampReading);
  // Send pin status and current data
  // Check for an instruction in the input buffer
  if(currentStatus != newStatus){
    updatePins(newStatus);
  }
  }

}
