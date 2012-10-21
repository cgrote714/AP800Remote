String prefix = "#12";
const int pinaInput1 = 1; // potentiometer 1 on remote
const int pinaInput2 = 2; // potentiometer 2 on remote
const int pinaInput_Invert = 2;
const int pinaSelect = 0; // base station mode select 1-4 rotary switch with different resistor values
const int pindUnlock = 10; // SPST momentary ON pushbutton on remote to unlock rotary level knobs
const int pindLED = 13; // "busy" indicator on base station

const int Mode1 = 563;
const int Mode2 = 698;
const int Mode3 = 766;
const int Mode4 = 840;
const int ModePrecision = 2;

const int Mode1_InputL1 = 1;
const int Mode1_InputR1 = 2;
const int Mode1_InputL2 = 3;
const int Mode1_InputR2 = 4;

const int Mode2_InputL1 = 5;
const int Mode2_InputR1 = 6;
const int Mode2_InputL2 = 7;
const int Mode2_InputR2 = 8;

const int Mode3_InputL1 = 1;
const int Mode3_InputR1 = 2;
const int Mode3_InputL2 = 3;
const int Mode3_InputR2 = 4;

const int Mode4_InputL1 = 5;
const int Mode4_InputR1 = 6;
const int Mode4_InputL2 = 7;
const int Mode4_InputR2 = 8;

int mode;

void setup()
{
  pinMode(pindUnlock, INPUT_PULLUP);
  pinMode(pindLED, OUTPUT);
  digitalWrite(pindLED, LOW);
  
  Serial.begin(9600);
}

void loop()
{
  //Serial.println(analogRead(pinaSelect));
  
  if(digitalRead(pindUnlock) == LOW)
  {
    digitalWrite(pindLED, HIGH);
   
    if(StableRead(pinaSelect, Mode1-ModePrecision, Mode1+ModePrecision, 10, 10) == true){mode = 1;}
    if(StableRead(pinaSelect, Mode2-ModePrecision, Mode2+ModePrecision, 10, 10) == true){mode = 2;}
    if(StableRead(pinaSelect, Mode3-ModePrecision, Mode3+ModePrecision, 10, 10) == true){mode = 3;}
    if(StableRead(pinaSelect, Mode4-ModePrecision, Mode4+ModePrecision, 10, 10) == true){mode = 4;}
 
    while(digitalRead(pindUnlock) == LOW)
    {
    ReadLevels();
    delay(100);
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

void SetLevel(int pin, int Left, int Right)
{
  int db;
  
  if(pin == pinaInput_Invert)
  {
    db = map(analogRead(pin),1023, 0, -21, 20);    
  }
  else
  {
    db = map(analogRead(pin),0, 1023, -21, 20);
  }
  
  if(db == -21)
  {
    //lowest setting = mute
    Serial.println(prefix + " GAIN " + String(Left) + " I -20 A");
    Serial.println(prefix + " MUTE " + String(Left) + " I 1");
    Serial.println(prefix + " GAIN " + String(Right) + " I -20 A");
    Serial.println(prefix + " MUTE " + String(Right) + " I 1");
  }
  else
  {
    //set non-mute level
    Serial.println(prefix + " MUTE " + String(Left) + " I 0");
    Serial.print(prefix + " GAIN " + String(Left) + " I ");
    Serial.print(db);
    Serial.println(" A");
    Serial.println(prefix + " MUTE " + String(Right) + " I 0");
    Serial.print(prefix + " GAIN " + String(Right) + " I ");
    Serial.print(db);
    Serial.println(" A");      
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
