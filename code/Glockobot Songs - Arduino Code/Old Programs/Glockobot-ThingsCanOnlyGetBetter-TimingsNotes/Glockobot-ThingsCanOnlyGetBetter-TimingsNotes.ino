#include <SPI.h>
#include <SD.h>


// Information about songs for loading
const int MaxNumSongs = 1;  // Number of songs to be loaded
const int NumSongs = 1;     // Number of songs to be loaded

const int Tune1Length = 1014;
const int Tune1ArrayLength = 190;
const int Tune1NoteMax = 5;
const int Tune1Tempo = 124;
const int Tune1BeatDenom = 4;

  // Harry Potter
const unsigned int Tune1[2][Tune1ArrayLength] PROGMEM = {
  {1,2,2,66,66,66,130,130,130,194,194,194,194,258,258,258,258,282,282,282,290,290,290,314,314,314,318,318,322,322,350,350,354,354,386,386,386,410,410,418,419,450,450,450,450,498,502,506,510,514,514,514,514,554,562,562,566,578,578,578,578,610,622,626,626,630,630,634,638,642,642,642,642,654,654,654,666,666,674,674,674,682,682,682,682,690,690,690,690,697,698,698,698,705,705,706,706,714,718,718,726,730,730,738,746,754,758,770,770,770,770,782,782,783,794,794,794,802,802,802,814,814,814,825,826,834,834,834,834,846,846,846,854,858,858,858,866,866,866,867,878,882,889,890,890,893,893,893,897,897,897,897,910,910,910,926,926,934,938,938,946,946,946,946,950,962,962,962,962,970,974,974,974,982,986,986,986,994,994,994,994,1002,1002,1002,1010,1010,1010,1014,1014,1014},
  {75,99,87,82,94,98,73,85,97,68,80,92,99,99,103,106,75,101,89,104,99,87,103,101,79,94,80,99,82,101,96,99,98,94,97,73,85,85,90,85,89,68,99,92,80,94,96,99,101,75,87,94,99,87,104,99,103,101,94,82,98,82,82,99,103,98,101,82,82,92,85,89,97,92,89,97,89,92,85,92,97,102,85,97,94,97,92,101,85,78,97,90,102,96,99,92,80,80,96,99,80,96,99,94,96,99,101,75,99,106,103,103,99,106,99,104,108,99,106,103,96,99,104,94,103,101,94,98,82,101,94,98,82,99,103,96,98,82,101,94,82,82,83,94,98,84,96,100,85,92,97,101,92,101,97,101,97,85,97,101,85,92,97,102,101,80,96,99,104,80,96,99,104,80,96,99,104,99,80,96,104,99,96,104,96,99,104,96,99,104}};


const int start_delay = 0;

// Create program variables
int On_Signal;    // Used to track conductor
unsigned int Load_Note;
unsigned int Load_Timing;

int NoteCount;
int BeatCount;
int SongCount;
int ArrayCount;

// Create array to store information about songs
int Tune_Info[MaxNumSongs][4];

// Define I/O pins
//const int Conductor_Pin = 5;  // Pin number for connecting conductor or button, Arduino Uno
const int Conductor_Pin = 23;  // Pin number for connecting conductor or button, Arduino Mega
const int Graphene_Sen_N = 25; // singnal from graphine conductor, set low with output pin
const int SR_Latch_Pin = 53; // latch shift register
const int SR_Reset_Pin = 41; // reset shift register
  // SD Card Reader - MOSI-11, MISO-12, SCK-13, CS-8

// Define Shift Register Length
const int SR_Length    = 30; // Adjust to match instrument

// Define key positions in shift register
const int SR_Note_G0   = 29; // Midi note 79
const int SR_Note_A1   = 28; // Midi note 81
const int SR_Note_B1   = 27; // Midi note 83
const int SR_Note_C1   = 26; // Midi note 84
const int SR_Note_D1   = 25; // Midi note 86
const int SR_Note_E1   = 24; // Midi note 88
const int SR_Note_F1   = 23; // Midi note 89
const int SR_Note_G1   = 22; // Midi note 91
const int SR_Note_A2   = 21; // Midi note 93
const int SR_Note_B2   = 20; // Midi note 95
const int SR_Note_C2   = 19; // Midi note 96
const int SR_Note_D2   = 18; // Midi note 98
const int SR_Note_E2   = 17; // Midi note 100
const int SR_Note_F2   = 16; // Midi note 101
const int SR_Note_G2   = 15; // Midi note 103
const int SR_Note_A3   = 14; // Midi note 105
const int SR_Note_B3   = 13; // Midi note 107
const int SR_Note_C3   = 12; // Midi note 108
const int SR_Note_A3s  = 11; // Midi note 106
const int SR_Note_G2s  = 10; // Midi note 104
const int SR_Note_F2s  =  9; // Midi note 102
const int SR_Note_D2s  =  8; // Midi note 99
const int SR_Note_C2s  =  7; // Midi note 97
const int SR_Note_A2s  =  6; // Midi note 94
const int SR_Note_G1s  =  5; // Midi note 92
const int SR_Note_F1s  =  4; // Midi note 90
const int SR_Note_D1s  =  3; // Midi note 87
const int SR_Note_C1s  =  2; // Midi note 85
const int SR_Note_A1s  =  1; // Midi note 82
const int SR_Note_G0s  =  0; // Midi note 80

