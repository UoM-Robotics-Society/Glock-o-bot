#include <SPI.h>

// Information about songs for loading
const int MaxNumSongs = 4;  // Number of songs to be loaded
const int NumSongs = 5;     // Number of songs to be loaded

const int PlaySongs[NumSongs] = {1,2,3,4,5};  //Which Songs to play and in what order

const int Tune1Length = 30;
const int Tune1NoteMax = 1;
const int Tune1Tempo = 200;
const int Tune1BeatDenom = 4;

const int Tune2Length = 184;
const int Tune2NoteMax = 1;
const int Tune2Tempo = 95;
const int Tune2BeatDenom = 4;

const int Tune3Length = 794;
const int Tune3NoteMax = 3;
const int Tune3Tempo = 126;
const int Tune3BeatDenom = 8;

const int Tune4Length = 349;
const int Tune4NoteMax = 4;
const int Tune4Tempo = 96;
const int Tune4BeatDenom = 4;

const int Tune5Length = 185;
const int Tune5NoteMax = 1;
const int Tune5Tempo = 110;
const int Tune5BeatDenom = 4;

  // Test Notes
const unsigned int Tune1[2][Tune1Length] PROGMEM = {
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29},
  {79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108}};
  
  // Harry Potter
const unsigned int Tune2[2][Tune2Length] PROGMEM = {
  {1,3,6,7,9,13,15,21,27,30,31,33,37,39,49,51,54,55,57,61,63,67,69,73,75,78,79,81,85,87,97,99,103,105,109,111,115,117,121,123,126,127,129,133,135,145,147,151,153,157,159,163,165,169,171,174,175,177,181,183},
  {83,88,91,90,88,95,93,90,88,91,90,86,89,83,83,88,91,90,88,95,98,97,96,92,96,95,94,90,91,88,91,95,91,95,91,96,95,94,90,91,95,94,82,83,95,91,95,91,95,91,98,97,96,92,96,95,94,90,91,88}};

  // Addams Family
const unsigned int Tune3[2][Tune3Length] PROGMEM = {
  {1,4,4,6,6,9,9,17,25,33,33,36,36,38,38,41,41,49,57,65,65,68,68,70,70,73,73,81,81,84,84,86,86,89,89,97,100,100,102,102,105,105,113,121,129,132,132,134,134,137,137,145,153,161,161,164,164,166,166,169,169,177,185,193,193,196,196,198,198,201,201,209,209,212,212,214,214,217,217,225,228,228,230,230,233,233,241,249,252,252,257,257,261,261,262,263,265,271,273,279,279,281,281,287,295,295,297,297,303,305,311,311,313,313,319,327,327,329,335,337,343,343,345,345,351,359,359,361,361,367,369,375,375,377,391,393,399,401,407,407,409,409,415,423,423,425,425,431,433,439,439,441,441,447,455,455,457,463,465,471,471,473,473,479,487,487,489,489,495,497,503,503,505,513,516,516,518,518,521,521,529,537,545,545,548,548,550,550,553,553,561,569,577,577,580,580,582,582,585,585,593,593,596,596,598,598,601,601,609,612,612,614,614,617,617,625,633,636,636,641,641,645,645,646,647,649,655,657,663,663,665,665,671,679,679,681,681,687,689,695,695,697,697,703,711,711,713,719,721,727,727,729,729,735,743,743,745,745,753,761,761,769,777,785,793},
  {89,79,91,81,93,82,94,91,91,79,91,81,93,83,95,84,96,91,91,79,91,81,93,83,95,84,96,79,91,81,93,83,95,84,96,89,79,91,81,93,82,94,91,91,89,79,91,81,93,82,94,91,91,79,91,81,93,83,95,84,96,91,91,79,91,81,93,83,95,84,96,79,91,81,93,83,95,84,96,89,79,91,81,93,82,94,91,91,89,101,89,101,89,101,89,89,94,98,94,91,95,87,96,96,92,88,93,89,96,93,89,93,86,94,94,89,93,94,98,94,91,95,87,96,96,94,88,93,89,89,91,93,96,94,89,94,98,94,91,95,87,96,96,92,88,93,89,96,93,89,93,86,94,94,89,93,94,98,94,91,95,87,96,96,94,88,93,89,89,91,93,96,94,89,79,91,81,93,82,94,91,91,79,91,81,93,83,95,84,96,91,91,79,91,81,93,83,95,84,96,79,91,81,93,83,95,84,96,89,79,91,81,93,82,94,91,91,89,101,89,101,89,101,89,89,94,98,94,91,95,87,96,96,92,88,93,89,96,93,89,93,86,94,94,89,93,94,98,94,91,95,87,96,96,94,88,93,89,89,91,89,93,94,91,91}};

  // Shape Of You
