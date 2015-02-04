//
//
//

#ifndef FUTABA_MDM166AA_H
#define FUTABA_MDM166AA_H

#include "FutabaVfd.h"


const int KMDM166AAWidthInPixels = 96;
const int KMDM166AAHeightInPixels = 16;
const int KMDM166AAPixelsPerByte = 8;
const int KMDM166AAFrameBufferSizeInBytes = KMDM166AAWidthInPixels*KMDM166AAHeightInPixels/KMDM166AAPixelsPerByte; //96*16/8=192
const int KMDM166AAFrameBufferPixelCount = KMDM166AAWidthInPixels*KMDM166AAHeightInPixels;


/**
MDM166AA is a graphic display module using a FUTABA 96x16dots VFD.
*/
class MDM166AA : public FutabaGraphicDisplay
	{
public:

    MDM166AA();
    ~MDM166AA();

	//From DisplayBase
	virtual int Open();
	virtual void SwapBuffers();
		//Brightness support
    virtual int MinBrightness() const {return 0;}
    virtual int MaxBrightness() const {return 2;}
	virtual void SetBrightness(int aBrightness);
		//Clock support
	virtual void ShowClock();
	virtual void HideClock();
	virtual bool SupportClock(){return true;}

	//From GraphicDisplay
    virtual int WidthInPixels() const {return KMDM166AAWidthInPixels;}
    virtual int HeightInPixels() const {return KMDM166AAHeightInPixels;}

	virtual void SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel);
	virtual void SetAllPixels(unsigned char aPattern);
    virtual int FrameBufferSizeInBytes() const {return KMDM166AAFrameBufferSizeInBytes;}	
	virtual void Clear();
	virtual void Fill();
	virtual void Request(TMiniDisplayRequest aRequest);
		
	//
	void ToggleOffScreenMode();
    void SetOffScreenMode(bool aOn);
    bool OffScreenMode() const {return iOffScreenMode;}
    //
    void SetFrameDifferencing(bool aOn){iUseFrameDifferencing=aOn;}
    bool FrameDifferencing() const {return iUseFrameDifferencing;}
    //    
    TMiniDisplayRequest AttemptRequestCompletion();
    FutabaVfdReport& InputReport() {return iInputReport;}
    bool IsPowerOn();
	char* DeviceId();
	char* FirmwareRevision();

private:

	enum TClockFormat
	{
		EClock12	=	0x00,
		EClock24	=	0x01,
	};

	enum TClockSize
	{
		EClockSmall		=	0x01,
		EClockLarge		=	0x02
	};
	

private:
	//Specific to MDM166AA
	//General setting command
	void SendCommandClear();
	void SendCommandReset();
	void SendCommandSetAddressCounter(unsigned char aAddressCounter);
	//void SendCommandWriteGraphicData();
	//
	//Clock commands
	void SendCommandClockSetting(unsigned char aHour, unsigned char aMinute);
	void SendCommandClockDisplay(TClockSize aClockSize, TClockFormat aClockFormat);	

	void SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char* aPixels);

private:
    void RequestDeviceId();
    void RequestFirmwareRevision();
    void RequestPowerSupplyStatus();
	//
	void SetClockSetting();


private:
	unsigned char OffScreenY() const;	
	void OffScreenTranslation(unsigned char& aX, unsigned char& aY);
	void ResetBuffers();

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
    BitArrayLow* iFrameNext;
    BitArrayLow* iFrameCurrent;
    BitArrayLow* iFramePrevious;
    //
    BitArrayLow* iFrameAlpha;
    BitArrayLow* iFrameBeta;
    BitArrayLow* iFrameGamma;
    //
    int iNeedFullFrameUpdate;
	//unsigned char iFrameBeta[256*64];
	//unsigned char *iFrontBuffer;
	//unsigned char *iBackBuffer;
    FutabaVfdReport iInputReport;
	//
	char iDeviceId[KFutabaMaxHidReportSize];
	char iFirmwareRevision[KFutabaMaxHidReportSize];
    bool iPowerOn;
	};



#endif
