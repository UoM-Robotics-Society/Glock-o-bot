#include <SPI.h>


#define Tune_Length 30 // Actual tune length = 288
#define Note_Max 4 // Max nuber of notes struck per beat
#define Tempo 144 // Beats per minute

const long Beat_Time = 416667; // Beat time in uSec

// Enter the tune here (79-108) A-81,B-83,C-84,D-86,E-88,F-89,G-91,A-93,B-95
const unsigned char Tune[Tune_Length][Note_Max] = { 81,00,00,00, // 1
                                                    00,83,00,00, // 2
                                                    00,00,84,00, // 3
                                                    00,00,00,86, // 4
                                                    88,00,00,00, // 5
                                                    00,89,00,00, // 6
                                                    00,00,91,00, // 7
                                                    00,00,00,93, // 8
                                                    95,00,00,00, // 9
                                                    95,00,00,00, // 10
                                                    00,93,00,00, // 11
                                                    00,00,91,00, // 12
                                                    00,00,00,89, // 13
                                                    88,00,00,00, // 14
                                                    00,86,00,00, // 15
                                                    00,00,84,00, // 16
                                                    00,00,00,83, // 17
                                                    81,00,00,00, // 18
                                                    81,83,84,86, // 19
                                                    83,84,86,88, // 20
                                                    84,86,88,89, // 21
                                                    86,88,89,91, // 22
                                                    88,89,91,93, // 23
                                                    89,91,93,95, // 24
                                                    88,89,91,93, // 25
                                                    86,88,89,91, // 26
                                                    84,86,88,89, // 27
                                                    83,84,86,88, // 28
                                                    81,83,84,86, // 29
                                                    00,00,00,00  // 30
                                                  };

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define shift register bits:
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

const int SR_Length    = 9; // Adjust to match instrument

const int SR_Note_A    = 8; //81
const int SR_Note_B    = 7; //83
const int SR_Note_C    = 6; //84
const int SR_Note_D    = 5; //86
const int SR_Note_E    = 4; //88
const int SR_Note_F    = 3; //89
const int SR_Note_G    = 2; //91
const int SR_Note_a    = 1; //93
const int SR_Note_b    = 0; //95


const int Solenoid_On_Time = 15000; // Pulse width in uSec


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
    case 95:
      SR_DataArray[SR_Note_b] = Solenoid_ON;
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

