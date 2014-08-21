//
//
//

#ifndef FUTABA_VFD_H
#define FUTABA_VFD_H

#include "hidapi.h"
#include "HidDevice.h"
#include "BitArray.h"
#include "MiniDisplay.h"

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif


//This was computed from our number of pixels as follow 256x64/8/64 = 32 + 1 = 33
//+1 was added for our header
const int KFutabaMaxCommandOutputReport = 33;
//TODO: Get ride of that constant once we figure out a way to get it from hidapi
const int KFutabaMaxHidReportSize = 65;

const int KHidReportIdIndex=0;
const int KFutabaHidReportSizeIndex=1;
//Define Futaba vendor ID to filter our list of device
const unsigned short KFutabaVendorId = 0x1008;
const unsigned short KFutabaProductIdGP1212A01A = 0x100C;
const unsigned short KFutabaProductIdGP1212A02A = 0x1013; //Or is it 0x1015
const int KGP12xWidthInPixels = 256;
const int KGP12xHeightInPixels = 64;
const int KGP12xPixelsPerByte = 8;
const int KGP12xFrameBufferSizeInBytes = KGP12xWidthInPixels*KGP12xHeightInPixels/KGP12xPixelsPerByte; //256*64/8=2048
const int KGP12xFrameBufferPixelCount = KGP12xWidthInPixels*KGP12xHeightInPixels;

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


#endif
