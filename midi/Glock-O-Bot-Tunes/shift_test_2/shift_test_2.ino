#include <SPI.h>
int ShiftLength = 16;
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
int count = 0;
int X = 0;
int A = 1;
int B = 2;
int C = 4;
int D = 8;
int E = 16;
int F = 32;
int G = 64;
int x = 128;
int a = 256;
int b = 512;
int c = 1024;
int d = 2048;
int e = 4096;
int f = 8192;
int g = 16384;
int ShiftValue[16] = {X, A, B, C, D, E, F, G, x, a, b, c, d, e, f, g}; 



void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  pinMode(latchPin, OUTPUT);
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));

}

void loop() {
  while(true){
    // initialise varibles:
    digitalWrite(latchPin,0);
    count=0;

    for (count = 0; count < 15; count++)  {
      SPI.transfer(ShiftValue,ShiftLength);
      digitalWrite(latchPin,1);
      delay(1000);
      digitalWrite(latchPin,0);
    }
  }
}
