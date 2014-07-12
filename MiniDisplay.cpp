
#include "MiniDisplay.h"
#include "FutabaVfd.h"





//Open & Close functions
MiniDisplayDevice MiniDisplayOpen()
	{
	GP1212A01A* device=NULL;
	device=new GP1212A01A();
	int success = device->Open();
	if (!success)
		{
		delete device;
		return NULL;
		}

	return device;
	}

void MiniDisplayClose(MiniDisplayDevice aDevice)
	{
	delete ((GP1212A01A*)aDevice);
	}


void MiniDisplayClear(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GP1212A01A*)aDevice)->SetAllPixels(0x00);
	}


void MiniDisplayFill(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GP1212A01A*)aDevice)->SetAllPixels(0xFF);
	}


void MiniDisplaySwapBuffers(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return;
		}

	((GP1212A01A*)aDevice)->SwapBuffers();
	}

//-------------------------------------------------------------
int MiniDisplayMaxBrightness(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GP1212A01A*)aDevice)->MaxBrightness();
	}

//-------------------------------------------------------------
int MiniDisplayMinBrightness(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GP1212A01A*)aDevice)->MinBrightness();
	}

//-------------------------------------------------------------
void MiniDisplaySetBrightness(MiniDisplayDevice aDevice, int aBrightness)
	{
	if (!aDevice)
		{
		return;
		}

	((GP1212A01A*)aDevice)->SetBrightness(aBrightness);
	}

//-------------------------------------------------------------
int MiniDisplayWidthInPixels(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GP1212A01A*)aDevice)->WidthInPixels();
	}

//-------------------------------------------------------------
int MiniDisplayHeightInPixels(MiniDisplayDevice aDevice)
	{
	if (!aDevice)
		{
		return 0;
		}

	return ((GP1212A01A*)aDevice)->HeightInPixels();
	}

//-------------------------------------------------------------
void MiniDisplaySetPixel(MiniDisplayDevice aDevice, int aX, int aY, int aValue)
	{
	//aValue&=0x00FFFFFF; //Filter out alpha component
	return ((GP1212A01A*)aDevice)->SetPixel(aX,aY,aValue);
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