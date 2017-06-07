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

#ifndef ARDUINO_DISPLAY_H
#define ARDUINO_DISPLAY_H

#include "hidapi.h"
#include "BitArray.h"
#include "MiniDisplay.h"
#include "Display.h"


//TODO: Get ride of that constant once we figure out a way to get it from hidapi
const int KArduinoMaxHidReportSize = 65; // Header and a full screen worth of data

//Define Futaba vendor ID to filter our list of device
const unsigned short KArduinoVendorId = 0x2341;
const unsigned short KArduinoMicroProductId = 0x8037;


//typedef struct hid_device_info HidDeviceInfo;

/**
Define a Futaba HID report.
*/
class ArduinoReport : public HidReport<KArduinoMaxHidReportSize>
{

private:

};


/**
*/
class ArduinoGraphicDisplay : public GraphicDisplay
{
public:
    //From DisplayBase
    virtual void Close();

protected:
    static const int KReportMinHeaderSize = 2;

};


#endif
