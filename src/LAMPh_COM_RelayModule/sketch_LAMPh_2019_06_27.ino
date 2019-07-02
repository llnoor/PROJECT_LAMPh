int dl = 100;
int flag = 0;
int val = 0;
int pin_a[8];
int pin_b[8];

//String inString;
String inputString = ""; 
bool stringComplete = false; 

void setup() {
  Serial.begin(9600);
  Serial.write("ArduinoDUE,RelayModule,LAMPh_0.5.0"); 
 
  for (int i=0;i<8;i++)
  {
    pin_a[i]=53-i*2;
    pin_b[i]=37-i*2;
  }

  for (int i=0;i<8;i++)
  {
    pinMode(pin_a[i], OUTPUT);
    pinMode(pin_b[i], OUTPUT);
    digitalWrite(pin_a[i], HIGH);  //HIGH = turn off !!!
    digitalWrite(pin_b[i], HIGH);  //LOW  = turn on  !!!
  }
  inputString.reserve(200);
}

void loop() {
  
  int intR=0;

  if (stringComplete) {
    //Serial.println(inputString);
    if (inputString.indexOf("*IDN?") >= 0) Serial.println("ArduinoDUE,RelayModule,LAMPh_0.5.0");
    intR = inputString.toInt();
    inputString = "";
    stringComplete = false;
  }

  switch (intR)
  {
  case 0: break; 
  case 1: turnA(3,1);turnA(4,1);turnA(6,0);turnA(7,0);turnB(3,1);turnB(4,1);turnB(6,0);turnB(7,0); break; 
  case 2: turnA(3,0);turnA(4,1);turnA(6,1);turnA(7,0);turnB(3,0);turnB(4,1);turnB(6,1);turnB(7,0); break;
  case 3: turnA(4,0);turnA(5,1);turnA(7,1);turnA(8,0);turnB(4,0);turnB(5,1);turnB(7,1);turnB(8,0); break;
  case 4: turnA(4,0);turnA(5,0);turnA(7,1);turnA(8,1);turnB(4,0);turnB(5,0);turnB(7,1);turnB(8,1); break;

  case 5: turnA(3,1);turnA(4,1);turnA(6,0);turnA(7,0);turnB(4,0);turnB(5,1);turnB(7,1);turnB(8,0); break;
  case 6: turnA(3,0);turnA(4,1);turnA(6,1);turnA(7,0);turnB(4,0);turnB(5,0);turnB(7,1);turnB(8,1); break;
  case 7: turnA(4,0);turnA(5,1);turnA(7,1);turnA(8,0);turnB(3,1);turnB(4,1);turnB(6,0);turnB(7,0); break;
  case 8: turnA(4,0);turnA(5,0);turnA(7,1);turnA(8,1);turnB(3,0);turnB(4,1);turnB(6,1);turnB(7,0); break;
  
  case 9: turnA(1,0);turnA(2,0);turnB(1,0);turnB(2,0); break;  
  case 10: turnA(1,1);turnA(2,1);turnB(1,1);turnB(2,1); break; 
  
  case 11: turnA(4,0);turnA(5,1);turnA(6,0);turnA(7,0);turnB(3,1);turnB(4,1);turnB(7,1);turnB(8,0); break;
  case 12: turnA(3,0);turnA(4,1);turnA(7,1);turnA(8,1);turnB(4,0);turnB(5,0);turnB(6,1);turnB(7,0); break;
  case 13: turnA(4,0);turnA(5,0);turnA(6,0);turnA(7,0);turnB(3,1);turnB(4,1);turnB(7,1);turnB(8,1); break;
  case 14: turnA(3,0);turnA(4,1);turnA(7,1);turnA(8,0);turnB(4,0);turnB(5,1);turnB(6,1);turnB(7,0); break;
  case 15: turnA(4,0);turnA(5,1);turnA(6,0);turnA(7,0);turnB(3,1);turnB(4,1);turnB(7,1);turnB(8,0); break;
  case 16: turnA(3,1);turnA(4,1);turnA(7,1);turnA(8,0);turnB(4,0);turnB(5,1);turnB(6,0);turnB(7,0); break;
  case 17: turnA(4,0);turnA(5,1);turnA(6,1);turnA(7,0);turnB(3,0);turnB(4,1);turnB(7,1);turnB(8,0); break;
  case 18: turnA(3,1);turnA(4,1);turnA(7,1);turnA(8,1);turnB(4,0);turnB(5,0);turnB(6,0);turnB(7,0); break;

  case 19: break; 
  case 20: break; 

  case 21: turnA(4,0);turnA(5,0);turnA(6,0);turnA(7,0);turnB(4,0);turnB(5,0);turnB(6,0);turnB(7,0); break;
  case 22: turnA(3,0);turnA(4,1);turnA(7,1);turnA(8,0);turnB(3,0);turnB(4,1);turnB(7,1);turnB(8,0); break;
  case 23: turnA(4,0);turnA(5,1);turnA(6,1);turnA(7,0);turnB(4,0);turnB(5,1);turnB(6,1);turnB(7,0); break;
  case 24: turnA(3,1);turnA(4,1);turnA(7,1);turnA(8,1);turnB(3,1);turnB(4,1);turnB(7,1);turnB(8,1); break;
  
  case 25: turnOff(); break; 
  case 26: turnOff(); break; 
  case 27: turnOff(); break; 
  case 28: turnOff(); break; 
  case 29: turnOff(); break;
  case 30: turnOff(); break;

  case 70: check(); break; 
  
  default: 
  if ((intR>30)and(intR<=68)){
    if ((intR>30)and(intR<=38)) turnA(intR-30,0);
    if ((intR>40)and(intR<=48)) turnA(intR-40,1);
    if ((intR>50)and(intR<=58)) turnB(intR-50,0);
    if ((intR>60)and(intR<=68)) turnB(intR-60,1);
    }
  break;
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
  i--;
  if (k == 1)
  digitalWrite(pin_a[i], LOW);
  else
  digitalWrite(pin_a[i], HIGH); 
}

void turnB(int i, int k){
  i--;
  if (k == 1)
  digitalWrite(pin_b[i], LOW);
  else
  digitalWrite(pin_b[i], HIGH);
}


void turnOff(){
  for (int i=2;i<8;i++)
  {
    digitalWrite(pin_a[i], HIGH);
    digitalWrite(pin_b[i], HIGH);
  }
}

void check(){
  for (int i=0;i<8;i++)
  {
    digitalWrite(pin_a[i], LOW);
    delay(dl);
  }
  for (int i=0;i<8;i++)
  {
    digitalWrite(pin_b[i], LOW);
    delay(dl);
  }
  for (int i=0;i<8;i++)
  {
    digitalWrite(pin_a[i], HIGH);
    delay(dl);
  }
  for (int i=0;i<8;i++)
  {
    digitalWrite(pin_b[i], HIGH);
    delay(dl);
  }
}


  
