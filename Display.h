//
//
//

#ifndef DISPLAY_H
#define DISPLAY_H


#include "HidDevice.h"
const int KMaxDisplayStringLength = 256;

/**
Define an interface to some basic display functionality
*/
class DisplayBase: public HidDevice
	{
public:
	DisplayBase():iRequest(EMiniDisplayRequestNone),iPowerOn(false)
		{	    
		iDeviceId[0]=0;
		iFirmwareRevision[0]=0;
		}

	virtual ~DisplayBase(){};
	//
	virtual int Open()=0;
	virtual void Close()=0;
	//
	virtual int MinBrightness() const=0;
	virtual int MaxBrightness() const=0;
	virtual void SetBrightness(int aBrightness)=0;
	//
	virtual void Clear()=0;
	virtual void Fill()=0;
	//
	virtual void SwapBuffers()=0;

	//Request management
	virtual void Request(TMiniDisplayRequest /*aRequest*/){ /*No request supported by default*/ };
	virtual TMiniDisplayRequest AttemptRequestCompletion(){return EMiniDisplayRequestNone;};
	
	virtual TMiniDisplayRequest CurrentRequest(){return iRequest;}
    virtual void CancelRequest(){iRequest=EMiniDisplayRequestNone;}
	virtual bool RequestPending(){return iRequest!=EMiniDisplayRequestNone;}

    virtual bool IsPowerOn()	{return iPowerOn;}
	virtual char* DeviceId() {return iDeviceId;}
	virtual char* FirmwareRevision() {return iFirmwareRevision;}

	virtual void TurnPowerOn(){}
	virtual void TurnPowerOff(){}
	virtual bool SupportPowerOnOff(){return false;}

	virtual void ShowClock(){}
	virtual void HideClock(){}
	virtual bool SupportClock(){return false;}

protected:
	void SetRequest(TMiniDisplayRequest aRequest) { iRequest=aRequest; }

protected:
	char iDeviceId[KMaxDisplayStringLength];
	char iFirmwareRevision[KMaxDisplayStringLength];
    bool iPowerOn;


private:
	TMiniDisplayRequest iRequest;
	};


/**
*/
class GraphicDisplay : public DisplayBase
	{
public:
	virtual int WidthInPixels() const=0;
	virtual int HeightInPixels() const=0;
	virtual void SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel)=0;
	virtual void SetAllPixels(unsigned char aOn)=0;
	virtual int FrameBufferSizeInBytes() const=0;
	//virtual int BitBlit(unsigned char* aSrc, unsigned char aSrcWidth, unsigned char aSrcHeight, unsigned char aTargetX, unsigned char aTargetY) const=0;

	};



#endif
