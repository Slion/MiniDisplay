//
//
//

#ifndef FUTABA_GP1212A02_H
#define FUTABA_GP1212A02_H

#include "FutabaGP1212.h"

#include "FutabaGP1212.h"
#include "FutabaVfd.h"

/**
GP1212A01A is a graphic display module using a FUTABA 256x64dots VFD.
The module do not include character ROM, the customer will compile the character
by themselves (from main system). 
*/
class GP1212A02A : public GP1212XXXX
	{
public:

    GP1212A02A();
    ~GP1212A02A();

	//From DisplayBase
	int Open();
	virtual void SwapBuffers();
	virtual void TurnPowerOn();
	virtual void TurnPowerOff();
	virtual bool SupportPowerOnOff(){return true;}
	virtual void ShowClock();
	virtual void HideClock();
	virtual bool SupportClock(){return true;}



	//From GraphicDisplay
	virtual void SetPixel(unsigned char aX, unsigned char aY, bool aOn);
	virtual void SetAllPixels(unsigned char aPattern);
    virtual int FrameBufferSizeInBytes() const {return KGP12xFrameBufferSizeInBytes;}
	virtual void SetBrightness(int aBrightness);
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

	enum TBmpBoxId
	{
		EBmpBoxIdNull=0x30,
		EBmpBoxIdOne=0x31,
		EBmpBoxIdTwo=0x32,
		EBmpBoxIdThree=0x33
	};

	enum TTarget
	{
		ETargetDisplayWindow=0x30,
		ETargetDataMemory=0x31
	};

	enum TDirection
	{
		EDirectionY=0x30,
		EDirectionX=0x31
	};

	enum TPowerStatus
	{
		EPowerOff=0x30,
		EPowerOn=0x31
	};

	enum TWeekDay
	{
		ESunday		=	0x00,
		EMonday		=	0x01,
		ETuesday	=	0x02,
		EWednesday	=	0x03,
		EThrusday	=	0x04,
		EFriday		=	0x05,
		ESaturday	=	0x06
	};

	enum TClockFormat
	{
		EClock24	=	0x00,
		EClockDay24	=	0x01,
		EClock12	=	0x10,
		EClockDay12	=	0x11
	};

	enum TFontSizeLogical
	{
		EFontTiny		=	0x30,
		EFontSmall		=	0x31,
		EFontMedium	=	0x32,
		EFontLarge		=	0x33
	};

	enum TFontSize
	{
		EFont6x8	= 0x30,
		EFont8x16	= 0x31,
		EFont12x24	= 0x32,
		EFont16x32	= 0x33,
		EFont16x16	= 0x34,
		EFont24x24	= 0x35,
		EFont32x32	= 0x36
	};

	enum TFontAction
	{
		EFontStore		= 0x30,
		EFontTransfer	= 0x31,
		EFontDelete		= 0x32
	};


private:
	//Specific to GP1212A02A
	//General setting command
	void SendCommandClear();
	void SendCommandReset();
	//
	void BmpDataInput(TTarget aTarget, unsigned short aAddress, TDirection aDirection, unsigned short aSize, unsigned char* aPixels);
	//
	void SendCommandPower(TPowerStatus aPowerStatus);
	//Clock commands
	void SendCommandClockSetting(TWeekDay aWeekDay, unsigned char aHour, unsigned char aMinute);
	void SendCommandClockDisplay(TClockFormat aClockFormat, unsigned short aAddress, TFontSizeLogical aSize);	
	void SendCommandClockCancel();
	//Font commands
	void SendCommandDefineCharacter(TFontSize aFontSize, unsigned short aCharacterCode, unsigned char* aPixelData);
	void SendCommandFontAction(TFontAction aFontAction);
	void SendCommandSelectFontSize(TFontSizeLogical aFontSoze);


	//BMP box
	void BmpBoxSetting(TBmpBoxId aBoxId, unsigned short aAddress, int aWidth, int aHeight);
	void BmpBoxSelect(TBmpBoxId aBoxId);
	void BmpBoxDataMemoryTransfer(unsigned short aAddress);
	void BmpBoxDataInput(unsigned short aSize, unsigned char* aPixels);

	//Clock utilities
	int ClockCharCount(TClockFormat aFormat);
	int ClockCharWidthInPixels(TFontSizeLogical aSize);
	int ClockCharHeightInPixels(TFontSizeLogical aSize);
	unsigned short ClockCenterAddress(TClockFormat aFormat, TFontSizeLogical aSize);
	//Font utilities
	int CharacterSizeInBytes(TFontSize aFontSize);

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
	//
	unsigned short iNextFrameAddress;
	};



#endif
