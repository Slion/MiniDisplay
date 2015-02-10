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

#ifndef FUTABA_GP1212_H
#define FUTABA_GP1212_H

#include "FutabaVfd.h"

const int KGP12xWidthInPixels = 256;
const int KGP12xHeightInPixels = 64;
const int KGP12xPixelsPerByte = 8;
const int KGP12xFrameBufferSizeInBytes = KGP12xWidthInPixels*KGP12xHeightInPixels/KGP12xPixelsPerByte; //256*64/8=2048
const int KGP12xFrameBufferPixelCount = KGP12xWidthInPixels*KGP12xHeightInPixels;


/**
Common functionality between GP1212A01A and GP1212A02A
*/
class GP1212XXXX : public FutabaGraphicDisplay
	{
public:
	//From GraphicDisplay
    virtual int MinBrightness() const {return 0;}
    virtual int MaxBrightness() const {return 5;}
    virtual int WidthInPixels() const {return KGP12xWidthInPixels;}
    virtual int HeightInPixels() const {return KGP12xHeightInPixels;}

	};



#endif
