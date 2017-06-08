//
// Copyright (C) 2014-2015 Stéphane Lenclud.
//
// This file is part of MiniDisplay.
//
// MiniDisplay is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MiniDisplay is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MiniDisplay.  If not, see <http://www.gnu.org/licenses/>.
//


#include "NoritakeGU256X64D39xx.h"

/**
Template for report data processor to avoid duplicating our core communication algorithm like we did for the Futaba displays.
*/
template <typename T>
class TReportDataProcessor
{
public:
    void Copy(unsigned char* aDestination, T aData, int aSize);
};

// Specialization for pattern fill
template<>
void TReportDataProcessor<unsigned char>::Copy(unsigned char* aDestination, unsigned char aData, int aSize)
{
    memset(aDestination, aData, aSize);
}

// Specialization for buffer copy
template<>
void TReportDataProcessor<unsigned char*&>::Copy(unsigned char* aDestination, unsigned char*& aData, int aSize)
{
    // Copy our data in place
    memcpy(aDestination, aData, aSize);
    // Advance our pointer
    aData += aSize;
}

GU256X64D39XX::GU256X64D39XX() :
    iScreenBufferAddress(KFrameRamAddressOne),
    iOffScreenBufferAddress(KFrameRamAddressTwo),
    iFrame(NULL)
{

}

GU256X64D39XX::~GU256X64D39XX()
{
    delete iFrame;
    iFrame = NULL;
}

/**
*/
int GU256X64D39XX::Open()
{
    int success = HidDevice::Open(KArduinoVendorId, KArduinoMicroProductId, NULL);
    if (success)
    {
        // Make sure our frame buffer addres is in sync
        CmdSpecifyDisplayStartAddress(iScreenBufferAddress);
        //Allocate frame
        delete iFrame;
        iFrame = NULL;
        iFrame = new BitArrayLow(KFrameBufferPixelCount);

        //To make sure it is synced properly
        //iNeedFullFrameUpdate = 0;
        //
        SetNonBlocking(1);
        //Since we can't get our display position we force it to our default
        //This makes sure frames are in sync from the start
        //Clever clients will have taken care of putting back frame (0,0) before closing
        ////SetDisplayPosition(iDisplayPositionX, iDisplayPositionY);
    }
    return success;
}


void GU256X64D39XX::SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel)
{
    bool on = (aPixel & 0x00FFFFFF) != 0x00000000;

    if (on)
    {
        iFrame->SetBit(aX*HeightInPixels() + aY);
    }
    else
    {
        iFrame->ClearBit(aX*HeightInPixels() + aY);
    }
}

void GU256X64D39XX::SetAllPixels(unsigned char aPattern)
{
    memset(iFrame->Ptr(), aPattern, iFrame->SizeInBytes());
}

void GU256X64D39XX::SetBrightness(int aBrightness)
{
    CmdBrightnessLevelSetting(aBrightness);
}

void GU256X64D39XX::Clear()
{
    iFrame->ClearAll();
}

/*
*/
void GU256X64D39XX::Fill()
{
    iFrame->SetAll();
}

/**
 * Fill in our display memory with the given 8 vertical pixels value from the given RAM address for the given size. 
 *
 * @param aRamAddress Display RAM address at which to start copying our pixel data.
 * @param aSize Size in bytes of our pixel data.
 * @param aValue Byte value corresponding to 8 pixels on a vertical line.
 * @return Zero on success. Positive number indicate not all data could be sent. Negative number means algorithm issue a too much data was sent.
 */
