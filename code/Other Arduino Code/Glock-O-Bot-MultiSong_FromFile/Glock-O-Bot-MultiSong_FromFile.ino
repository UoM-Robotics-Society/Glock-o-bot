#include <SPI.h>
#include <SD.h>


// Information about songs for loading
const int MaxNumSongs = 2;  // Number of songs to be loaded
const int NumSongs = 1;     // Number of songs to be loaded
//String SongNames[MaxNumSongs] = {"Harry Potter", "Adams Family"};  // Names and order of the songs to be loaded
//String SongNames[MaxNumSongs] = {"HPotter", "AdFam"};
//String SongNames[MaxNumSongs] = {"AdFam", "HPotter"};
String SongNames[MaxNumSongs] = {"Test1", "Test2"};


// Create program variables
int Settings[4];  // Settings for each song, [0]-Number of notes in song, [1]-Tempo(bpm), [2]-Beat denomination, [3]-Max notes played at once
int NumNotes;     // Number of Notes in peice
int Tempo;        // Tempo of the peice in bpm
int BeatDenom;    // Smallest denomination of a beat to be played, i.e. 1 for whole beat, 4 for ...
int NoteMax;      // Max number of notes played at the same time
int On_Signal;    // Used to track conductor
const int start_delay = 0;
unsigned int Load_Note;

int NoteCount;
int BeatCount;


// Define I/O pins
const int Conductor_Pin = 5;  // Pin number for connecting conductor or button
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

// Variables for loading files
File LoadFile;
String FileName;
int count;

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

// --------------------------------------------------------------------------------------

void setup() 
{
     // Configure I/O pins
    pinMode(Conductor_Pin, INPUT_PULLUP);
    pinMode(SR_Latch_Pin, OUTPUT);
    pinMode(SR_Reset_Pin, OUTPUT);
    
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
    // Load files for each song
    for(int i = 0; i < NumSongs; i++)
    {
          Serial.print("Song Name: ");
          Serial.println(SongNames[i]);
          // Load Settings File
          FileName = SongNames[i] + "S.txt";  
          Serial.print("File Name: ");
          Serial.println(FileName);
          
          LoadFile = SD.open(FileName,FILE_READ);  
          if (LoadFile) 
          {
              Serial.println("Settings File Loaded");
              while (LoadFile.available()) 
              {
                  String line = LoadFile.readStringUntil('\n');
                  Settings[count] = line.toInt();
                  count+=1;
              }
              count = 0;
              LoadFile.close();

              Serial.print("Number of Notes: ");
              Serial.println(Settings[0]);
              Serial.print("Tempo(Bpm): ");
              Serial.println(Settings[1]);
              Serial.print("Beat Denomination: ");
              Serial.println(Settings[2]);
              Serial.print("Max Notes: ");
              Serial.println(Settings[3]);
          }
          else
          {
               Serial.println("Settings File Not Loaded");
          }
 
          NumNotes = Settings[0];
          Tempo = Settings[1];
          BeatDenom = Settings[2];
          NoteMax = Settings[3];
          //NoteMax = 1;

          Beat_Time = (1/(Tempo/60.0))/BeatDenom*1000000.0;
          Serial.print("Beat Time (s): ");
          Serial.println(Beat_Time/1000000.0);
          Serial.println();
    
          // Load Music File
          int Notes[NumNotes];  // Create array for storing the notes to be played
          
          FileName = SongNames[i] + "M.txt";
          //FileName = "HPotterM.txt";
          Serial.print("File Name: ");
          Serial.println(FileName);
          
          LoadFile = SD.open(FileName,FILE_READ);  
          if (LoadFile) 
          {
              Serial.println("Music File Loaded");
              while (LoadFile.available()) 
              {
                  String line = LoadFile.readStringUntil('\n');
                  Notes[count] = line.toInt();
                  count+=1;
              }
              count = 0;
              LoadFile.close();

              Serial.println("Notes: ");
              for (int j=0; j < NumNotes; j++) 
              {
                  Serial.print(j);
                  Serial.print(":");
                  Serial.print(Notes[j]);
                  Serial.println(", ");
              }
          Serial.println();
          }
          else
          {
               Serial.println("Music File Not Loaded");
          }
        
          
          // Play Music     
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

          Next_Beat = 0;      // Reset Flag ADDED keep here as interupt can change it
          Start_Beat_Timer(); // Start Timer 1

          for (BeatCount=0; BeatCount < NumNotes; BeatCount++)
          {
                Serial.print("Beat: ");
                Serial.println(BeatCount);
                          
                for (NoteCount=0; NoteCount < NoteMax; NoteCount++)
                { 
                      Load_Note = Notes[BeatCount];
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
                  
          }
          part_stop_time = micros();

          //-----------------
          // calculate tempo
          
          
          part_time = (part_stop_time-part_start_time)/1000000.0; 
          part_tempo = ((NumNotes/BeatDenom)/part_time)*60.0;

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
