//
//
//

#ifndef FUTABA_VFD_H
#define FUTABA_VFD_H

#include "hidapi.h"
#include "HidDevice.h"
#include "BitArray.h"

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

/**
*/
class FutabaVfd : public HidDevice
	{
public:
	virtual int MinBrightness() const=0;
	virtual int MaxBrightness() const=0;
	virtual void SetBrightness(int aBrightness)=0;
	virtual void Clear()=0;
	};


/**
*/
class FutabaGraphicVfd : public FutabaVfd
	{
public:
	virtual int WidthInPixels() const=0;
	virtual int HeightInPixels() const=0;
	virtual void SetPixel(unsigned char aX, unsigned char aY, bool aOn)=0;
	virtual void SetAllPixels(unsigned char aOn)=0;
	virtual int FrameBufferSizeInBytes() const=0;
	//virtual int BitBlit(unsigned char* aSrc, unsigned char aSrcWidth, unsigned char aSrcHeight, unsigned char aTargetX, unsigned char aTargetY) const=0;

	};

/**
Common functionality between GP1212A01A and GP1212A02A
*/
class GP1212XXXX : public FutabaGraphicVfd
	{
public:
	//From FutabaVfd
    virtual int MinBrightness() const {return 0;}
    virtual int MaxBrightness() const {return 5;}
	};

/**
GP1212A01A is a graphic display module using a FUTABA 256x64dots VFD.
The module do not include character ROM, the customer will compile the character
by themselves (from main system).
*/
class GP1212A01A : public GP1212XXXX
	{
public:
    enum Request
        {
        ERequestNone,
        ERequestDeviceId,
        ERequestFirmwareRevision,
        ERequestPowerSupplyStatus
        };

public:
    GP1212A01A();
    ~GP1212A01A();

	//
	int Open();
	//From FutabaGraphicVfd
    virtual int WidthInPixels() const {return KGP12xWidthInPixels;}
    virtual int HeightInPixels() const {return KGP12xHeightInPixels;}
	virtual void SetPixel(unsigned char aX, unsigned char aY, bool aOn);
	virtual void SetAllPixels(unsigned char aPattern);
    virtual int FrameBufferSizeInBytes() const {return KGP12xFrameBufferSizeInBytes;}
    virtual void BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const;
	//From FutabaVfd
	virtual void SetBrightness(int aBrightness);
	virtual void Clear();

	//Specific to GP1212A01A
	void SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char aValue);
    void SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char* aPixels);
    //Define display position within our display RAM
	void SetDisplayPosition(unsigned char aX, unsigned char aY);
    unsigned char DisplayPositionX() const {return iDisplayPositionX;}
    unsigned char DisplayPositionY() const {return iDisplayPositionY;}
    //
	void SwapBuffers();
    //
    void RequestDeviceId();
    void RequestFirmwareRevision();
    void RequestPowerSupplyStatus();
	//
	void ToggleOffScreenMode();
    void SetOffScreenMode(bool aOn);
    bool OffScreenMode() const {return iOffScreenMode;}
    //
    void SetFrameDifferencing(bool aOn){iUseFrameDifferencing=aOn;}
    bool FrameDifferencing() const {return iUseFrameDifferencing;}
    //
    bool RequestPending(){return iRequest!=ERequestNone;}
    Request CurrentRequest(){return iRequest;}
    void CancelRequest(){iRequest=ERequestNone;}
    Request AttemptRequestCompletion();
    FutabaVfdReport& InputReport() {return iInputReport;}
    bool PowerOn(){return iPowerOn;}

private:
	enum DW
		{
        DW1=0xC0,
        DW2=0xD0
		};

	void SetDisplayPosition(DW aDw,unsigned char aX, unsigned char aY);
	unsigned char OffScreenY() const;
	void SendClearCommand();
	void OffScreenTranslation(unsigned char& aX, unsigned char& aY);
	void ResetBuffers();
    void SendModifiedPixelBlocks();

private:
	unsigned char iDisplayPositionX;
	unsigned char iDisplayPositionY;
	///Off screen mode is the recommended default settings to avoid tearing.
	///Though turning it off can be useful for debugging
	bool iOffScreenMode;
    ///Frame differences algo is used to reduce USB bus traffic and improve frame rate in typical use case
    bool iUseFrameDifferencing;
	///
	//FutabaVfdReport iReport;
	///
	//unsigned char iFrameBuffer[256*64];
    BitArray* iFrameNext;
    BitArray* iFrameCurrent;
    BitArray* iFramePrevious;
    //
    BitArray* iFrameAlpha;
    BitArray* iFrameBeta;
    BitArray* iFrameGamma;
    //
    int iNeedFullFrameUpdate;
	//unsigned char iFrameBeta[256*64];
	//unsigned char *iFrontBuffer;
	//unsigned char *iBackBuffer;
    Request iRequest;
    FutabaVfdReport iInputReport;
    bool iPowerOn;
	};


#endif
