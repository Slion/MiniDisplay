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

#ifndef FUTABA_VFD_H
#define FUTABA_VFD_H

#include "hidapi.h"
#include "BitArray.h"
#include "MiniDisplay.h"
#include "Display.h"


//This was computed from our number of pixels as follow 256x64/8/64 = 32 + 1 = 33
//+1 was added for our header
const int KFutabaMaxCommandOutputReport = 33;
//TODO: Get ride of that constant once we figure out a way to get it from hidapi
const int KFutabaMaxHidReportSize = 65;

const int KHidReportIdIndex=0;
const int KFutabaHidReportSizeIndex=1;
//Define Futaba vendor ID to filter our list of device
const unsigned short KTargaVendorId = 0x19C2;
const unsigned short KFutabaVendorId = 0x1008;
const unsigned short KFutabaProductIdGP1212A01A = 0x100C;
const unsigned short KFutabaProductIdGP1212A02A = 0x1015;
const unsigned short KFutabaProductIdMDM166AA = 0x6A11;


//typedef struct hid_device_info HidDeviceInfo;

/**
Define a Futaba HID report.
*/
class FutabaVfdReport: public HidReport<KFutabaMaxHidReportSize>
	{

private:

	};


/**
Define a generic Futaba VFD command.
*/
class FutabaVfdCommand
    {
public:
    FutabaVfdCommand();
    ~FutabaVfdCommand();
    //
    //void Create(int aMaxSize);
    //void Delete();

    //inline unsigned char& operator[](int aIndex){return iBuffer[aIndex];}

    void Reset();

private:
    //unsigned char* iBuffer;
    FutabaVfdReport iReports[KFutabaMaxCommandOutputReport];
    int iSize;
    int iMaxSize;
    };

/**
*/
class FutabaGraphicDisplay : public GraphicDisplay
	{
public:
	//From DisplayBase
	virtual void Close();

	};


#endif
