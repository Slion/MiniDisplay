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

#include "HidDevice.h"

//
// class HidDevice
//

HidDevice::HidDevice():iHidDevice(NULL)
    {
    Close();
    }

/**
*/
HidDevice::~HidDevice()
	{
	Close();
	}


/**
*/
int HidDevice::Open(const char* aPath)
	{
	Close();

	iHidDevice =  hid_open_path(aPath);

	if (!iHidDevice)
		{
		//Fail to connect our device
		return 0;
		}

    FetchStrings();

	return 1;
	}

/**
See hidapi documentation.
*/
int HidDevice::Open(unsigned short aVendorId, unsigned short aProductId, const wchar_t* aSerialNumber)
	{
    Close();

	iHidDevice = hid_open(aVendorId, aProductId, aSerialNumber);

	if (!iHidDevice)
		{
		//Fail to connect our device
		return 0;
		}

    FetchStrings();

	return 1;
	}

/**
See hidapi documentation.
*/
int HidDevice::Open(unsigned short aVendorId, unsigned short aProductId, unsigned short aUsagePage, unsigned short aUsage)
{
    Close();

    iHidDevice = hid_open_usage(aVendorId, aProductId, aUsagePage, aUsage);

    if (!iHidDevice)
    {
        //Fail to connect our device
        return 0;
    }

    FetchStrings();

    return 1;
}

/**
Close this HID device
*/
void HidDevice::Close()
	{
	hid_close(iHidDevice); //No effect if device is null
	iHidDevice=NULL;
    //
    memset(iVendor,0,sizeof(iVendor));
    memset(iProduct,0,sizeof(iProduct));
    memset(iSerialNumber,0,sizeof(iSerialNumber));
	}

/**
*/
bool HidDevice::IsOpen()
    {
    return iHidDevice!=NULL;
    }


/**
*/
const wchar_t* HidDevice::Error()
	{
	return hid_error(iHidDevice);
	}

/**
*/
int HidDevice::SetNonBlocking(int aNonBlocking)
	{
	//Success we are now connected to our HID device
	//Set read operation as non blocking
	return hid_set_nonblocking(iHidDevice, aNonBlocking);
	}

/**
*/
wchar_t* HidDevice::Vendor()
    {
    return iVendor;
    }

/**
*/
wchar_t* HidDevice::Product()
    {
    return iProduct;
    }

/**
*/
wchar_t* HidDevice::SerialNumber()
    {
    return iSerialNumber;
    }

/**

*/
void HidDevice::FetchStrings()
    {
    hid_get_manufacturer_string(iHidDevice,iVendor,sizeof(iVendor));
    hid_get_product_string(iHidDevice,iProduct,sizeof(iProduct));
    hid_get_serial_number_string(iHidDevice,iSerialNumber,sizeof(iSerialNumber));
    }
