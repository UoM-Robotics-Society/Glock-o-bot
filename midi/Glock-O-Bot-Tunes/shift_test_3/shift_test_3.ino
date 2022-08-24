#include <SPI.h>
int ShiftLength = 2;
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
int count = 0;

byte ShiftValue[2] = {}; 



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

    for (count = 0; count < 7; count++)  {
      in sr.
    }
      for (count = 0; count < 7; count++)  {
      SPI.transfer(ShiftValue[count]);
      digitalWrite(latchPin,1);
      delay(1000);
      digitalWrite(latchPin,0);
    }
  }
}