// Shift Register Output patterns
byte      Solenoid_ON  = B00000010;
byte      LED_ON       = B00000100;
byte      All_OFF      = B00000000;
byte      SR_Note_Data[SR_Length];
byte      SR_LED_Data[SR_Length]; 

// Variable for measuring time of part and calculating tempo
unsigned long sr_start_time= 0;
unsigned long sr_stop_time= 0;

unsigned long min_us= 240000000;
long int Beat_Time;

unsigned long part_start_time = 0;
unsigned long part_stop_time = 0;
float part_time = 0;
int part_tempo = 0;

// Voltage set to 6.5V use Time_2 between 1000 - 2000 for vol 0-100
const int Solenoid_On_Time_1 = 1000; // Pulse width in uSec Fixed
const int Solenoid_Off_Time = 1000; // Pulse width in uSec Fixed
int Solenoid_On_Time_2 = 1000; // Pulse width in uSec Variable

volatile unsigned int Timer1_Beat_Count = 0; // Used to set Timer 1 to correct tempo
volatile unsigned int Next_Beat = 0; // Flag to move to next beat, set by Timer 1 Interrupt


void setup() 
{
    // Populate array to store song information
    Tune_Info[0][0] =  Tune1Length;
    Tune_Info[0][1] =  Tune1NoteMax;
    Tune_Info[0][2] =  Tune1Tempo;
    Tune_Info[0][3] =  Tune1BeatDenom;

    // Configure I/O pins
    pinMode(Graphene_Sen_N, OUTPUT);
    pinMode(Conductor_Pin, INPUT_PULLUP);
    pinMode(SR_Latch_Pin, OUTPUT);
    pinMode(SR_Reset_Pin, OUTPUT);

    digitalWrite(Graphene_Sen_N, LOW);
    digitalWrite(Conductor_Pin, HIGH);
    digitalWrite(SR_Latch_Pin, LOW);
    digitalWrite(SR_Reset_Pin, LOW);
    
    // Begin USB Serial
    Serial.begin(9600);

    // Begin SD Card Serial
    SD.begin(8);

    // start the SPI library:
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); // 100kHz Shift Clock
    Clear_SR_Note_Data();
    Clear_SR_LED_Data();
    Reset_SR();   

    
}  

// --------------------------------------------------------------------------------------

