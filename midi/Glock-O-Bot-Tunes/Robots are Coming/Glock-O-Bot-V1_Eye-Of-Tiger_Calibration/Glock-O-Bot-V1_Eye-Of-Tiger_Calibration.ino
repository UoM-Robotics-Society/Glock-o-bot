#include <SPI.h>

// calibration done at 6V and T2 800, notes just striking
// All 30 notes now connected and working 22/06/2016
// Added volume 14/06/2016
// Split into 18 parts 23/07/2016


// #define Tune_Length for each part

#define Tune1_Length 60 
const int Part1_Bars = 6;


#define Note_Max 1 // Max number of notes struck per beat = 5
#define Tempo 144 // Beats per minute

const long Beat_Time = 1000000; // 1/8th beat time in uSec( tempo 8 )

const int start_delay = 0;


const unsigned char Tune1[Note_Max][Tune1_Length] PROGMEM = { 79,79,80,80,81,81,82,82,83,83,84,84,85,85,86,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,106,106,107,107,108,108  };

                                                    


int Beat_Count = 0; // Used to index Tune array
int Note_Count = 0; // Used to index Tune array
int Graphene_Signal = 0; // Used to track conductor


// define I/O pins
const int ledpin = 13; // flash led
const int SR_Latch_Pin = 53; // latch shift register
const int SR_Reset_Pin = 41; // reset shift register
const int Ext_Voltage_On = 48; // checks fo external +5v
const int Graphene_Sen_P = 23; // singnal from graphine conductor, read for status low indicates star
const int Graphene_Sen_N = 25; // singnal from graphine conductor, set low with output pin

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

// Timing check variables

unsigned long sr_start_time= 0;
unsigned long sr_stop_time= 0;

unsigned long min_us= 240000000;

unsigned long part1_start_time= 0;
unsigned long part1_tempo= 0;
unsigned long part1_stop_time= 0;

unsigned long part2_start_time= 0;

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

volatile unsigned int Next_Beat = 0; // Flag to move to next beat, set by Timmer 1 Interrupt CHANGED


void setup() {
  
  // configure I/O pins
  pinMode(ledpin, OUTPUT);
  pinMode(SR_Latch_Pin, OUTPUT);
  pinMode(SR_Reset_Pin, OUTPUT);
  pinMode(Graphene_Sen_N, OUTPUT);
  pinMode(Graphene_Sen_P, INPUT_PULLUP);
    
  digitalWrite(ledpin, LOW);
  digitalWrite(SR_Latch_Pin, LOW);
  digitalWrite(SR_Reset_Pin, LOW);
  digitalWrite(Graphene_Sen_N, LOW);
  digitalWrite(Graphene_Sen_P, HIGH);
  
  // start the SPI library:
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
  Clear_SR_Note_Data();
  Clear_SR_LED_Data();
  Reset_SR();
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo ADDED FIXED First Time THROUGH ISSUES !!!
  while (Next_Beat < 1)
        {
      //           Waitfor Beat Timer 
        }
        
  }


// --------------------------------------------------------------------------------------

void loop() 
  {

// Part 1 .......................
  digitalWrite(ledpin,HIGH); 
  Graphene_Signal = 1;
  Solenoid_On_Time_2 = 1200;
  
  while (Graphene_Signal > 0)
        {
      Graphene_Signal = digitalRead(Graphene_Sen_P); //  Wait for Graphene  ON (LOW)
        }
  Graphene_Signal = 1; // debounce 1msec
  part1_start_time = micros();
  digitalWrite(ledpin, LOW);
  delayMicroseconds(1000);
  while (Graphene_Signal > 0)
        {
      Graphene_Signal = digitalRead(Graphene_Sen_P); //  Wait for Graphene  ON
        }
  delay(start_delay);      
  Next_Beat = 0; // Reset Flag ADDED keep here as interupt can change it
  Start_Beat_Timer(); // Start Timmer 1 with correct Tempo
      
  for (int Beat_Count=0; Beat_Count < Tune1_Length; Beat_Count++)
      {
        
       for (int Note_Count=0; Note_Count < Note_Max; Note_Count++)
            {
              
              Load_Note = pgm_read_byte(&Tune1[Note_Count][Beat_Count]);
              if (Load_Note >= 149)
                {
                     // Volume 5
                     Solenoid_On_Time_2 = 1300;
                     Load_Note = Load_Note -70;
                }
              else if (Load_Note >= 114)
                {
                    // Volume 4
                    Solenoid_On_Time_2 = 1200;
                    Load_Note = Load_Note -35;
                }
              else if (Load_Note >= 79)
                {
                    // Volume 3
                    Solenoid_On_Time_2 = 800; // use to cal at lowest level , move to 1100 for playing
                    Load_Note = Load_Note -0;
                }
              else if (Load_Note >= 44)
                {
                    // Volume 2
                    Solenoid_On_Time_2 = 1000;
                    Load_Note = Load_Note +35;
                }
              else 
                {
                    // Volume 1
                    Solenoid_On_Time_2 = 900;
                    Load_Note = Load_Note +70;
                }
              Load_Beat(Load_Note);
              
              Play_Beat();
            }
      Flash_LED_ON();   

      Clear_SR_Note_Data();
      
      while (Next_Beat < 1)
        {
      //           Waitfor Beat Timer 
        }
      
      // Turn OFF LEDs
       Reset_SR();
       Next_Beat = 0;
      }
      
   digitalWrite(ledpin,HIGH); 
    
   while (Graphene_Signal < 1)
        {
      Graphene_Signal = digitalRead(Graphene_Sen_P); //  Wait for Graphene signal OFF (HIGH)
        }

  }
//-----------------
        



    
// calculate tempo's - use this for code check
// part1_tempo = ((min_us*Part1_Bars)/(part1_stop_time-part1_start_time));  
   


         


// --------------------------------------------------------------------------------------

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
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 400; // not vol calibration
      SR_Note_Data[SR_Note_G0] = Solenoid_ON;
      SR_LED_Data[SR_Note_G0] = LED_ON;
      break;
    case 80:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // #not vol calibration
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
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 50; // #not vol calibration
      SR_Note_Data[SR_Note_C1s] = Solenoid_ON;
      SR_LED_Data[SR_Note_C1s] = LED_ON;
      break;
    case 86:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // not vol calibration
      SR_Note_Data[SR_Note_D1] = Solenoid_ON;
      SR_LED_Data[SR_Note_D1] = LED_ON;
      break;
    case 87:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 150; // #not vol calibration
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
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 200; // not vol calibration
      SR_Note_Data[SR_Note_E2] = Solenoid_ON;
      SR_LED_Data[SR_Note_E2] = LED_ON;
      break;
    case 101:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 250; // not vol calibration
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
      Solenoid_On_Time_2 = Solenoid_On_Time_2 - 150; // #not vol calibration
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
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 100; // #not vol calibration
      SR_Note_Data[SR_Note_B3] = Solenoid_ON;
      SR_LED_Data[SR_Note_B3] = LED_ON;
      break;
    case 108:
      Solenoid_On_Time_2 = Solenoid_On_Time_2 + 400; // #not vol calibration
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
  Set_SR();
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

void Set_SR() 
    {
    digitalWrite(SR_Latch_Pin, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR_Latch_Pin, LOW);
    }
  

void Load_Notes_SR() {

  //function body
  sr_start_time=micros();
  digitalWrite(SR_Latch_Pin, LOW);
  for ( int i=0; i < SR_Length; i++)
  {
    SPI.transfer(SR_Note_Data[i]);           
  }
  Set_SR();
  sr_stop_time=micros();
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
  Next_Beat= 1;
}

