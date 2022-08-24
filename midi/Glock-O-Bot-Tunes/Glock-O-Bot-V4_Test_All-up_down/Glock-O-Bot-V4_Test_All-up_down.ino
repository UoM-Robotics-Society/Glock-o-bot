#include <SPI.h>


#define Tune_Length 34 // Actual tune length = 288
#define Note_Max 4 // Max number of notes struck per beat
#define Tempo 200 // Beats per minute

const long Beat_Time = 1000000; // Beat time in uSec 133333

// Enter the tune here (79-108)
const unsigned char Tune[Note_Max][Tune_Length] = { {79,79,79,79,79,81,83,84,86,88,89,91,93,95,96,98,100,101,103,105,107,108,106,104,102,99,97,94,92,90,87,85,82,80}, 
                                                    {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, 
                                                    {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},
                                                    {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},};
                                                    
//const unsigned char Tune[Note_Max][Tune_Length] = { {79,81,83,84,86,88,89,91,93,95,96,98,100,101,103,105,107,108,106,104,102,99,97,94,92,90,87,85}, 
   //                                                 {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, 
  //                                                 {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},
 //                                                   {00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},};


                                                    

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define I/O pins
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

// define Shift Register Length
const int SR_Length    = 30; // Adjust to match instrument

// define key positions in sfift register
const int SR_Note_G0   = 29; //79
const int SR_Note_A1   = 28; //81
const int SR_Note_B1   = 27; //83
const int SR_Note_C1   = 26; //84
const int SR_Note_D1   = 25; //86
const int SR_Note_E1   = 24; //88
const int SR_Note_F1   = 23; //89
const int SR_Note_G1   = 22; //91
const int SR_Note_A2   = 21; //93
const int SR_Note_B2   = 20; //95
const int SR_Note_C2   = 19; //96
const int SR_Note_D2   = 18; //98
const int SR_Note_E2   = 17; //100
const int SR_Note_F2   = 16; //101
const int SR_Note_G2   = 15; //103
const int SR_Note_A3   = 14; //105
const int SR_Note_B3   = 13; //107
const int SR_Note_C3   = 12; //108
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


// Voltage set to 6.5V use Time_2 between 1000 - 2000 for vol 0-100

const int Solenoid_On_Time_1 = 1000; // Pulse width in uSec Fixed
const int Solenoid_Off_Time = 1000; // Pulse width in uSec Fixed
int Solenoid_On_Time_2 = 1000; // Pulse width in uSec Variable


// Shift Register Output patterns

byte      Solenoid_ON  = B00000010;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_Note_Data[SR_Length];
byte      SR_LED_Data[SR_Length]; 

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
  Clear_SR_Note_Data();
  Clear_SR_LED_Data();
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo
 
   
  }

void loop() {

  // Main Code here, to run repeatedly
  
      
  for (int Beat_Count=0; Beat_Count < Tune_Length; Beat_Count++)
      {
        
       for (int Note_Count=0; Note_Count < Note_Max; Note_Count++)
            {
              Load_Note = Tune[Note_Count][Beat_Count];
              Load_Beat(Load_Note); 
              Solenoid_On_Time_2 = 1200;
              Play_Beat();
            }
         Flash_LED_ON();   

      Clear_SR_Note_Data();

     while (Next_Beat < 1)
        {
          // Waitfor Beat Timer 
        }

      Next_Beat = 0; // Reset Flag
      
      // Turn OFF LEDs
        digitalWrite(SR_Reset_Pin, LOW);
        digitalWrite(SR_Latch_Pin, HIGH);
        delayMicroseconds(20);
        digitalWrite(SR_Latch_Pin, LOW);

      }
      
   delayMicroseconds(15000);
   
  }


void Clear_SR_Note_Data() {

  //function body
  for ( int i=0; i < SR_Length; i++){
  SR_Note_Data[i] = All_OFF;                  // clear all bits
  }
}
  
void Clear_SR_LED_Data() {

  //function body
  for ( int i=0; i < SR_Length; i++){
  SR_LED_Data[i] = All_OFF;                  // clear all bits
  }
}

