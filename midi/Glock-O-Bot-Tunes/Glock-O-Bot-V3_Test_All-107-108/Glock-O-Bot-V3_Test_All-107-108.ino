#include <SPI.h>


#define Tune_Length 28 // Actual tune length = 288
#define Note_Max 1 // Max nuber of notes struck per beat
#define Tempo 144 // Beats per minute

const long Beat_Time = 208333; // Half Beat time in uSec

// Enter the tune here (79-108)
const unsigned char Tune[Note_Max][Tune_Length] = { 79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106  };

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define I/O pins
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

// define Shift Register Length
const int SR_Length    = 28; // Adjust to match instrument

// define key positions in sfift register
const int SR_Note_G0   = 27; //79
const int SR_Note_A1   = 26; //81
const int SR_Note_B1   = 25; //83
const int SR_Note_C1   = 24; //84
const int SR_Note_D1   = 23; //86
const int SR_Note_E1   = 22; //88
const int SR_Note_F1   = 21; //89
const int SR_Note_G1   = 20; //91
const int SR_Note_A2   = 19; //93
const int SR_Note_B2   = 18; //95
const int SR_Note_C2   = 17; //96
const int SR_Note_D2   = 16; //98
const int SR_Note_E2   = 15; //100
const int SR_Note_F2   = 14; //101
const int SR_Note_G2   = 13; //103
const int SR_Note_A3   = 12; //105
//const int SR_Note_B3   = 13; //107
//const int SR_Note_C3   = 12; //108
const int SR_Note_A3s  = 11; //106
const int SR_Note_G2s  = 10; //104
const int SR_Note_F2s  =  9; //102
const int SR_Note_D2s  =  8; //99
const int SR_Note_C2s  =  7; //97
const int SR_Note_A2s  =  6; //94
const int SR_Note_G1s  =  5; //92
const int SR_Note_F1s  =  4; //90
const int SR_Note_D1s  =  3; //87
const int SR_Note_C1s  =  2; //85
const int SR_Note_A1s  =  1; //82
const int SR_Note_G0s  =  0; //80


const int Solenoid_On_Time = 2000; // Pulse width in uSec


// Shift Register Output patterns

byte      Solenoid_ON  = B00000110;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_DataArray[SR_Length];

unsigned char    Load_Note;

volatile unsigned int Timer1_Beat_Count =0; // Used to set Timmer 1 to correct tempo

volatile char Next_Beat = 0; // Flag to move to next beat, set by Timmer 1 Interrupt


void setup() {
  
  // start the SPI library:
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
    
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);

  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
  Clear_SR_DataArray();
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo
 
   
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

     while (Next_Beat < 1)
        {
          // Waitfor Beat Timer 
        }

      Next_Beat = 0; // Reset Flag
      }
      
   delayMicroseconds(15000);
   
  }


void Clear_SR_DataArray() {

  //function body
  for ( int i=0; i < SR_Length; i++){
  SR_DataArray[i] = All_OFF;                  // clear all bits
  }
}
  


void Load_Beat(unsigned char Note) {

// Load upto 4 notes for single beat

switch (Note) {
    case 00:
      // No Note
      break;
    case 79:
      SR_DataArray[SR_Note_G0] = Solenoid_ON;
      break;
    case 80:
      SR_DataArray[SR_Note_G0s] = Solenoid_ON;
      break;
    case 81:
      SR_DataArray[SR_Note_A1] = Solenoid_ON;
      break;
    case 82:
      SR_DataArray[SR_Note_A1s] = Solenoid_ON;
      break;
    case 83:
      SR_DataArray[SR_Note_B1] = Solenoid_ON;
      break;
    case 84:
      SR_DataArray[SR_Note_C1] = Solenoid_ON;
      break;
    case 85:
      SR_DataArray[SR_Note_C1s] = Solenoid_ON;
      break;
    case 86:
      SR_DataArray[SR_Note_D1] = Solenoid_ON;
      break;
    case 87:
      SR_DataArray[SR_Note_D1s] = Solenoid_ON;
      break;
    case 88:
      SR_DataArray[SR_Note_E1] = Solenoid_ON;
      break;
    case 89:
      SR_DataArray[SR_Note_F1] = Solenoid_ON;
      break;
    case 90:
      SR_DataArray[SR_Note_F1s] = Solenoid_ON;
      break;
    case 91:
      SR_DataArray[SR_Note_G1] = Solenoid_ON;
      break;
    case 92:
      SR_DataArray[SR_Note_G1s] = Solenoid_ON;
      break;
    case 93:
      SR_DataArray[SR_Note_A2] = Solenoid_ON;
      break;
    case 94:
      SR_DataArray[SR_Note_A2s] = Solenoid_ON;
      break;
    case 95:
      SR_DataArray[SR_Note_B2] = Solenoid_ON;
      break;
    case 96:
      SR_DataArray[SR_Note_C2] = Solenoid_ON;
      break;
    case 97:
      SR_DataArray[SR_Note_C2s] = Solenoid_ON;
      break;
    case 98:
      SR_DataArray[SR_Note_D2] = Solenoid_ON;
      break;
    case 99:
      SR_DataArray[SR_Note_D2s] = Solenoid_ON;
      break;
    case 100:
      SR_DataArray[SR_Note_E2] = Solenoid_ON;
      break;
    case 101:
      SR_DataArray[SR_Note_F2] = Solenoid_ON;
      break;
    case 102:
      SR_DataArray[SR_Note_F2s] = Solenoid_ON;
      break;
    case 103:
      SR_DataArray[SR_Note_G2] = Solenoid_ON;
      break;
    case 104:
      SR_DataArray[SR_Note_G2s] = Solenoid_ON;
      break;
    case 105:
      SR_DataArray[SR_Note_A3] = Solenoid_ON;
      break;
    case 106:
      SR_DataArray[SR_Note_A3s] = Solenoid_ON;
      break;
//    case 107:
//      SR_DataArray[SR_Note_B3] = Solenoid_ON;
//      break;
//    case 108:
//      SR_DataArray[SR_Note_C3] = Solenoid_ON;
//      break;
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
    delayMicroseconds(Solenoid_On_Time);
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
  delayMicroseconds(20);
  digitalWrite(SR_Latch_Pin, LOW);
}

void Start_Beat_Timer() {

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  Timer1_Beat_Count = (65536 -(Beat_Time/16));   // preload timer 65536-(416666/16) = 39494
  TCNT1 = Timer1_Beat_Count;   // preload timer
  TCCR1B |= (1 << CS12);    // Sets 256 prescaler 16 uSec counts
  interrupts();             // enable all interrupts

}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = Timer1_Beat_Count;   // Reload timer1 with beacount
  Next_Beat = 1;
 // digitalWrite(Test_Pin, HIGH);
 // delayMicroseconds(50);
 // digitalWrite(Test_Pin, LOW);
}

