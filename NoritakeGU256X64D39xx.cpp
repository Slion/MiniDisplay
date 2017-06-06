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
    ArduinoReport report; // TODO: massive object on the stack?
    report[0] = 0; //Report ID
    report[1] = 0x02; // STX header
    report[2] = 0x44; // Header 2
    report[3] = 0xFF; // Display address: broadcast
    report[4] = 0x46; // Command: Bit image write
    report[5] = 0x00; // Write address lower byte
    report[6] = 0x00; // Write address higher byte
    report[7] = 0x00; // Data size lower byte
    report[8] = 0x08; // Data size higher byte

    int aValue = 0x00;
    int aSize = 2048;
    int sizeWritten = MIN(aSize, report.Size() - 9);
    memset(report.Buffer() + 9, aValue, sizeWritten);
    Write(report);
    int remainingSize = aSize;
    remainingSize -= sizeWritten;
    // TODO: That's broken, fix it
    //We need to keep on sending our pixel data until we are done
    while (remainingSize>0)
    {
        report.Reset();
        report[0] = 0x00; //Report ID
        sizeWritten = 64;
        memset(report.Buffer() + 1, aValue, sizeWritten);
        int written = Write(report);
        while (written < 64)
        {
            written = Write(report);
        }
        remainingSize -= sizeWritten;
    }
}

void GU256X64D39XX::Fill()
{
    ArduinoReport report; // TODO: massive object on the stack?
    report[0] = 0; //Report ID
    report[1] = 0x02; // STX header
    report[2] = 0x44; // Header 2
    report[3] = 0xFF; // Display address: broadcast
    report[4] = 0x46; // Command: Bit image write
    report[5] = 0x00; // Write address lower byte
    report[6] = 0x00; // Write address higher byte
    report[7] = 0x00; // Data size lower byte
    report[8] = 0x08; // Data size higher byte

    int aValue = 0xFF;
    int aSize = 2048;
    int sizeWritten = MIN(aSize, report.Size() - 9);
    memset(report.Buffer() + 9, aValue, sizeWritten);
    Write(report);
    int remainingSize = aSize;
    remainingSize -= sizeWritten;
    // TODO: That's broken, fix it
    //We need to keep on sending our pixel data until we are done
    while (remainingSize>0)
    {
        report.Reset();        
        report[0] = 0x00; //Report ID
        sizeWritten = 64;
        memset(report.Buffer() + 1, aValue, sizeWritten);
        int written=Write(report);
        while (written < 64)
        {
            written = Write(report);
        }
        remainingSize -= sizeWritten;
    }


    /*
    int i = 9;
    while (i < report.Size())
    {
        report[i] = 0xFF;
        i++;
    }
    Write(report);*/
}

void GU256X64D39XX::SwapBuffers()
{

}


