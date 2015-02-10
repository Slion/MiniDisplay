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

#include "MiniDisplay.h"
#include "FutabaGP1212A01.h"
#include "FutabaGP1212A02.h"
#include "FutabaMDM166AA.h"

/**
Make sure you update this list when adding a new display.
The order has to match the one from TMiniDisplayType.
*/
wchar_t* KDisplayNames[]=
	{
	L"Auto-Detect",
	L"Futaba GP1212A01",
	L"Futaba GP1212A02",
	L"Futaba MDM166AA",
	L"Unknown display device"
	};

MiniDisplayDevice MiniDisplayOpen(TMiniDisplayType aType, bool aAutoDetect)
	{
	GraphicDisplay* device=NULL;

	switch (aType)
		{
	case EMiniDisplayAutoDetect:
	case EMiniDisplayFutabaGP1212A01:
		device=new GP1212A01A();
		break;
		
	case EMiniDisplayFutabaGP1212A02:
		device=new GP1212A02A();
		break;

	case EMiniDisplayFutabaMDM166AA:
		device=new MDM166AA();
		break;

	case EMiniDisplayAutoDetectFailed:
		//Auto detect sequence failed
		return NULL;
		break;
		};

	int success = device->Open();
	if (!success)
		{
		delete device;
		device=NULL;
		if (aAutoDetect)
			{
			//Go recursive for auto detect
			int typeValue=(int)aType;
			typeValue++;
			TMiniDisplayType nextType=(TMiniDisplayType)typeValue;
			return MiniDisplayOpen(nextType,aAutoDetect);
			}
		}

	return device;
	}


//Open & Close functions
MiniDisplayDevice MiniDisplayOpen(TMiniDisplayType aType)
	{
	bool autoDetect=aType==EMiniDisplayAutoDetect;
	//If we want auto detect we need to pass in our first display type
	//If we don't want auto detect we just pass in the given display type. 
	return MiniDisplayOpen((autoDetect?EMiniDisplayFutabaGP1212A01:aType),autoDetect);
	}


//

void MiniDisplayClose(MiniDisplayDevice aDevice)
	{
	delete ((GraphicDisplay*)aDevice);
	}


int MiniDisplayTypeCount()
	{
	return EMiniDisplayAutoDetectFailed;
	}


wchar_t* MiniDisplayTypeName(TMiniDisplayType aType)
	{
		if (aType>=EMiniDisplayAutoDetectFailed)
		{
			return KDisplayNames[EMiniDisplayAutoDetectFailed];
		}

		return KDisplayNames[aType];
	}


void MiniDisplayClear(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GraphicDisplay*)aDevice)->Clear();
	}


void MiniDisplayFill(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GraphicDisplay*)aDevice)->Fill();
	}


void MiniDisplaySwapBuffers(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GraphicDisplay*)aDevice)->SwapBuffers();
	}

//-------------------------------------------------------------
int MiniDisplayMaxBrightness(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GraphicDisplay*)aDevice)->MaxBrightness();
	}

//-------------------------------------------------------------
int MiniDisplayMinBrightness(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GraphicDisplay*)aDevice)->MinBrightness();
	}

//-------------------------------------------------------------
void MiniDisplaySetBrightness(MiniDisplayDevice aDevice, int aBrightness)
	{
	if (!aDevice)
		{
		return;
		}

	((GraphicDisplay*)aDevice)->SetBrightness(aBrightness);
	}

//-------------------------------------------------------------
int MiniDisplayWidthInPixels(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GraphicDisplay*)aDevice)->WidthInPixels();
	}

//-------------------------------------------------------------
int MiniDisplayHeightInPixels(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GraphicDisplay*)aDevice)->HeightInPixels();
	}

//-------------------------------------------------------------
void MiniDisplaySetPixel(MiniDisplayDevice aDevice, int aX, int aY, unsigned int aPixel)
	{
	//aValue&=0x00FFFFFF; //Filter out alpha component
	return ((GraphicDisplay*)aDevice)->SetPixel(aX,aY,aPixel);
	}

