String prefix = "#12";
const int pinaInput1 = 1; // potentiometer 1 on remote
const int pinaInput2 = 2; // potentiometer 2 on remote
const int pinaInput_Invert = 2; //specify potentiometer pin to invert
const int pinaSelect = 0; // base station mode select 1-4 rotary switch with different resistor values
const int pindUnlock = 10; // SPST momentary ON pushbutton on remote to unlock rotary level knobs
const int pindLED = 13; // "busy" indicator on base station

const unsigned long ap_delay = 150; //ms to wait after each serial command to AP800
const int Mode1 = 182;
const int Mode2 = 324;
const int Mode3 = 458;
const int Mode4 = 256;
const int ModePrecision = 10;

const char Mode1_InputL1 = '5';
const char Mode1_InputR1 = '6';
const char Mode1_InputL2 = '7';
const char Mode1_InputR2 = '8';

const char Mode2_InputL1 = '1';
const char Mode2_InputR1 = '2';
const char Mode2_InputL2 = '3';
const char Mode2_InputR2 = '4';

const char Mode3_InputL1 = '1';
const char Mode3_InputR1 = '2';
const char Mode3_InputL2 = '3';
const char Mode3_InputR2 = '4';

const char Mode4_InputL1 = '5';
const char Mode4_InputR1 = '6';
const char Mode4_InputL2 = '7';
const char Mode4_InputR2 = '8';

int mode;

void setup()
{
  pinMode(pindUnlock, INPUT_PULLUP);
  pinMode(pindLED, OUTPUT);
  digitalWrite(pindLED, LOW);
  
  Serial.begin(9600);
  Serial1.begin(9600); //Leonardo uses Serial1 to communicate with RX and TX header pins
}

void loop()
{
  if(digitalRead(pindUnlock) == LOW)
  {
    digitalWrite(pindLED, HIGH);
    mode = 0;
    //Serial.println(analogRead(pinaSelect));
    
    if(StableRead(pinaSelect, Mode1-ModePrecision, Mode1+ModePrecision, 10, 10) == true){mode = 1;}
    if(StableRead(pinaSelect, Mode2-ModePrecision, Mode2+ModePrecision, 10, 10) == true){mode = 2;}
    if(StableRead(pinaSelect, Mode3-ModePrecision, Mode3+ModePrecision, 10, 10) == true){mode = 3;}
    if(StableRead(pinaSelect, Mode4-ModePrecision, Mode4+ModePrecision, 10, 10) == true){mode = 4;}
 
    while(digitalRead(pindUnlock) == LOW)
    {
    ReadLevels();
    //delay(100);
    }
  }
  else
  {
    digitalWrite(pindLED, LOW); 
  }
}

void ReadLevels()
{
 if(mode == 1)
 {
    SetLevel(pinaInput1, Mode1_InputL1, Mode1_InputR1);
    SetLevel(pinaInput2, Mode1_InputL2, Mode1_InputR2);  
 }
 else if(mode == 2)
 {
    SetLevel(pinaInput1, Mode2_InputL1, Mode2_InputR1);
    SetLevel(pinaInput2, Mode2_InputL2, Mode2_InputR2);    
 }
 else if(mode == 3)
 {
    SetLevel(pinaInput1, Mode3_InputL1, Mode3_InputR1);
    SetLevel(pinaInput2, Mode3_InputL2, Mode3_InputR2);    
 }
 else if(mode == 4)
 {
    SetLevel(pinaInput1, Mode4_InputL1, Mode4_InputR1);
    SetLevel(pinaInput2, Mode4_InputL2, Mode4_InputR2);    
 }
}

void SetLevel(int pin, char Left, char Right)
{
  int db;
  
  if(pin == pinaInput_Invert)
  {
    db = map(analogRead(pin),1023, 0, -20, 20);    
  }
  else
  {
    db = map(analogRead(pin),0, 1023, -20, 20);
  }
  
  if(db == -21)
  {
    //lowest setting = mute
    Serial1.println(prefix + " GAIN " + Left + " I -20 A");
    delay(ap_delay);
    Serial1.println(prefix + " MUTE " + Left + " I 1");
    delay(ap_delay);
    Serial1.println(prefix + " GAIN " + Right + " I -20 A");
    delay(ap_delay);
    Serial1.println(prefix + " MUTE " + Right + " I 1");
    delay(ap_delay);    
  }
  else
  {
    //set non-mute level
    //Serial1.println(prefix + " MUTE " + Left + " I 0");
    //delay(ap_delay);    
    Serial1.print(prefix + " GAIN " + Left + " I ");
    Serial1.print(db);
    Serial1.println(" A");
    delay(ap_delay);    
    //Serial1.println(prefix + " MUTE " + Right + " I 0");
    //delay(ap_delay);    
    Serial1.print(prefix + " GAIN " + Right + " I ");
    Serial1.print(db);
    Serial1.println(" A");
    delay(ap_delay);    
  }
}

boolean StableRead(int aPin, int iFrom, int iTo, int nTimes, int msDelay)
{
  //ensures reading is consistent nTimes with msDelay retest delay

  int pa = 0; 
  if(analogRead(aPin) <= iTo && analogRead(aPin) >= iFrom){pa++;}
  delay(msDelay);
  
  for (int i=2; i <= nTimes; i++)
  {
    if(analogRead(aPin) <= iTo && analogRead(aPin) >= iFrom){pa++;}
    // update LCD line 2
    delay(msDelay);
  }

  if(pa == nTimes)
  {
    return true;
  }
  else
  {
    return false;
  }
}
