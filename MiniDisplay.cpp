
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
	delete aDevice;
	//device = NULL;
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

