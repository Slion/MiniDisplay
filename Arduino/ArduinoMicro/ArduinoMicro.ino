/*
   Noritake GU256x64D-3900B
   Character writting test
   By Stéphane Lenclud

!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!!!! 
Using serial port is handy for debug.
However is causes pixel noise issues.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/



#include "HID-Project.h"


const byte KRdyOffset = 7;
const byte KWrOffset = 6;
const byte KWriteDelayInUs = 1;


// Buffer to hold RawHID data.
// If host tries to send more data than this,
// it will respond with an error.
// If the data is not read until the host sends the next data
// it will also respond with an error and the data will be lost.
uint8_t rawhidData[255];


void delayProtocolWrite()
{
  //delayMicroseconds(KWriteDelayInUs);
  //__asm__("nop\n\t");
}

/*
   Send a single byte to our VFD module
*/
void sendByte(byte aByte)
{
  //int i=0;
  while (isDisplayBusy())
  {
    delayProtocolWrite();
    //Serial.println("Busy!");
    //Serial.println(i);
    //i++;
  }
  // Write data
  PORTB = aByte;
  // Bring /WR down to tell our display data is ready
  PORTC &= B10111111;
  // Make sure we wait long enough for our display to kick in
  // This very is important otherwise some characters will be lost
  // 108us was not long enough, characters would get lost.
  // 109us seems very reliable
  //delayMicroseconds(109);
  delayProtocolWrite();
  // Set /WR high up again
  PORTC |= 1 << KWrOffset;
}

/*

*/
bool isDisplayBusy()
{
  return (PINC & (1 << KRdyOffset)) == 0;
}

/*
   DMA clear screen
*/
void dmaFillScreen(byte aData)
{
  sendByte(0x02); // STX header
  sendByte(0x44); // Header 2
  sendByte(0xFF); // Display address: broadcast
  sendByte(0x46); // Command: Bit image write
  sendByte(0x00); // Write address lower byte
  sendByte(0x00); // Write address higher byte
  sendByte(0x00); // Data size lower byte
  sendByte(0x08); // Data size higher byte
  int i = 2048; // Size of one screen memory: 256x64 / 8
  while (i != 0)
  {
    sendByte(aData); // Data
    --i;
  }
}

/*
   Clear VFD module screen
*/
void clearScreen()
{
  //sendByte(0x0C);
  dmaFillScreen(0x00);
}


void setup()
{
  // Open serial port
  Serial.begin(115200);
  // Set the RawHID OUT report array.
  // Feature reports are also (parallel) possible, see the other example for this.
  RawHID.begin(rawhidData, sizeof(rawhidData));

  // Set port B pins as output
  DDRB = B11111111;
  // Set /WR pins as output
  DDRC = DDRC | B01000000;
  // Set RDY as input
  DDRC = DDRC & B01111111;
  // Clear our screen and wait a bit for it to be noticeable as we come online
  dmaFillScreen(0xFF);
  delay(1000);
  clearScreen(); //Twice in case we come here after interrupting an earlier write
  //clearScreen();
  delay(1000);
}

bool isIdle=true;

void loop()
{
  // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    isIdle=false;
    //Serial.print("Report ");
    //Serial.print(bytesAvailable);
    //Serial.println(" bytes.");
    // First get our byte count
    bytesAvailable--;
    int byteCount = RawHID.read();
    //byteCount--;
    //Serial.print("Count ");
    //Serial.print(byteCount);
    //Serial.println(" bytes.");    
    // Send data to display
    while (bytesAvailable--)
    {
      int b = RawHID.read();
      if (byteCount--)
      {
        // Only send that byte if within our count
        sendByte(b);
      }
      //Serial.print(bytesAvailable);
      //Serial.print(":");
      //Serial.println(b);
    }
  }
  else if (!isIdle)
  {
    // Going idle then 
    isIdle = true;
    //Serial.println("-------------------------");
  }
}

