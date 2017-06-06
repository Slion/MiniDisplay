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

#ifndef NORITAKE_GU256X64D39XX_H
#define NORITAKE_GU256X64D39XX_H

#include "ArduinoDisplay.h"



/**

*/
class GU256X64D39XX : public ArduinoGraphicDisplay
{
public:
    //From DisplayBase
    virtual int Open();
    virtual void SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel);
    virtual void SetAllPixels(unsigned char aPattern);
    virtual void SetBrightness(int aBrightness);
    virtual void Clear();
    virtual void Fill();
    virtual void SwapBuffers();

    //From GraphicDisplay
    virtual int MinBrightness() const { return 0; }
    virtual int MaxBrightness() const { return 4; }
    virtual int WidthInPixels() const { return KWidthInPixels; }
    virtual int HeightInPixels() const { return KHeightInPixels; }
    virtual int FrameBufferSizeInBytes() const { return KFrameBufferSizeInBytes; }

private:
    void CmdBitImageWrite(unsigned short aRamAddress, unsigned short aSize, unsigned char aValue);

public:
    static const int KWidthInPixels = 256;
    static const int KHeightInPixels = 64;
    static const int KPixelsPerByte = 8;
    static const int KFrameBufferSizeInBytes = KWidthInPixels*KHeightInPixels / KPixelsPerByte; //256*64/8=2048
    static const int KFrameBufferPixelCount = KWidthInPixels*KHeightInPixels;


};



#endif
