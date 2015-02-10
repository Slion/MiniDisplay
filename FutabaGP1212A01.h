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

#ifndef FUTABA_GP1212A01_H
#define FUTABA_GP1212A01_H

#include "FutabaGP1212.h"
#include "FutabaVfd.h"

/**
GP1212A01A is a graphic display module using a FUTABA 256x64dots VFD.
The module do not include character ROM, the customer will compile the character
by themselves (from main system).
*/
class GP1212A01A : public GP1212XXXX
	{
public:


public:
    GP1212A01A();
    ~GP1212A01A();

	//From DisplayBase
	int Open();
	//From FutabaGraphicVfd
	virtual void SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel);
	virtual void SetAllPixels(unsigned char aPattern);
    virtual int FrameBufferSizeInBytes() const {return KGP12xFrameBufferSizeInBytes;}
    //virtual void BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const;
	//From FutabaVfd
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
	void SwapBuffers();
    //
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

private:
	void RequestDeviceId();
    void RequestFirmwareRevision();
    void RequestPowerSupplyStatus();


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
    BitArrayHigh* iFrameNext;
    BitArrayHigh* iFrameCurrent;
    BitArrayHigh* iFramePrevious;
    //
    BitArrayHigh* iFrameAlpha;
    BitArrayHigh* iFrameBeta;
    BitArrayHigh* iFrameGamma;
    //
    int iNeedFullFrameUpdate;
	//unsigned char iFrameBeta[256*64];
	//unsigned char *iFrontBuffer;
	//unsigned char *iBackBuffer;
    FutabaVfdReport iInputReport;
	};


#endif
