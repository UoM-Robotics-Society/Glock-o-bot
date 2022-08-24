#include <SPI.h>


#define Tune_Length 8 // Actual tune length = 288
#define Note_Max 4 // Max number of notes struck per beat
#define Tempo 200 // Beats per minute

const long Beat_Time = 75000; // Beat time in uSec

//Enter the tune here (79-108)
const unsigned char Tune[Note_Max][Tune_Length] = { {79,79,79,79,79,79,79,79},
                                                    {00,00,00,00,00,00,00,00},
                                                    {00,00,00,00,00,00,00,00},
                                                    {00,00,00,00,00,00,00,00}};
                                                    
//const unsigned char Tune[Note_Max][Tune_Length] = {79,79,79,79,79,79,79,79};

int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array


// define I/O pins
const int SR_Latch_Pin = 53;
const int SR_Reset_Pin = 41;

// define Shift Register Length
const int SR_Length    = 29; // Adjust to match instrument

// define key positions in sfift register
const int SR_Note_G0   = 28; //79
const int SR_Note_A1   = 27; //81
const int SR_Note_B1   = 26; //83
const int SR_Note_C1   = 25; //84
const int SR_Note_D1   = 24; //86
const int SR_Note_E1   = 23; //88
const int SR_Note_F1   = 22; //89
const int SR_Note_G1   = 21; //91
const int SR_Note_A2   = 20; //93
const int SR_Note_B2   = 19; //95
const int SR_Note_C2   = 18; //96
const int SR_Note_D2   = 17; //98
const int SR_Note_E2   = 16; //100
const int SR_Note_F2   = 15; //101
const int SR_Note_G2   = 14; //103
const int SR_Note_A3   = 13; //105
const int SR_Note_B3   = 12; //107
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


const int Solenoid_On_Time_1 = 600; // Pulse width in uSec
const int Solenoid_Off_Time = 600; // Pulse width in uSec
const int Solenoid_On_Time_2 = 600; // Pulse width in uSec

// Shift Register Output patterns

byte      Solenoid_ON  = B00000010;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_Note_Data[SR_Length];
byte      SR_LED_Data[SR_Length]; 

unsigned char    Load_Note;

volatile unsigned int Timer1_Beat_Count =0; // Used to set Timmer 1 to correct tempo

volatile unsigned int Next_Beat = 1; // Flag to move to next beat, set by Timmer 1 Interrupt CHANGED


void setup() {
  
  // start the SPI library:
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
    
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);

  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
  Clear_SR_Note_Data();
  Clear_SR_LED_Data();
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo ADDED FIXED First Time THROUGH ISSUES
  while (Next_Beat < 2)
        {
      //           Waitfor Beat Timer 
        }
        
  }

