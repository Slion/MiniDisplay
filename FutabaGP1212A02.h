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
    virtual void BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const;
	virtual void SetBrightness(int aBrightness);
	virtual void Clear();
	virtual void Fill();

	//Specific to GP1212A01A
	void SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char aValue);
    void SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char* aPixels);
    //Define display position within our display RAM
	void SetDisplayPosition(unsigned char aX, unsigned char aY);
    unsigned char DisplayPositionX() const {return iDisplayPositionX;}
    unsigned char DisplayPositionY() const {return iDisplayPositionY;}
	
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
    bool RequestPending(){return iRequest!=EMiniDisplayRequestNone;}
    TMiniDisplayRequest CurrentRequest(){return iRequest;}
    void CancelRequest(){iRequest=EMiniDisplayRequestNone;}
    TMiniDisplayRequest AttemptRequestCompletion();
    FutabaVfdReport& InputReport() {return iInputReport;}
    bool PowerOn();
	char* DeviceId();
	char* FirmwareRevision();

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
    TMiniDisplayRequest iRequest;
    FutabaVfdReport iInputReport;
	//
	char iDeviceId[KFutabaMaxHidReportSize];
	char iFirmwareRevision[KFutabaMaxHidReportSize];
    bool iPowerOn;
	};



#endif
