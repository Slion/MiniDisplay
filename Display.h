//
// Copyright (C) 2014-2015 Stéphane Lenclud.
//
// This file is part of MiniDisplay.
//
// MiniDisplay is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MiniDisplay is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MiniDisplay.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include "MiniDisplay.h"
#include "HidDevice.h"
const int KMaxDisplayStringLength = 256;

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif


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

	//Icon management
	//Icon support queries return the number of segments an icon supports.
	virtual int IconCount(TMiniDisplayIconType /*aIcon*/){return 0;}
	virtual int IconStatusCount(TMiniDisplayIconType /*aIcon*/){return 0;}
	virtual void SetIconStatus(TMiniDisplayIconType /*aIcon*/, int /*aIndex*/, int /*aStatus*/){}

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