template<typename T>
int GU256X64D39XX::CmdBitImageWrite(unsigned short aRamAddress, unsigned short aSize, T aValue)
{
    ArduinoReport report;
    TReportDataProcessor<T> processor;

    const int KReportCmdHeaderSize = 10;
    const int KArdruinoCmdHeaderSize = 9;
    const int KNoritakeCmdHeaderSize = 8;
    const int KMaxDataSizePerReport = report.MaxSize() - KReportMinHeaderSize;
    const int KMinDataSizePerReport = report.MaxSize() - KReportCmdHeaderSize;

    const int KMaxRetry = 100;
    int retry = KMaxRetry;

    
    report[0] = 0x00; //Report ID
    report[1] = (aSize <= KMinDataSizePerReport ? aSize + KNoritakeCmdHeaderSize : KMaxDataSizePerReport); //Report length. -10 is for our header first 10 bytes. +8 is for our Futaba header size; // Arduino protocol
    report[2] = 0x02; // STX header
    report[3] = 0x44; // Header 2
    report[4] = 0xFF; // Display address: broadcast
    report[5] = 0x46; // Command: Bit image write
    report[6] = (unsigned char)aRamAddress; // Write address lower byte
    report[7] = aRamAddress>>8; // Write address higher byte
    report[8] = (unsigned char)aSize; // Data size lower byte
    report[9] = aSize>>8; // Data size higher byte
    
    int remainingSize = aSize;
    int sizeWritten = MIN(aSize, report.Size() - KReportCmdHeaderSize);    
    processor.Copy(report.Buffer() + KReportCmdHeaderSize, aValue, sizeWritten);
    while (Write(report) != report.Size() && retry-->0);
    if (retry < 0)
    {
        // Abort since we can't send our command header
        return remainingSize;
    }
    retry = KMaxRetry; // Rearm
    remainingSize -= sizeWritten;
    //We need to keep on sending our pixel data until we are done
    while (remainingSize>0)
    {
        report.Reset();
        report[0] = 0x00; //Report ID
        report[1] = MIN(remainingSize, KMaxDataSizePerReport); //Report length, should be 64 or the remaining size
        sizeWritten = report[1];
        processor.Copy(report.Buffer() + KReportMinHeaderSize, aValue, sizeWritten);
        while (Write(report) != report.Size() && retry-->0);
        if (retry < 0)
        {
            // Error, can't recover from here
            break;
        }
        retry = KMaxRetry; // Rearm
        remainingSize -= sizeWritten;
    }

    if (remainingSize != 0)
    {
        // Silence error for now
        return remainingSize;
    }

    // Success
    return 0;
}

/*
*/
int GU256X64D39XX::CmdSpecifyDisplayStartAddress(unsigned short aRamAddress)
{
    const int KMaxRetry = 100;
    int retry = KMaxRetry;
    ArduinoReport report;
    report[0] = 0x00; //Report ID
    report[1] = 0x06; //Report length excluding first two bytes.
    report[2] = 0x02; // STX header
    report[3] = 0x44; // Header 2
    report[4] = 0xFF; // Display address: broadcast
    report[5] = 0x53; // Command: Specifiy Display Start Address
    report[6] = (unsigned char)aRamAddress; // Write address lower byte
    report[7] = aRamAddress >> 8; // Write address higher byte
    while (Write(report) != report.Size() && retry-->0);
    if (retry < 0)
    {
        // Abort since we can't send our command header
        return report.Size();
    }
    return 0;
}

/*
*/
int GU256X64D39XX::CmdBrightnessLevelSetting(unsigned char aBrightness)
{
    const int KMaxRetry = 100;
    int retry = KMaxRetry;
    ArduinoReport report;
    report[0] = 0x00; //Report ID
    report[1] = 0x05; //Report length excluding first two bytes.
    report[2] = 0x02; // STX header
    report[3] = 0x44; // Header 2
    report[4] = 0xFF; // Display address: broadcast
    report[5] = 0x58; // Command: Brightness level setting
    report[6] = aBrightness; // Write address lower byte
    while (Write(report) != report.Size() && retry-->0);
    if (retry < 0)
    {
        // Abort since we can't send our command header
        return report.Size();
    }
    return 0;
}

/*
*/
void GU256X64D39XX::SwapBuffers()
{
    // Our template needs to pointer reference to be able advance it.
    // Therefore we take a local copy...
    unsigned char* ptr = iFrame->Ptr();
    // ...and pass its reference to our template function.
    // That requires explicitly specifying the template parameter.
    CmdBitImageWrite<unsigned char*&>(iOffScreenBufferAddress, iFrame->SizeInBytes(), ptr);
    if (!CmdSpecifyDisplayStartAddress(iOffScreenBufferAddress))
    {
        // Only register our swap if there was no error
        unsigned short screenAddress = iOffScreenBufferAddress;
        iOffScreenBufferAddress = iScreenBufferAddress;
        iScreenBufferAddress = screenAddress;
    }
    iFrame->ClearAll();
}