//-------------------------------------------------------------
wchar_t* MiniDisplayVendor(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->Vendor();
    }

//-------------------------------------------------------------
wchar_t* MiniDisplayProduct(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->Product();
    }

//-------------------------------------------------------------
wchar_t* MiniDisplaySerialNumber(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->SerialNumber();
    }

//-------------------------------------------------------------
void MiniDisplayRequest(MiniDisplayDevice aDevice, TMiniDisplayRequest aRequest)
    {
    ((GraphicDisplay*)aDevice)->Request(aRequest);
    }

//-------------------------------------------------------------
bool MiniDisplayRequestPending(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->RequestPending();
    }

//-------------------------------------------------------------
TMiniDisplayRequest MiniDisplayCurrentRequest(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->CurrentRequest();
    }

//-------------------------------------------------------------
void MiniDisplayCancelRequest(MiniDisplayDevice aDevice)
    {
    ((GraphicDisplay*)aDevice)->CancelRequest();
    }

//-------------------------------------------------------------
TMiniDisplayRequest MiniDisplayAttemptRequestCompletion(MiniDisplayDevice aDevice)
	{
	return ((GraphicDisplay*)aDevice)->AttemptRequestCompletion();
	}

//-------------------------------------------------------------
char* MiniDisplayDeviceId(MiniDisplayDevice aDevice)
	{
	return ((GraphicDisplay*)aDevice)->DeviceId();
	}

//-------------------------------------------------------------
char* MiniDisplayFirmwareRevision(MiniDisplayDevice aDevice)
	{
	return ((GraphicDisplay*)aDevice)->FirmwareRevision();
	}

//-------------------------------------------------------------
bool MiniDisplayPowerSupplyStatus(MiniDisplayDevice aDevice)
	{
	return ((GraphicDisplay*)aDevice)->IsPowerOn();
	}

//-------------------------------------------------------------
void MiniDisplayPowerOn(MiniDisplayDevice aDevice)
    {
    ((GraphicDisplay*)aDevice)->TurnPowerOn();
    }

//-------------------------------------------------------------
void MiniDisplayPowerOff(MiniDisplayDevice aDevice)
    {
    ((GraphicDisplay*)aDevice)->TurnPowerOff();
    }

//-------------------------------------------------------------
bool MiniDisplaySupportPowerOnOff(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->SupportPowerOnOff();
    }

//-------------------------------------------------------------
void MiniDisplayShowClock(MiniDisplayDevice aDevice)
    {
    ((GraphicDisplay*)aDevice)->ShowClock();
    }

//-------------------------------------------------------------
void MiniDisplayHideClock(MiniDisplayDevice aDevice)
    {
    ((GraphicDisplay*)aDevice)->HideClock();
    }

//-------------------------------------------------------------
bool MiniDisplaySupportClock(MiniDisplayDevice aDevice)
    {
    return ((GraphicDisplay*)aDevice)->SupportClock();
    }

//-------------------------------------------------------------
int MiniDisplayIconCount(MiniDisplayDevice aDevice, TMiniDisplayIconType aIcon)
	{
	return ((GraphicDisplay*)aDevice)->IconCount(aIcon);
	}

//-------------------------------------------------------------
int MiniDisplayIconStatusCount(MiniDisplayDevice aDevice, TMiniDisplayIconType aIcon)
	{
	return ((GraphicDisplay*)aDevice)->IconStatusCount(aIcon);
	}

//-------------------------------------------------------------
void MiniDisplaySetIconStatus(MiniDisplayDevice aDevice, TMiniDisplayIconType aIcon, int aIndex, int aStatus)
	{
	((GraphicDisplay*)aDevice)->SetIconStatus(aIcon,aIndex,aStatus);
	}