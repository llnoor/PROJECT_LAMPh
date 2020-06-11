int dl = 100;
int flag = 0;
int val = 0;
int pin_a[8];
int flag_remote =0;
int buttonState = 0;
int buttonPin = 9; 

//String inString;
String inputString = ""; 
bool stringComplete = false; 

void setup() {
  Serial.begin(9600);
  Serial.write("ArduinoDUE,RelayModule2,LAMPh_0.5.0"); 
 
  for (int i=0;i<4;i++)
  {
    pin_a[i]=4+i;
  }

  for (int i=0;i<8;i++)
  {
    pinMode(pin_a[i], OUTPUT);
    digitalWrite(pin_a[i], LOW);  //HIGH = turn on !!!   //LOW  = turn off  !!!
  }

  pinMode(buttonPin, INPUT);
  
  inputString.reserve(200);
}

void loop() {

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {   
    turnA(2,1);turnA(3,1);delay(5000);
    turnA(2,0);turnA(3,0);delay(5000); 
  }
  else {
    int intR=0;

    if (stringComplete) {
      //Serial.println(inputString);
      if (inputString.indexOf("*IDN?") >= 0) Serial.println("ArduinoDUE,RelayModule2,LAMPh_0.5.0");
      intR = inputString.toInt();
      inputString = "";
      stringComplete = false;
    }
  
    switch (intR)
    {
    //case 0: break; 
    case 1: turnA(2,1);turnA(3,1);break; 
    case 2: turnA(2,0);turnA(3,0);break;
    case 3: turnA(0,1);turnA(1,1);break;
    case 4: turnA(0,0);turnA(1,0);break;
  
    case 70: flag_remote=1; check(); break; 
    case 71: flag_remote=0; turnA(0,0);turnA(1,0); turnA(2,0);turnA(3,0); break; 
    }
  } 
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void turnA(int i, int k){
  if (k == 1)
  digitalWrite(pin_a[i], HIGH);
  else
  digitalWrite(pin_a[i], LOW); 
}

void turnOff(){
  for (int i=0;i<4;i++)
  {
    digitalWrite(pin_a[i], LOW);
  }
}

void check(){
  for (int i=0;i<4;i++)
  {
    digitalWrite(pin_a[i], HIGH);
    delay(dl);
  }
  for (int i=0;i<4;i++)
  {
    digitalWrite(pin_a[i], LOW);
    delay(dl);
  }
}


  
