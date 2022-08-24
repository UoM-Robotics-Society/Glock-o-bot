#include <SPI.h>
#include <avr/pgmspace.h>
#define TuneLength 100
const String Tune[] = {"C","B","C","C","B","A","D","C","D","D","D"};
const  int Note_Count = 11;
// define shift register bits:
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

const int SR_Length    = 3;

const int SR_Note_A    = 2;
const int SR_Note_B    = 1;
const int SR_Note_C    = 0;
const int SR_Note_D    = 3;

const int Solenoid_On_Time = 3; // Pulse width in mSec

const int Beat = 1000; // Beat time in mSec

byte      Solenoid_ON  = B00000110;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_DataArray[SR_Length];

String    PL_Note;
String    IP_Note;

void setup() {
  // start the SPI library:
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  Clear_SR_DataArray();
  }

void loop() {

  // put your main code here, to run repeatedly
  
      Serial.begin(9600);
  for (int i=0; i < Note_Count; i++) Serial.println(Tune[i]);
  for (int i=0; i < Note_Count; i++)
      {
      PL_Note = Tune[i];
      Play_Note(PL_Note); 
      delay(Beat);
      }
      delay(200);    
   
  }


void Clear_SR_DataArray() {

  //function body
  for ( int i=0; i < SR_Length; i++){
  SR_DataArray[i] = All_OFF;                  // clear all bits
  }
}
  
void Set_SR_DataArray(String Note) {

  //function body
  if (Note == "A")
    {
      SR_DataArray[SR_Note_A] = Solenoid_ON;
    }
else if (Note == "B")
    {
      SR_DataArray[SR_Note_B] = Solenoid_ON;
    }
else if (Note == "C")
    {
      SR_DataArray[SR_Note_C] = Solenoid_ON;
    }
else if (Note == "D")
    {
      SR_DataArray[SR_Note_A] = Solenoid_ON;
      SR_DataArray[SR_Note_B] = Solenoid_ON;
      SR_DataArray[SR_Note_C] = Solenoid_ON;
    }
else
    {
      // do Thing C
    }
 
}

void Play_Note(String SR_Note) {

// function body

    Set_SR_DataArray(SR_Note);
    digitalWrite(SR_Reset_Pin, HIGH);
    Load_SR();
    delay(Solenoid_On_Time);
    digitalWrite(SR_Reset_Pin, LOW);
    digitalWrite(SR_Latch_Pin, HIGH);
    delay(1);
    digitalWrite(SR_Latch_Pin, LOW);
    Clear_SR_DataArray();

}


void Load_SR() {

  //function body
  digitalWrite(SR_Latch_Pin, LOW);
  for ( int i=0; i < SR_Length; i++){
  SPI.transfer(SR_DataArray[i]);                  // clear all bits
  }
  digitalWrite(SR_Latch_Pin, HIGH);
  delay(1);
  digitalWrite(SR_Latch_Pin, LOW);
}