const unsigned int Tune4[2][Tune4Length] PROGMEM = {
  {0,0,0,3,3,3,6,6,6,8,8,8,11,11,11,14,14,14,16,16,16,19,19,19,22,22,22,24,24,24,27,27,27,30,30,30,32,32,32,35,35,35,38,38,38,40,40,40,43,43,43,46,46,46,48,48,48,51,51,51,54,54,54,56,56,56,56,59,59,59,62,62,62,64,64,64,67,67,67,70,70,70,72,72,72,75,75,75,78,78,78,80,80,80,83,83,83,86,86,86,88,88,88,91,91,91,94,94,94,96,96,96,99,99,99,102,102,102,104,104,104,107,107,107,110,110,110,112,112,112,115,115,115,118,118,118,120,120,120,120,123,123,123,126,126,126,128,128,128,131,131,131,134,134,134,136,136,136,139,139,139,142,142,142,144,144,144,147,147,147,150,150,150,152,152,152,155,155,155,158,158,158,160,160,160,163,163,163,166,166,166,168,168,168,171,171,171,174,174,174,176,176,176,179,179,179,182,182,182,184,184,184,184,187,187,187,190,190,190,192,192,192,195,195,195,198,198,198,200,200,200,203,203,203,206,206,206,208,208,208,211,211,211,214,214,214,216,216,216,219,219,219,222,222,222,224,224,224,227,227,227,230,230,230,232,232,232,235,235,235,238,238,238,240,240,240,243,243,243,246,246,246,248,248,248,248,251,251,251,254,254,254,256,256,256,259,259,259,262,262,262,264,264,264,267,267,267,270,270,270,272,272,272,275,275,275,278,278,278,280,280,280,283,283,283,286,286,286,288,288,288,291,291,291,294,294,294,296,296,296,299,299,299,302,302,302,304,304,304},
  {85,92,97,100,92,85,85,92,97,97,93,78,78,93,100,97,93,78,81,93,97,100,93,81,81,93,97,99,90,83,83,90,97,95,90,83,85,92,97,100,92,85,85,92,97,97,93,78,78,93,100,97,93,78,81,93,97,100,93,81,81,93,97,100,95,90,83,99,90,83,83,90,95,97,92,85,85,92,100,97,92,85,78,93,97,100,93,78,78,93,97,97,93,81,81,93,100,97,93,81,83,90,100,99,90,83,83,90,95,97,92,85,85,92,100,85,92,97,78,97,93,78,93,100,78,97,93,81,93,97,100,93,81,81,93,97,100,95,90,83,99,90,83,83,90,95,85,92,97,100,92,85,85,92,97,78,97,93,78,93,100,78,93,97,81,93,97,100,93,81,81,93,97,83,90,99,83,90,97,83,95,90,85,92,97,100,92,85,85,92,97,97,93,78,78,93,100,97,93,78,81,93,97,100,93,81,81,93,97,100,95,90,83,99,90,83,83,90,95,97,92,85,85,92,100,97,92,85,78,93,97,100,93,78,78,93,97,97,93,81,81,93,100,97,93,81,83,90,100,99,90,83,83,90,97,97,92,85,85,92,100,97,92,85,78,93,97,100,93,78,78,93,97,97,93,81,81,93,100,97,93,81,100,95,90,83,83,90,99,95,90,83,85,92,97,100,92,85,85,92,97,97,93,78,78,93,100,97,93,78,81,93,97,100,93,81,81,93,97,100,90,83,83,90,99,95,90,83,85,92,97,100,92,85,85,92,97,97,93,78,78,93,100,97,93,78,81,93,97}};

  // Hall of the Mountain King - Medium Speed Part
const unsigned int Tune5[2][Tune5Length] PROGMEM = {
  {0,2,4,6,8,10,12,16,18,20,24,26,28,32,34,36,38,40,42,44,46,48,50,52,54,56,64,66,68,70,72,74,76,80,82,84,88,90,92,96,98,100,102,104,106,108,112,114,116,120,128,130,132,134,136,138,140,144,146,148,152,154,156,160,162,164,166,168,170,172,174,176,178,180,182,184},
  {83,85,86,88,90,86,90,89,85,89,88,84,88,83,85,86,88,90,86,90,95,93,90,86,90,93,90,92,94,95,97,94,97,98,94,98,97,94,97,90,92,94,95,97,94,97,98,94,98,97,83,85,86,88,90,86,90,89,85,89,88,84,88,83,85,86,88,90,86,90,95,93,90,86,90,93}};

const int start_delay = 0;

// Create program variables
int On_Signal;    // Used to track conductor
unsigned int Load_Note;
unsigned int Load_Timing;

