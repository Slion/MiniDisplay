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


#ifndef HID_REPORT_H
#define HID_REPORT_H

#include <string.h>

/**
Define an HID report.
Can be used as input and output.
*/
template <int S>
class HidReport
    {
public:
    HidReport(){Reset();}
    void Reset();
    inline unsigned char& operator[](int aIndex){return iBuffer[aIndex];}
    const unsigned char* Buffer() const {return iBuffer;}
    unsigned char* Buffer() {return iBuffer;}
    int Size() const {return iSize;}
    void SetSize(int aSize) { iSize=aSize; }
    int MaxSize() const {return S;}
protected:
    int iSize;
    unsigned char iBuffer[S];
    };

template <int S>
void HidReport<S>::Reset()
    {
    // Size should be set to zero I guess
    // However for backward compatibility we keep it like that for now
    iSize = MaxSize();
    memset(iBuffer,0,sizeof(iBuffer));
    }


#endif
