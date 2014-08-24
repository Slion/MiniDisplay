
#include "MiniDisplay.h"
#include "FutabaGP1212A01.h"
#include "FutabaGP1212A02.h"



//Open & Close functions
MiniDisplayDevice MiniDisplayOpen(TMiniDisplayType aType)
	{
	GraphicDisplay* device=NULL;

	switch (aType)
		{
	case EMiniDisplayAutoDetect:
		//TODO
		device=new GP1212A01A();
		break;

	case EMiniDisplayFutabaGP1212A01:
		device=new GP1212A01A();
		break;
		
	case EMiniDisplayFutabaGP1212A02:
		device=new GP1212A02A();
		break;
		};

	int success = device->Open();
	if (!success)
		{
		delete device;
		device=NULL;
		}

	return device;
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
    return ((GP1212A01A*)aDevice)->Vendor();
    }

//-------------------------------------------------------------
wchar_t* MiniDisplayProduct(MiniDisplayDevice aDevice)
    {
    return ((GP1212A01A*)aDevice)->Product();
    }

//-------------------------------------------------------------
wchar_t* MiniDisplaySerialNumber(MiniDisplayDevice aDevice)
    {
    return ((GP1212A01A*)aDevice)->SerialNumber();
    }

//-------------------------------------------------------------
void MiniDisplayRequestDeviceId(MiniDisplayDevice aDevice)
    {
    ((GP1212A01A*)aDevice)->RequestDeviceId();
    }

//-------------------------------------------------------------
void MiniDisplayRequestPowerSupplyStatus(MiniDisplayDevice aDevice)
    {
    ((GP1212A01A*)aDevice)->RequestPowerSupplyStatus();
    }

//-------------------------------------------------------------
void MiniDisplayRequestFirmwareRevision(MiniDisplayDevice aDevice)
    {
    ((GP1212A01A*)aDevice)->RequestFirmwareRevision();
    }

//-------------------------------------------------------------
bool MiniDisplayRequestPending(MiniDisplayDevice aDevice)
    {
    return ((GP1212A01A*)aDevice)->RequestPending();
    }

//-------------------------------------------------------------
TMiniDisplayRequest MiniDisplayCurrentRequest(MiniDisplayDevice aDevice)
    {
    return ((GP1212A01A*)aDevice)->CurrentRequest();
    }

//-------------------------------------------------------------
void MiniDisplayCancelRequest(MiniDisplayDevice aDevice)
    {
    ((GP1212A01A*)aDevice)->CancelRequest();
    }

//-------------------------------------------------------------
TMiniDisplayRequest MiniDisplayAttemptRequestCompletion(MiniDisplayDevice aDevice)
	{
	return ((GP1212A01A*)aDevice)->AttemptRequestCompletion();
	}

//-------------------------------------------------------------
char* MiniDisplayDeviceId(MiniDisplayDevice aDevice)
	{
	return ((GP1212A01A*)aDevice)->DeviceId();
	}

//-------------------------------------------------------------
char* MiniDisplayFirmwareRevision(MiniDisplayDevice aDevice)
	{
	return ((GP1212A01A*)aDevice)->FirmwareRevision();
	}

//-------------------------------------------------------------
bool MiniDisplayPowerSupplyStatus(MiniDisplayDevice aDevice)
	{
	return ((GP1212A01A*)aDevice)->PowerOn();
	}