
#include "MiniDisplay.h"
#include "FutabaGP1212A01.h"
#include "FutabaGP1212A02.h"


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
void MiniDisplaySetPixel(MiniDisplayDevice aDevice, int aX, int aY, int aValue)
	{
	//aValue&=0x00FFFFFF; //Filter out alpha component
	return ((GraphicDisplay*)aDevice)->SetPixel(aX,aY,aValue);
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