int ArrayCount;
int NoteCount;
int BeatCount;
int SongCount;
int SongNum; // Number of the Song to Play

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
    Tune_Info[1][0] =  Tune2Length;
    Tune_Info[1][1] =  Tune2NoteMax;
    Tune_Info[1][2] =  Tune2Tempo;
    Tune_Info[1][3] =  Tune2BeatDenom;
    Tune_Info[2][0] =  Tune3Length;
    Tune_Info[2][1] =  Tune3NoteMax;
    Tune_Info[2][2] =  Tune3Tempo;
    Tune_Info[2][3] =  Tune3BeatDenom;
    Tune_Info[3][0] =  Tune4Length;
    Tune_Info[3][1] =  Tune4NoteMax;
    Tune_Info[3][2] =  Tune4Tempo;
    Tune_Info[3][3] =  Tune4BeatDenom;
    Tune_Info[4][0] =  Tune5Length;
    Tune_Info[4][1] =  Tune5NoteMax;
    Tune_Info[4][2] =  Tune5Tempo;
    Tune_Info[4][3] =  Tune5BeatDenom;

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
          SongNum = PlaySongs[SongCount];
          //SongNum = SongCount;
          
          Serial.print("Song Count: ");
          Serial.println(SongNum);
          Serial.print("Number of Notes: ");
          Serial.println(Tune_Info[SongNum][0]);
          Serial.print("Max Notes: ");
          Serial.println(Tune_Info[SongNum][1]);
          Serial.print("Tempo(Bpm): ");
          Serial.println(Tune_Info[SongNum][2]);
          Serial.print("Beat Denomination: ");
          Serial.println(Tune_Info[SongNum][3]);
          
          Serial.println("Ready to play, waiting for On Signal");
          
          On_Signal = 1;
          while (On_Signal > 0)
          {
              On_Signal = digitalRead(Conductor_Pin); // Wait for Button ON (LOW)
          }
          On_Signal = 1;
          delayMicroseconds(1000);    // Delay and second pin read for debouncing the button
          while (On_Signal > 0)   
          {
              On_Signal = digitalRead(Conductor_Pin); // Wait for Button ON (LOW)
          }
          
          //On_Signal = 1;
          
          part_start_time = micros();
          delay(start_delay);
          Serial.println("Song Start");

          Beat_Time = (1/(Tune_Info[SongNum][2]/60.0))/Tune_Info[SongNum][3]*1000000.0;
          Serial.print("Beat Time (s): ");
          Serial.println(Beat_Time/1000000.0);

          Next_Beat = 0;      // Reset Flag ADDED keep here as interupt can change it
          Start_Beat_Timer(); // Start Timer 1
          ArrayCount = 0;

          for (BeatCount=0; BeatCount < Tune_Info[SongNum][0]; BeatCount++)
          {
                Serial.print("Beat: ");
                Serial.println(BeatCount);
                          
                for (NoteCount=0; NoteCount < Tune_Info[SongNum][1]; NoteCount++)
                { 
                      Serial.println(ArrayCount);
                      switch (SongNum)
                      {
                          case 0:                   
                            Load_Timing = pgm_read_byte(&Tune1[0][ArrayCount]);
                            break;
                          case 1:
                            Load_Timing = pgm_read_byte(&Tune2[0][ArrayCount]);
                            break;
                          case 2:
                            Load_Timing = pgm_read_word(&Tune3[0][ArrayCount]);
                            break;
                          case 3:
                            Load_Timing = pgm_read_byte(&Tune4[0][ArrayCount]);
                            break;
                          case 4:
                            Load_Timing = pgm_read_byte(&Tune5[0][ArrayCount]);
                            break;
                      }

                      if (Load_Timing == BeatCount)
                      {
                            switch (SongNum)
                            {
                                case 0:                   
                                  Load_Note = pgm_read_byte(&Tune1[1][ArrayCount]);
                                  break;
                                case 1:
                                  Load_Note = pgm_read_byte(&Tune2[1][ArrayCount]);
                                  break;
                                case 2:
                                  Load_Note = pgm_read_byte(&Tune3[1][ArrayCount]);
                                  break;
                                case 3:
                                  Load_Note = pgm_read_byte(&Tune4[1][ArrayCount]);
                                  break;
                                case 4:
                                  Load_Note = pgm_read_byte(&Tune5[1][ArrayCount]);
                                  break;
                            }
                    

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
                Flash_LED_ON();   

                Clear_SR_Note_Data();
                
                Next_Beat = 0;                
                while (Next_Beat < 1)
                {
                  // Waitfor Beat Timer 
                }
                Next_Beat = 0;
                Reset_SR();

                On_Signal = digitalRead(Conductor_Pin);
                //Serial.print("On_Signal: ");
                //Serial.println(On_Signal);
                if (On_Signal > 0)
                {
                    break;
                }
                  
          }

          while (On_Signal < 1)
          {
              On_Signal = digitalRead(Conductor_Pin); // Wait for Button OFF (HIGH)
          }
          
          part_stop_time = micros();

          //-----------------
          // calculate tempo
          
          
          part_time = (part_stop_time-part_start_time)/1000000.0; 
          part_tempo = ((Tune_Info[SongNum][0]/Tune_Info[SongNum][3])/part_time)*60.0;

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
  
