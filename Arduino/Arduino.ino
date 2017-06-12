/*
Noritake GU256x64D-3900B
DMA Control Mode.
By Stéphane Lenclud

!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!!!!
Using serial port is handy for debug.
However is causes pixel noise issues on Adruino Micro.
This is not an issue on Teensy 3.2.
Therefore Teensy 3.2 is our recommanded board.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/


/*
Define to target Teensy 3.2.
Otherwise we assume Adruino Micro.
*/
#define TEENSY_3_2

/*
Use this to do serial port debug
*/
//#define SLOG

#ifdef TEENSY_3_2
const int KPinBusy = 0;
const int KPinWrite = 1;
const int KPortDToPin[] = { 2,14,7,8,6,20,21,5 };
#else
#include "HID-Project.h"
const byte KRdyOffset = 7;
const byte KWrOffset = 6;
#endif 

const byte KDelayBusyInUs = 1;
const byte KDelayWriteInUs = 5;
const ushort KScreenSizeInBytes = 256 * 64 / 8; //2048
// We have two screens in RAM
const ushort KDisplayRamSizeInBytes = KScreenSizeInBytes; //4096
const ushort KFrameBufferAlpha = 0x0000;
const ushort KFrameBufferBeta = KScreenSizeInBytes;


// Buffer to hold RawHID data.
// If host tries to send more data than this,
// it will respond with an error.
// If the data is not read until the host sends the next data
// it will also respond with an error and the data will be lost.
uint8_t rawhidData[64];
//
bool isIdle = true;


void delayProtocolBusy()
{
    delayMicroseconds(KDelayBusyInUs);
    /*
    for (int i = 0; i++; i < 100)
    {
    __asm__("nop\n\t");
    }
    */
}



void delayProtocolWrite()
{
    delayMicroseconds(KDelayWriteInUs);
    
    /*
    for (int i = 0; i++; i < 100000)
    {
        __asm__("nop\n\t");
    }
    */
    
}

int busyCount = 0;

/*
Send a single byte to our VFD module
*/
void sendByte(byte aByte)
{    
    while (isDisplayBusy())
    {
        delayProtocolBusy();
#ifdef SLOG
        Serial.println("Busy!");
        Serial.println(busyCount);
        busyCount++;
#endif
    }

#ifdef TEENSY_3_2
    // Write data
    GPIOD_PDOR = aByte;
    // Bring /WR down to tell our display data is ready
    digitalWriteFast(KPinWrite, LOW);
    // Make sure we wait long enough for our display to kick in
    delayProtocolWrite();
    // Set /WR high up again
    digitalWriteFast(KPinWrite, HIGH);
#else
    // Write data
    PORTB = aByte;
    //PORTB = 0;
    // Bring /WR down to tell our display data is ready
    PORTC &= B10111111;
    // Make sure we wait long enough for our display to kick in
    delayProtocolWrite();
    // Set /WR high up again
    PORTC |= 1 << KWrOffset;
#endif

}

/*

*/
bool isDisplayBusy()
{
#ifdef TEENSY_3_2
    return digitalReadFast(KPinBusy) == 0;
#else
    return (PINC & (1 << KRdyOffset)) == 0;
#endif
}


/*
Change display RAM address of the current frame.
Basically used to implement swap buffer.
*/
void dmaSpecifyDisplayStartAddress(ushort aAddress)
{
    sendByte(0x02); // STX header
    sendByte(0x44); // Header 2
    sendByte(0xFF); // Display address: broadcast
    sendByte(0x53); // Command: Specify Display Start Address
    sendByte(aAddress & 0x00FF); // Write address lower byte
    sendByte(aAddress >>8 ); // Write address higher byte
}

/*
*/
void dmaSpecifyDisplaySynchronisation()
{
    sendByte(0x02); // STX header
    sendByte(0x44); // Header 2
    sendByte(0xFF); // Display address: broadcast
    sendByte(0x57); // Command: Specify Display Synchronisation
    sendByte(0x01); // Write address lower byte
}


/*
DMA clear screen
*/
void dmaBitImageWrite(ushort aAddress, byte aData, ushort aSize)
{
    sendByte(0x02); // STX header
    sendByte(0x44); // Header 2
    sendByte(0xFF); // Display address: broadcast
    sendByte(0x46); // Command: Bit image write
    sendByte(aAddress & 0x00FF); // Write address lower byte
    sendByte(aAddress >> 8); // Write address higher byte
    sendByte(0x00); // Data size lower byte
    sendByte(0x08); // Data size higher byte
    int i = aSize; // Size of one screen memory: 256x64 / 8
    while (i != 0)
    {
        sendByte(aData); // Data
        --i;
    }
}

/*
Clear VFD module screen
*/
void clearDisplayRam()
{
    //sendByte(0x0C);
    dmaBitImageWrite(KFrameBufferAlpha,0x00,KDisplayRamSizeInBytes);
}

/*
Entry point
*/
void setup()
{
    // Open serial port
#ifdef SLOG
    Serial.begin(9600);
    Serial.println("Connected");
#endif
    // Set the RawHID OUT report array.
    // Feature reports are also (parallel) possible, see the other example for this.


#ifdef TEENSY_3_2
    for (int i = 0; i<sizeof(KPortDToPin); i++)
    {
        pinMode(KPortDToPin[i], OUTPUT);
    }

    pinMode(KPinWrite, OUTPUT);
    pinMode(KPinBusy, INPUT);
#else
    RawHID.begin(rawhidData, sizeof(rawhidData));
    // Set port B pins as output
    DDRB = B11111111;
    // Set /WR pins as output
    DDRC = DDRC | B01000000;
    // Set RDY as input
    DDRC = DDRC & B01111111;
#endif

    // Clear our screen and wait a bit for it to be noticeable as we come online
    clearDisplayRam();
    dmaSpecifyDisplayStartAddress(KFrameBufferAlpha);
    delay(1000);
    dmaBitImageWrite(KFrameBufferBeta, 0xFF, KScreenSizeInBytes);
    dmaSpecifyDisplayStartAddress(KFrameBufferBeta);
    delay(1000);
    dmaBitImageWrite(KFrameBufferAlpha, 0x00, KScreenSizeInBytes);
    dmaSpecifyDisplayStartAddress(KFrameBufferAlpha);

}

/*
*/
void loop()
{

#ifdef TEENSY_3_2
    int n;
    n = RawHID.recv(rawhidData, 0); // 0 timeout = do not wait
    if (n > 0)
    {
        isIdle = false;
        int byteCount = rawhidData[0];
        //Serial.print("byteCount: ");
        //Serial.println(byteCount);
        int i = 0;
        while (i<sizeof(rawhidData) && i++<byteCount)
        {
            // Only send that byte if within our count
            sendByte(rawhidData[i]);

            //Serial.print("Data: ");
            //Serial.println(rawhidData[i],HEX);
        }

    }
#else
    // Check if there is new data from the RawHID device
    auto bytesAvailable = RawHID.available();
    if (bytesAvailable)
    {
        isIdle = false;
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
#endif
    else if (!isIdle)
    {
        // Going idle then 
        isIdle = true;
        //Serial.println("-------------------------");
    }
}

