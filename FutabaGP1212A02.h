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

	//From GraphicDisplay
	virtual void SetPixel(unsigned char aX, unsigned char aY, bool aOn);
	virtual void SetAllPixels(unsigned char aPattern);
    virtual int FrameBufferSizeInBytes() const {return KGP12xFrameBufferSizeInBytes;}
    //virtual void BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const;
	virtual void SetBrightness(int aBrightness);
	virtual void Clear();
	virtual void Fill();

	//Specific to GP1212A02A
	void SetFrame(int aSize, unsigned char* aPixels);
	
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
    TMiniDisplayRequest AttemptRequestCompletion();
    FutabaVfdReport& InputReport() {return iInputReport;}
    bool PowerOn();
	char* DeviceId();
	char* FirmwareRevision();

private:
	unsigned char OffScreenY() const;
	void SendClearCommand();
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
