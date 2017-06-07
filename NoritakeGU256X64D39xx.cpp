//
// Copyright (C) 2014-2015 St�phane Lenclud.
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
*/
int GU256X64D39XX::Open()
{
    int success = HidDevice::Open(KArduinoVendorId, KArduinoMicroProductId, NULL);
    if (success)
    {
        //Allocate both frames
        /*
        delete iFrameAlpha;
        iFrameAlpha = NULL;
        iFrameAlpha = new BitArrayHigh(KGP12xFrameBufferPixelCount);
        //
        delete iFrameBeta;
        iFrameBeta = NULL;
        iFrameBeta = new BitArrayHigh(KGP12xFrameBufferPixelCount);
        //
        delete iFrameGamma;
        iFrameGamma = NULL;
        iFrameGamma = new BitArrayHigh(KGP12xFrameBufferPixelCount);
        //
        iFrameNext = iFrameAlpha;
        iFrameCurrent = iFrameBeta;
        iFramePrevious = iFrameGamma;
        */


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

}

void GU256X64D39XX::SetAllPixels(unsigned char aPattern)
{

}

void GU256X64D39XX::SetBrightness(int aBrightness)
{

}

void GU256X64D39XX::Clear()
{
    CmdBitImageWrite(0x0000, 0x0800, 0x00);
}

/*
*/
void GU256X64D39XX::Fill()
{
    CmdBitImageWrite(0x0000, 0x0800, 0xFF);
}

/**
 * Fill in our display memory with the given 8 vertical pixels value from the given RAM address for the given size. 
 *
 * @param aRamAddress Display RAM address at which to start copying our pixel data.
 * @param aSize Size in bytes of our pixel data.
 * @param aValue Byte value corresponding to 8 pixels on a vertical line.
 */
void GU256X64D39XX::CmdBitImageWrite(unsigned short aRamAddress, unsigned short aSize, unsigned char aValue)
{
    ArduinoReport report;

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
    
    int sizeWritten = MIN(aSize, report.Size() - KReportCmdHeaderSize);
    memset(report.Buffer() + KReportCmdHeaderSize, aValue, sizeWritten);
    while (Write(report) != report.Size() && retry-->0);
    if (retry < 0)
    {
        // Abort since we can't sent our command header
        return;
    }
    retry = KMaxRetry; // Rearm

    int remainingSize = aSize; // -sizeWritten;
    //We need to keep on sending our pixel data until we are done
    while (report[1] == KMaxDataSizePerReport)
    {
        report.Reset();
        remainingSize -= sizeWritten;
        report[0] = 0x00; //Report ID
        report[1] = MIN(remainingSize, KMaxDataSizePerReport); //Report length, should be 64 or the remaining size
        sizeWritten = report[1];
        memset(report.Buffer() + KReportMinHeaderSize, aValue, sizeWritten);
        while (Write(report) != report.Size() && retry-->0);
        retry = KMaxRetry; // Rearm
    }
}

void GU256X64D39XX::SwapBuffers()
{

}


