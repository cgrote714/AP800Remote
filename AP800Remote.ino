String prefix = "#12";
const int pinaInput1 = 1;
const int pinaInput2 = 2;
const int pinaSelect = 0;
const int pindUnlock = 10;
const int pindLED = 13;

const int Mode1 = 30;
const int Mode2 = 90;
const int Mode3 = 120;
const int Mode4 = 150;
const int ModePrecision = 10;

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

void setup()
{
  pinMode(pindUnlock, INPUT_PULLUP);
  pinMode(pindLED, OUTPUT);
  digitalWrite(pindLED, LOW);
  
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(pindUnlock) == LOW)
  {
   digitalWrite(pindLED, HIGH);
   ReadLevels();
  }
  else
  {
    digitalWrite(pindLED, LOW); 
  }

}

void ReadLevels()
{
 if(StableRead(pinaSelect, Mode1-ModePrecision, Mode1+ModePrecision, 5, 5) == true)
 {
    SetLevel(pinaInput1, Mode1_InputL1, Mode1_InputR1);
    SetLevel(pinaInput2, Mode1_InputL2, Mode1_InputR2);  
 }
 else if(StableRead(pinaSelect, Mode1-ModePrecision, Mode1+ModePrecision, 5, 5) == true)
 {
    SetLevel(pinaInput1, Mode2_InputL1, Mode2_InputR1);
    SetLevel(pinaInput2, Mode2_InputL2, Mode2_InputR2);    
 }
 else if(StableRead(pinaSelect, Mode1-ModePrecision, Mode1+ModePrecision, 5, 5) == true)
 {
    SetLevel(pinaInput1, Mode3_InputL1, Mode3_InputR1);
    SetLevel(pinaInput2, Mode3_InputL2, Mode3_InputR2);    
 }
 else
 {
    SetLevel(pinaInput1, Mode4_InputL1, Mode4_InputR1);
    SetLevel(pinaInput2, Mode4_InputL2, Mode4_InputR2);    
 }
}

void SetLevel(int pin, int Left, int Right)
{
    int db = map(analogRead(pin),0, 1023, -21, 20);
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
      Serial.println(prefix + " GAIN " + String(Left) + " I " + String(db) + " A");
      Serial.println(prefix + " MUTE " + String(Right) + " I 0");
      Serial.println(prefix + " GAIN " + String(Right) + " I " + String(db) + " A");      
    }
}

boolean StableRead(int aPin, int iFrom, int iTo, int nTimes, int msDelay)
{
  //ensures sensor reading is consistent nTimes with msDelay retest delay
  
  // update LCD line 2 with what read is looking for 
  // Wait XXX<YYY<ZZZ
  // 1234567890123456
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
