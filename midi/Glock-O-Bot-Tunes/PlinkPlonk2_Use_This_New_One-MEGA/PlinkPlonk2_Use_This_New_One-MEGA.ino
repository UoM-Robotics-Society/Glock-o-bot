#include <SPI.h>
#include <avr/pgmspace.h>
#define Tune_Length 6 // Actual tune length = 288
#define Note_Max 4 // Max nuber of notes struck per beat
#define Tempo 144 // Beats per minute

const int Beat_Time = 417; // Beat time in mSec

// Enter the tune here (A=78, D=81)
const unsigned char Tune[Tune_Length][Note_Max] = { 81,00,00,00,
                                                    00,83,00,00,
                                                    00,00,84,00,
                                                    81,84,83,00,
                                                    00,00,00,00,
                                                    81,83,84,00
                                                  };

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define shift register bits:
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

const int SR_Length    = 3; // Adjust to match instrument

const int SR_Note_A    = 2; //81
const int SR_Note_B    = 1; //83
const int SR_Note_C    = 0; //84
const int SR_Note_D    = 3; //80

const int Solenoid_On_Time = 3; // Pulse width in mSec



byte      Solenoid_ON  = B00000110;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_DataArray[SR_Length];

unsigned char    Load_Note;

void setup() {
  // start the SPI library:
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
  Clear_SR_DataArray();
  }

void loop() {

  // Main Code here, to run repeatedly
  
      
  for (int Beat_Count=0; Beat_Count < Tune_Length; Beat_Count++)
      {
        
       for (int Note_Count=0; Note_Count < Note_Max; Note_Count++)
            {
              Load_Note = Tune[Beat_Count][Note_Count];
              Load_Beat(Load_Note); 
            }
            
      Play_Beat();
      Clear_SR_DataArray();
      delay(Beat_Time);
      
      }
      
   delay(2000);
   
  }


void Clear_SR_DataArray() {

  //function body
  for ( int i=0; i < SR_Length; i++){
  SR_DataArray[i] = All_OFF;                  // clear all bits
  }
}
  


  //function body
  //if (Note == "A")
  //  {
  //    SR_DataArray[SR_Note_A] = Solenoid_ON;
  //  }


void Load_Beat(unsigned char Note) {

// Load upto 4 notes for single beat

switch (Note) {
    case 00:
      // No Note
      break;
    case 81:
      SR_DataArray[SR_Note_A] = Solenoid_ON;
      break;
    case 83:
      SR_DataArray[SR_Note_B] = Solenoid_ON;
      break;
    case 84:
      SR_DataArray[SR_Note_C] = Solenoid_ON;
      break;
    case 80:
      SR_DataArray[SR_Note_D] = Solenoid_ON;
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

}

void Play_Beat() {

// function body

    digitalWrite(SR_Reset_Pin, HIGH);
    Load_SR();
    delay(Solenoid_On_Time);
    digitalWrite(SR_Reset_Pin, LOW);
    digitalWrite(SR_Latch_Pin, HIGH);
    delayMicroseconds(20);
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