void loop() 
{
    for(SongCount = 0; SongCount < NumSongs; SongCount++)
    {    
          Serial.print("Number of Notes: ");
          Serial.println(Tune_Info[SongCount][0]);
          Serial.print("Max Notes: ");
          Serial.println(Tune_Info[SongCount][1]);
          Serial.print("Tempo(Bpm): ");
          Serial.println(Tune_Info[SongCount][2]);
          Serial.print("Beat Denomination: ");
          Serial.println(Tune_Info[SongCount][3]);
          
          Serial.println("Ready to play, waiting for On Signal");
          
          On_Signal = 1;
          while (On_Signal > 0)
          {
              On_Signal = digitalRead(Conductor_Pin); // Wait for Graphene ON (LOW)
          }
          On_Signal = 1;
          
          part_start_time = micros();
          delay(start_delay);
          Serial.println("Song Start");

          Beat_Time = (1/(Tune_Info[SongCount][2]/60.0))/Tune_Info[SongCount][3]*1000000.0;
          Serial.print("Beat Time (s): ");
          Serial.println(Beat_Time/1000000.0);
          Serial.println();
          
          Next_Beat = 0;      // Reset Flag ADDED keep here as interupt can change it
          Start_Beat_Timer(); // Start Timer 1
          ArrayCount=0;
          
          for (BeatCount=0; BeatCount < Tune_Info[SongCount][0]; BeatCount++)
          {
                Serial.print("Beat: ");
                Serial.println(BeatCount);
                          
                for (NoteCount=0; NoteCount < Tune_Info[SongCount][1]; NoteCount++)
                { 
                      
                      Load_Timing = pgm_read_word(&Tune1[0][ArrayCount]);
                      Serial.print("Array Count: ");
                      Serial.println(ArrayCount);

                      if (Load_Timing == BeatCount)
                      {
                            Load_Note = pgm_read_word(&Tune1[1][ArrayCount]);
                            Serial.print("Note: ");
                            Serial.println(Load_Note);
  
                            if (Load_Note >= 149)
                            {
                                 // Volume 5
                                 Solenoid_On_Time_2 = 1300;
                                 Load_Note = Load_Note - 70;
                            }
                            else if (Load_Note >= 114)
                            {
                                  // Volume 4
                                  Solenoid_On_Time_2 = 1200;
                                  Load_Note = Load_Note - 35;
                            }
                            else if (Load_Note >= 79)
                            {
                                  // Volume 3
                                  Solenoid_On_Time_2 = 1100; // use to cal at lowest level , move to 1100 for playing
                                  Load_Note = Load_Note - 0;
                            }
                            else if (Load_Note >= 44)
                            {
                                  // Volume 2
                                  Solenoid_On_Time_2 = 1000;
                                  Load_Note = Load_Note + 35;
                            }
                            else 
                            {
                                  // Volume 1
                                  Solenoid_On_Time_2 = 900;
                                  Load_Note = Load_Note + 70;
                            }
                            
                            Load_Beat(Load_Note);
                            Play_Beat();
                            ArrayCount +=1;
                      }                                            
                }
                Serial.println();
                
                Flash_LED_ON();   

                Clear_SR_Note_Data();
                
                Next_Beat = 0;                
                while (Next_Beat < 1)
                {
                  // Waitfor Beat Timer 
                }
                Next_Beat = 0;
                Reset_SR();
                  
          }
          part_stop_time = micros();

          //-----------------
          // calculate tempo
          
          
          part_time = (part_stop_time-part_start_time)/1000000.0; 
          part_tempo = ((Tune_Info[SongCount][0]/Tune_Info[SongCount][3])/part_time)*60.0;

          // display results   
          Serial.print("Part Time (s): ");
          Serial.println(part_time);
          Serial.print("Part Tempo (bpm): ");
          Serial.println(part_tempo);
          
          Serial.print("Shift Register Load Time: ");  
          Serial.println(sr_stop_time-sr_start_time);
          
          Serial.println("------------------------------------------------");
          Serial.println();
          
          Serial.println("-----------------------------------------------------------------");

    } 
    
      
}
// --------------------------------------------------------------------------------------

// Load note functions
void Load_Beat(unsigned char Note) 
{
// Load note and LED arrays ready for shift register
    switch (Note) 
    {
        case 00:
          // No Note
          break;
        case 79:
          Solenoid_On_Time_2 = Solenoid_On_Time_2 + 300; // not vol calibration
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
          SR_Note_Data[SR_Note_B3] = Solenoid_ON;
          SR_LED_Data[SR_Note_B3] = LED_ON;
          break;
        case 108:
          SR_Note_Data[SR_Note_C3] = Solenoid_ON;
          SR_LED_Data[SR_Note_C3] = LED_ON;
          break;
        default: 
          // if nothing else matches, do the default
          // default is optional
        break;
      }
}


// Shift Register Functions
void Clear_SR_Note_Data() 
{
    for ( int i=0; i < SR_Length; i++)
    {
        SR_Note_Data[i] = All_OFF;                  // clear all bits
    }
}
  
void Clear_SR_LED_Data() 
{
    for ( int i=0; i < SR_Length; i++)
    {
        SR_LED_Data[i] = All_OFF;                  // clear all bits
    }
}

void Flash_LED_ON() 
{
    digitalWrite(SR_Reset_Pin, HIGH);
    Load_LED_SR();
    Clear_SR_LED_Data();
}


void Load_LED_SR() 
{
    digitalWrite(SR_Latch_Pin, LOW);
    for ( int i=0; i < SR_Length; i++)
    {
        SPI.transfer(SR_LED_Data[i]);                  // clear all bits
    }
    Set_SR();
}

void Play_Beat() 
{
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
  
void Load_Notes_SR() 
{
    sr_start_time=micros();
    digitalWrite(SR_Latch_Pin, LOW);
    for ( int i=0; i < SR_Length; i++)
    {
      SPI.transfer(SR_Note_Data[i]);           
    }
    Set_SR();
    sr_stop_time=micros();
}


// Functions for interrupt timer

void Start_Beat_Timer() 
{
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
  
