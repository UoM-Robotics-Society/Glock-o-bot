#include <SPI.h>
//#include <avr/pgmspace.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>

#define Tune_Length 30 // Actual tune length = 288
#define Note_Max 4 // Max nuber of notes struck per beat
#define Tempo 144 // Beats per minute

const int Beat_Time = 417; // Beat time in mSec

// Enter the tune here (79-108)
const unsigned char Tune[Tune_Length][Note_Max] = { 81,00,00,00, // 1
                                                    00,83,00,00, // 2
                                                    00,00,84,00, // 3
                                                    81,84,83,00, // 4
                                                    00,00,00,00, // 5
                                                    81,83,84,00, // 6
                                                    81,00,00,00, // 7
                                                    00,00,84,00, // 8
                                                    81,84,83,00, // 9
                                                    81,84,83,00, // 10
                                                    00,00,00,00, // 11
                                                    81,83,84,00, // 12
                                                    00,00,84,00, // 13
                                                    81,84,83,00, // 14
                                                    00,00,00,00, // 15
                                                    81,83,84,00, // 16
                                                    81,00,00,00, // 17
                                                    00,00,84,00, // 18
                                                    81,84,83,00, // 19
                                                    81,84,83,00, // 20
                                                    00,00,00,00, // 21
                                                    81,83,84,00, // 22
                                                    00,00,84,00, // 23
                                                    81,84,83,00, // 24
                                                    00,00,00,00, // 25
                                                    81,83,84,00, // 26
                                                    81,00,00,00, // 27
                                                    00,00,84,00, // 28
                                                    81,84,83,00, // 29
                                                    81,84,83,00  // 30
                                                  };

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define shift register bits:
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

const int Test_Pin =7;

const int SR_Length    = 3; // Adjust to match instrument

const int SR_Note_A    = 2; //81
const int SR_Note_B    = 1; //83
const int SR_Note_C    = 0; //84
const int SR_Note_D    = 3; //80

const int Solenoid_On_Time = 30; // Pulse width in mSec



byte      Solenoid_ON  = B00000110;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_DataArray[SR_Length];

unsigned char    Load_Note;
unsigned int Timer1_Beat_Count =0;
char Next_Beat = 0;


void setup() {
  
  // start the SPI library:
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
  
  pinMode(Test_Pin, OUTPUT); // Used for debug
  
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);
  digitalWrite(Test_Pin, LOW);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
  Clear_SR_DataArray();
  Start_Beat_Timer();
   
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
 //   delay(Beat_Time);
      while (Next_Beat < 100){
        Next_Beat++;
        delay(2);
        } // Waitfor Beat Timer Interupt
      Next_Beat = 0;
      }
      
   delay(1000);
   
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

void Start_Beat_Timer() {

  cli();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  Timer1_Beat_Count = 0x0BDC;   // preload timer 65536-16MHz/256/2.4Hz (1/BeatTime)39494
  TCNT1 = Timer1_Beat_Count;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
//  TCCR1B |= (1 << CS10); 
  sei();             // enable all interrupts

}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = Timer1_Beat_Count;   // Reload timer1 with beacount
  Next_Beat = 1;
  digitalWrite(Test_Pin, HIGH);
  delay(1);
  digitalWrite(Test_Pin, LOW);
}