void loop() 
  {
  
  
  Next_Beat = 1; // Reset Flag ADDED
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo
      
  for (int Beat_Count=0; Beat_Count < Tune_Length; Beat_Count++)
      {
        
       for (int Note_Count=0; Note_Count < Note_Max; Note_Count++)
            {
              Load_Note = Tune[Note_Count][Beat_Count];
              Load_Beat(Load_Note); 
              Play_Beat();
            }
      Flash_LED_ON();   

      Clear_SR_Note_Data();

      while (Next_Beat < 2)
        {
      //           Waitfor Beat Timer 
        }
      
      // Turn OFF LEDs
       digitalWrite(SR_Reset_Pin, LOW);
       digitalWrite(SR_Latch_Pin, HIGH);
       delayMicroseconds(20);
       digitalWrite(SR_Latch_Pin, LOW);
       Next_Beat = 1;
      }
      
   for (int j=0; j <100; j++)
   {
    delayMicroseconds(15000);
   }
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
      SR_Note_Data[SR_Note_G0] = Solenoid_ON;
      SR_LED_Data[SR_Note_G0] = LED_ON;
      break;
    case 80:
      SR_Note_Data[SR_Note_G0s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G0s] = LED_ON;
      break;
    case 81:
      SR_Note_Data[SR_Note_A1] = Solenoid_ON;
      SR_LED_Data[SR_Note_A1] = LED_ON;
      break;
    case 82:
      SR_Note_Data[SR_Note_A1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A1s] = LED_ON;
      break;
    case 83:
      SR_Note_Data[SR_Note_B1] = Solenoid_ON;
      SR_LED_Data[SR_Note_B1] = LED_ON;
      break;
    case 84:
      SR_Note_Data[SR_Note_C1] = Solenoid_ON;
      SR_LED_Data[SR_Note_C1] = LED_ON;
      break;
    case 85:
      SR_Note_Data[SR_Note_C1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_C1s] = LED_ON;
      break;
    case 86:
      SR_Note_Data[SR_Note_D1] = Solenoid_ON;
      SR_LED_Data[SR_Note_D1] = LED_ON;
      break;
    case 87:
      SR_Note_Data[SR_Note_D1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_D1s] = LED_ON;
      break;
    case 88:
      SR_Note_Data[SR_Note_E1] = Solenoid_ON;
      SR_LED_Data[SR_Note_E1] = LED_ON;
      break;
    case 89:
      SR_Note_Data[SR_Note_F1] = Solenoid_ON;
      SR_LED_Data[SR_Note_F1] = LED_ON;
      break;
    case 90:
      SR_Note_Data[SR_Note_F1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_F1s] = LED_ON;
      break;
    case 91:
      SR_Note_Data[SR_Note_G1] = Solenoid_ON;
      SR_LED_Data[SR_Note_G1] = LED_ON;
      break;
    case 92:
      SR_Note_Data[SR_Note_G1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G1s] = LED_ON;
      break;
    case 93:
      SR_Note_Data[SR_Note_A2] = Solenoid_ON;
      SR_LED_Data[SR_Note_A2] = LED_ON;
      break;
    case 94:
      SR_Note_Data[SR_Note_A2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A2s] = LED_ON;
      break;
    case 95:
      SR_Note_Data[SR_Note_B2] = Solenoid_ON;
      SR_LED_Data[SR_Note_B2] = LED_ON;
      break;
    case 96:
      SR_Note_Data[SR_Note_C2] = Solenoid_ON;
      SR_LED_Data[SR_Note_C2] = LED_ON;
      break;
    case 97:
      SR_Note_Data[SR_Note_C2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_C2s] = LED_ON;
      break;
    case 98:
      SR_Note_Data[SR_Note_D2] = Solenoid_ON;
      SR_LED_Data[SR_Note_D2] = LED_ON;
      break;
    case 99:
      SR_Note_Data[SR_Note_D2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_D2s] = LED_ON;
      break;
    case 100:
      SR_Note_Data[SR_Note_E2] = Solenoid_ON;
      SR_LED_Data[SR_Note_E2] = LED_ON;
      break;
    case 101:
      SR_Note_Data[SR_Note_F2] = Solenoid_ON;
      SR_LED_Data[SR_Note_F2] = LED_ON;
      break;
    case 102:
      SR_Note_Data[SR_Note_F2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_F2s] = LED_ON;
      break;
    case 103:
      SR_Note_Data[SR_Note_G2] = Solenoid_ON;
      SR_LED_Data[SR_Note_G2] = LED_ON;
      break;
    case 104:
      SR_Note_Data[SR_Note_G2s] = Solenoid_ON;
      SR_LED_Data[SR_Note_G2s] = LED_ON;
      break;
    case 105:
      SR_Note_Data[SR_Note_A3] = Solenoid_ON;
      SR_LED_Data[SR_Note_A3] = LED_ON;
      break;
    case 106:
      SR_Note_Data[SR_Note_A3s] = Solenoid_ON;
      SR_LED_Data[SR_Note_A3s] = LED_ON;
      break;
//    case 107:
//      SR_Note_Data[SR_Note_B3] = Solenoid_ON;
//      SR_LED_Data[SR_Note_B3] = LED_ON;
//      break;
//    case 108:
//      SR_Note_Data[SR_Note_C3] = Solenoid_ON;
//      SR_LED_Data[SR_Note_C3] = LED_ON;
//      break;
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

// function body

    digitalWrite(SR_Reset_Pin, HIGH);
    Load_Notes_SR();
    delayMicroseconds(Solenoid_On_Time_1);
    digitalWrite(SR_Reset_Pin, LOW);
    digitalWrite(SR_Latch_Pin, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR_Latch_Pin, LOW);
    delayMicroseconds(Solenoid_Off_Time);
    
    digitalWrite(SR_Reset_Pin, HIGH);
    Load_Notes_SR();
    delayMicroseconds(Solenoid_On_Time_2);
    digitalWrite(SR_Reset_Pin, LOW);
    digitalWrite(SR_Latch_Pin, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR_Latch_Pin, LOW);
    Clear_SR_Note_Data();

}


void Load_Notes_SR() {

  //function body
  digitalWrite(SR_Latch_Pin, LOW);
  for ( int i=0; i < SR_Length; i++){
  SPI.transfer(SR_Note_Data[i]);           
  }
  digitalWrite(SR_Latch_Pin, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR_Latch_Pin, LOW);
}

void Start_Beat_Timer() {

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
  TCNT1 = Timer1_Beat_Count;   // preload timer
  TCCR1B |= (1 << CS12);    // Sets 256 prescaler 16 uSec counts
  Timer1_Beat_Count = (65536 -(Beat_Time/16));   // preload timer 65536-(416666/16) = 39494
  interrupts();             // enable all interrupts

}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = Timer1_Beat_Count;   // Reload timer1 with beacount
  Next_Beat= 2;
 // digitalWrite(Test_Pin, HIGH);
 // delayMicroseconds(50);
 // digitalWrite(Test_Pin, LOW);
}