void Load_Beat(unsigned char Note) {

// Load upto 4 notes for single beat

switch (Note) {
    case 00:
      // No Note
      break;
    case 79:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 700; // not vol calibration
      SR_Note_Data[SR_Note_G0] = Solenoid_ON;
      SR_LED_Data[SR_Note_G0] = LED_ON;
      break;
    case 80:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 0; // #not vol calibration
      SR_Note_Data[SR_Note_G0s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G0s] = LED_ON;
      break;
    case 81:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 220; // not vol calibration
      SR_Note_Data[SR_Note_A1] = Solenoid_ON;
      SR_LED_Data[SR_Note_A1] = LED_ON;
      break;
    case 82:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 120; // #not vol calibration
      SR_Note_Data[SR_Note_A1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A1s] = LED_ON;
      break;
    case 83:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 300; // not vol calibration
      SR_Note_Data[SR_Note_B1] = Solenoid_ON;
      SR_LED_Data[SR_Note_B1] = LED_ON;
      break;
    case 84:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 220; // not vol calibration
      SR_Note_Data[SR_Note_C1] = Solenoid_ON;
      SR_LED_Data[SR_Note_C1] = LED_ON;
      break;
    case 85:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 100; // #not vol calibration
      SR_Note_Data[SR_Note_C1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_C1s] = LED_ON;
      break;
    case 86:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // not vol calibration
      SR_Note_Data[SR_Note_D1] = Solenoid_ON;
      SR_LED_Data[SR_Note_D1] = LED_ON;
      break;
    case 87:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 100; // #not vol calibration
      SR_Note_Data[SR_Note_D1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_D1s] = LED_ON;
      break;
    case 88:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 200; // not vol calibration
      SR_Note_Data[SR_Note_E1] = Solenoid_ON;
      SR_LED_Data[SR_Note_E1] = LED_ON;
      break;
    case 89:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 200; // not vol calibration
      SR_Note_Data[SR_Note_F1] = Solenoid_ON;
      SR_LED_Data[SR_Note_F1] = LED_ON;
      break;
    case 90:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 150; // #not vol calibration
      SR_Note_Data[SR_Note_F1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_F1s] = LED_ON;
      break;
    case 91:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // not vol calibration
      SR_Note_Data[SR_Note_G1] = Solenoid_ON;
      SR_LED_Data[SR_Note_G1] = LED_ON;
      break;
    case 92:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 50; // #not vol calibration
      SR_Note_Data[SR_Note_G1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G1s] = LED_ON;
      break;
    case 93:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 350; // not vol calibration
      SR_Note_Data[SR_Note_A2] = Solenoid_ON;
      SR_LED_Data[SR_Note_A2] = LED_ON;
      break;
    case 94:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 50; // #not vol calibration
      SR_Note_Data[SR_Note_A2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A2s] = LED_ON;
      break;
    case 95:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 400; // not vol calibration
      SR_Note_Data[SR_Note_B2] = Solenoid_ON;
      SR_LED_Data[SR_Note_B2] = LED_ON;
      break;
    case 96:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 300; // not vol calibration
      SR_Note_Data[SR_Note_C2] = Solenoid_ON;
      SR_LED_Data[SR_Note_C2] = LED_ON;
      break;
    case 97:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 0; // #not vol calibration
      SR_Note_Data[SR_Note_C2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_C2s] = LED_ON;
      break;
    case 98:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 300; // not vol calibration
      SR_Note_Data[SR_Note_D2] = Solenoid_ON;
      SR_LED_Data[SR_Note_D2] = LED_ON;
      break;
    case 99:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 50; // #not vol calibration
      SR_Note_Data[SR_Note_D2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_D2s] = LED_ON;
      break;
    case 100:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 270; // not vol calibration
      SR_Note_Data[SR_Note_E2] = Solenoid_ON;
      SR_LED_Data[SR_Note_E2] = LED_ON;
      break;
    case 101:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 420; // not vol calibration
      SR_Note_Data[SR_Note_F2] = Solenoid_ON;
      SR_LED_Data[SR_Note_F2] = LED_ON;
      break;
    case 102:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 50; // #not vol calibration
      SR_Note_Data[SR_Note_F2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_F2s] = LED_ON;
      break;
    case 103:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 350; // not vol calibration
      SR_Note_Data[SR_Note_G2] = Solenoid_ON;
      SR_LED_Data[SR_Note_G2] = LED_ON;
      break;
    case 104:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 220; // #not vol calibration
      SR_Note_Data[SR_Note_G2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G2s] = LED_ON;
      break;
    case 105:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // not vol calibration
      SR_Note_Data[SR_Note_A3] = Solenoid_ON;
      SR_LED_Data[SR_Note_A3] = LED_ON;
      break;
    case 106:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 50; // #not vol calibration
      SR_Note_Data[SR_Note_A3s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A3s] = LED_ON;
      break;
    case 107:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 50; // #not vol calibration
      SR_Note_Data[SR_Note_B3] = Solenoid_ON;
      SR_LED_Data[SR_Note_B3] = LED_ON;
      break;
    case 108:
    Solenoid_On_Time_2 = Solenoid_On_Time_2 + 550; // #not vol calibration
      SR_Note_Data[SR_Note_C3] = Solenoid_ON;
      SR_LED_Data[SR_Note_C3] = LED_ON;
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

}

void Flash_LED_ON() {

// function body

    digitalWrite(SR_Reset_Pin, HIGH);
    Load_LED_SR();
    Clear_SR_LED_Data();

}


void Load_LED_SR() {

  //function body
  digitalWrite(SR_Latch_Pin, LOW);
  for ( int i=0; i < SR_Length; i++){
  SPI.transfer(SR_LED_Data[i]);                  // clear all bits
  }
  digitalWrite(SR_Latch_Pin, HIGH);
  delayMicroseconds(20);
  digitalWrite(SR_Latch_Pin, LOW);
}

void Play_Beat() {

// sends two timed pulses to the selected solenoid

    //First Pulse
    digitalWrite(SR_Reset_Pin, HIGH);
    Load_Notes_SR();
    delayMicroseconds(Solenoid_On_Time_1);
    Reset_SR();
    delayMicroseconds(Solenoid_Off_Time);

    //Second Pulse
    digitalWrite(SR_Reset_Pin, HIGH);
    Load_Notes_SR();
    delayMicroseconds(Solenoid_On_Time_2);
    Reset_SR();
    Clear_SR_Note_Data();


}

void Reset_SR() 
    {
    digitalWrite(SR_Reset_Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(SR_Latch_Pin, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR_Latch_Pin, LOW);
//    digitalWrite(SR_Reset_Pin, HIGH);
    }

void Load_Notes_SR() {

  //function body
  digitalWrite(SR_Latch_Pin, LOW);
  for ( int i=0; i < SR_Length; i++){
  SPI.transfer(SR_Note_Data[i]);                  // clear all bits
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

