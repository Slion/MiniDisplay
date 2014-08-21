//
//
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include "HidDevice.h"

/**
Define an interface to some basic display functionality
*/
class DisplayBase : public HidDevice
	{
public:
	virtual int MinBrightness() const=0;
	virtual int MaxBrightness() const=0;
	virtual void SetBrightness(int aBrightness)=0;
	virtual void Clear()=0;
	};


/**
*/
class GraphicDisplay : public DisplayBase
	{
public:
	virtual int WidthInPixels() const=0;
	virtual int HeightInPixels() const=0;
	virtual void SetPixel(unsigned char aX, unsigned char aY, bool aOn)=0;
	virtual void SetAllPixels(unsigned char aOn)=0;
	virtual int FrameBufferSizeInBytes() const=0;
	//virtual int BitBlit(unsigned char* aSrc, unsigned char aSrcWidth, unsigned char aSrcHeight, unsigned char aTargetX, unsigned char aTargetY) const=0;

	};



#endif
