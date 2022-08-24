#include <SPI.h>


#define Tune_Length 33 // Actual tune length = 288
#define Note_Max 1 // Max nuber of notes struck per beat
#define Tempo 144 // Beats per minute

const long Beat_Time = 208333; // Half Beat time in uSec

// Enter the tune here (79-108) A-81,B-83,C-84,D-86,E-88,F-89,G-91,A-93,B-95
const unsigned char Tune[Tune_Length][Note_Max] = { 81,83,84,86,88,89,91,93,95,96,98,81,83,84,86,88,89,91,93,95,96,98,81,83,84,86,88,89,91,93,95,96,98
                                                  };

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define shift register bits:
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

const int SR_Length    = 11; // Adjust to match instrument

const int SR_Note_A    = 10; //81
const int SR_Note_B    = 9; //83
const int SR_Note_C    = 8; //84
const int SR_Note_D    = 7; //86
const int SR_Note_E    = 6; //88
const int SR_Note_F    = 5; //89
const int SR_Note_G    = 4; //91
const int SR_Note_a    = 3; //93
const int SR_Note_as   = 2; //94 Fudged to play b!!
const int SR_Note_b    = 2; //95
const int SR_Note_c    = 1; //96
const int SR_Note_d    = 0; //98


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
    case 81:
      SR_DataArray[SR_Note_A] = Solenoid_ON;
      break;
    case 83:
      SR_DataArray[SR_Note_B] = Solenoid_ON;
      break;
    case 84:
      SR_DataArray[SR_Note_C] = Solenoid_ON;
      break;
    case 86:
      SR_DataArray[SR_Note_D] = Solenoid_ON;
      break;
    case 88:
      SR_DataArray[SR_Note_E] = Solenoid_ON;
      break;
    case 89:
      SR_DataArray[SR_Note_F] = Solenoid_ON;
      break;
    case 91:
      SR_DataArray[SR_Note_G] = Solenoid_ON;
      break;
    case 93:
      SR_DataArray[SR_Note_a] = Solenoid_ON;
      break;
    case 94:
      SR_DataArray[SR_Note_as] = Solenoid_ON;
      break;
    case 95:
      SR_DataArray[SR_Note_b] = Solenoid_ON;
      break;
    case 96:
      SR_DataArray[SR_Note_c] = Solenoid_ON;
      break;
    case 98:
      SR_DataArray[SR_Note_d] = Solenoid_ON;
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

