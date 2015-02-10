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
		//Icons
	virtual int IconCount(TMiniDisplayIconType aIcon);
	virtual int IconStatusCount(TMiniDisplayIconType aIcon);
	virtual void SetIconStatus(TMiniDisplayIconType aIcon, int aIndex, int aStatus);

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

		//Icon specific
	void SetIconNetworkSignal(int aIndex, int aStatus);
	void SetIconInternet(int aIndex, int aStatus);
	void SetIconEmail(int aIndex, int aStatus);
	void SetIconMute(int aIndex, int aStatus);
	void SetIconVolumeLabel(int aIndex, int aStatus);
	void SetIconVolume(int aIndex, int aStatus);
	void SetIconPlay(int aIndex, int aStatus);
	void SetIconPause(int aIndex, int aStatus);
	void SetIconRecording(int aIndex, int aStatus);


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
	
	enum TIconId
		{		
		EIconPlay				=	0x00,
		EIconPause				=	0x01,
		EIconRecording			=	0x02,
		EIconEnvelop			=	0x03,
		EIconAt					=	0x04,
		EIconMute				=	0x05,
		EIconNetworkMast		=   0x06,
		EIconNetworkSignalLow	=   0x07,
		EIconNetworkSignalMid	=   0x08,
		EIconNetworkSignalHigh	=   0x09,
		EIconVolumeLabel		=   0x0A,
		EIconVolumeLevel01		=   0x0B,
		EIconVolumeLevel02		=   0x0C,
		EIconVolumeLevel03		=   0x0D,
		EIconVolumeLevel04		=   0x0E,
		EIconVolumeLevel05		=   0x0F,
		EIconVolumeLevel06		=   0x10,
		EIconVolumeLevel07		=   0x11,
		EIconVolumeLevel08		=   0x12,
		EIconVolumeLevel09		=   0x13,
		EIconVolumeLevel10		=   0x14,
		EIconVolumeLevel11		=   0x15,
		EIconVolumeLevel12		=   0x16,
		EIconVolumeLevel13		=   0x17,
		EIconVolumeLevel14		=   0x18,
		EIconFirst				=   EIconPlay,
		EIconLast				=	EIconVolumeLevel14,		
		};

	enum TIconStatus
		{
		EIconOff		=	0x00,
		EIconFaded		=	0x01,
		EIconOn			=	0x02
		};
	

private:
	//Testing
	void SetAllIcons(TIconStatus aStatus);

	//Specific to MDM166AA
	//General setting command
	void SendCommandClear();
	void SendCommandReset();
	//
	//Clock commands
	void SendCommandSetClockData(unsigned char aHour, unsigned char aMinute);
	void SendCommandClockDisplay(TClockSize aClockSize, TClockFormat aClockFormat);	
	void AttemptClockSynchronization();

	//Graphics commands
	void SendCommandSetAddressCounter(unsigned char aAddressCounter);
	void SendCommandWriteGraphicData(int aSize, unsigned char* aPixels);

	//Icon/Symbol command
	void SendCommandSymbolControl(TIconId aIconId, TIconStatus aStatus);

    void RequestDeviceId();
    void RequestFirmwareRevision();
    void RequestPowerSupplyStatus();
	//
	void SetClockData();
	//
	void ResetBuffers();

private:
	///Off screen mode is the recommended default settings to avoid tearing.
	///Though turning it off can be useful for debugging
	bool iOffScreenMode;
	///We use this flag to align display clock seconds with system time
	bool iNeedAccurateClockData;
    //
	BitArrayLow* iFrameNext;
    BitArrayLow* iFrameCurrent;
    BitArrayLow* iFramePrevious;
    //
    BitArrayLow* iFrameAlpha; //owned
    BitArrayLow* iFrameBeta;  //owned
    BitArrayLow* iFrameGamma; //owned
	};



#endif
