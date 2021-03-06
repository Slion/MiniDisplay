//
// Copyright (C) 2014-2015 St�phane Lenclud.
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

#include "FutabaMDM140AA.h"

#include <stdio.h>
#include <time.h>


typedef void (MDM140AA::*TSetIconStatus) (int aIndex, int aStatus);

//TODO: Use a single array instead of those three, use a new class as array element

// Order matters
const TSetIconStatus KFunctionPerIcon[]=
	{
	NULL,	//EMiniDisplayIconNetworkSignal,
    &MDM140AA::SetIconInternet,			//EMiniDisplayInternet,
	NULL,	//EMiniDisplayIconEmail,
    &MDM140AA::SetIconMute,	//EMiniDisplayIconMute,
    &MDM140AA::SetIconVolume, //EMiniDisplayIconVolume,
	&MDM140AA::SetIconVolumeLabel,	//EMiniDisplayIconVolumeLabel,
	&MDM140AA::SetIconPlay,	//EMiniDisplayIconPlay,
	&MDM140AA::SetIconPause,	//EMiniDisplayIconPause,
	&MDM140AA::SetIconRecording,	//EMiniDisplayIconRecording
    &MDM140AA::SetIconRewind, //EMiniDisplayIconRewind,
    &MDM140AA::SetIconForward, //EMiniDisplayIconForward,
    &MDM140AA::SetIconDvd, //EMiniDisplayIconDvd,
    &MDM140AA::SetIconCd, //EMiniDisplayIconCd,
    &MDM140AA::SetIconVcd, // EMiniDisplayIconVcd,
	};

const int KMaxIconType = sizeof(KFunctionPerIcon)/sizeof(TSetIconStatus);

/**
Define how segments each of our icons have.
Order matters.
*/
const int KSegmentsPerIcon[]=
	{
	0,	// EMiniDisplayIconNetworkSignal,
	1,	// EMiniDisplayIconInternet,
    0,	// EMiniDisplayIconEmail,
    1,	// EMiniDisplayIconMute,
    12, // EMiniDisplayIconVolume,
	1,	// EMiniDisplayIconVolumeLabel,
	1,	// EMiniDisplayIconPlay,
	1,	// EMiniDisplayIconPause,
	1,	// EMiniDisplayIconRecording
    1,  // EMiniDisplayIconRewind
    1,  // EMiniDisplayIconForward,
    1,  // EMiniDisplayIconDvd,
    1,  // EMiniDisplayIconCd,
    1   // EMiniDisplayIconVcd,
	};

/**
Define how status each of our icon can assume.
Its typically two for On and Off status.
*/
const int KStatusPerIcon[]=
	{
	0,	// EMiniDisplayIconNetworkSignal,
	2,	// EMiniDisplayIconInternet,
    0,	// EMiniDisplayIconEmail,
    2,	// EMiniDisplayIconMute,
    3,  // EMiniDisplayIconVolume,
	2,	// EMiniDisplayIconVolumeLabel,
	2,	// EMiniDisplayIconPlay,
	2,	// EMiniDisplayIconPause,
	2,	// EMiniDisplayIconRecording
    2,  // EMiniDisplayIconRewind
    2,  // EMiniDisplayIconForward,
    2,  // EMiniDisplayIconDvd,
    2,  // EMiniDisplayIconCd,
    2   // EMiniDisplayIconVcd,
	};



static void sleep(unsigned int mseconds)
	{
    clock_t goal = mseconds + clock();
    while (goal > clock());
	}

//
// class MDM140AA
//

MDM140AA::MDM140AA():
    iOffScreenMode(true),
	iNeedAccurateClockData(false),
    iFrameNext(NULL),
    iFrameCurrent(NULL),
    iFramePrevious(NULL),
    iFrameAlpha(NULL),
    iFrameBeta(NULL),
    iFrameGamma(NULL)
	{
	iDeviceId[0]=0;
	iFirmwareRevision[0]=0;
	//ResetBuffers();
	}

/**
*/
MDM140AA::~MDM140AA()
	{
    delete iFrameAlpha;
    iFrameAlpha=NULL;
    //
    delete iFrameBeta;
    iFrameBeta=NULL;
    //
    delete iFrameGamma;
    iFrameGamma=NULL;
    //
    iFrameNext=NULL;
    iFrameCurrent=NULL;
    iFramePrevious=NULL;
	}

/**
*/
int MDM140AA::Open()
	{
	int success = HidDevice::Open(KVendorIdFujitsu, KProductIdFujitsuScaleoE,NULL);
	if (success)
		{
        //Allocate both frames
        delete iFrameAlpha;
        iFrameAlpha=NULL;
        iFrameAlpha=new BitArrayLow(KMDM140AAFrameBufferPixelCount);
        //
        delete iFrameBeta;
        iFrameBeta=NULL;
        iFrameBeta=new BitArrayLow(KMDM140AAFrameBufferPixelCount);
        //
        delete iFrameGamma;
        iFrameGamma=NULL;
        iFrameGamma=new BitArrayLow(KMDM140AAFrameBufferPixelCount);
        //
        iFrameNext=iFrameAlpha;
        iFrameCurrent=iFrameBeta;
        iFramePrevious=iFrameGamma;
        //
		SetNonBlocking(1);
		//
		SendCommandReset();
		
		//We will need accurate clock data
		//iNeedAccurateClockData=true;
		//Until we get it just use rough time instead
		//This is needed otherwise the clock won't work for the first minute.
		//It flashes the clock when opening the display but that's no big deal.
		ShowClock();
		SetClockData();

		//Turns mast ON
		//SetIconNetwork(0,EIconOn);
		//Show volume label
		//SendCommandSymbolControl(EIconVolumeLabel,EIconOn);
		//Icon checks
		//SetAllIcons(EIconOn);
		}
	return success;
	}

/**
*/
void MDM140AA::SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel)
	{
	//
	//int byteOffset=(aX*HeightInPixels()+aY)/8;
	//int bitOffset=(aX*HeightInPixels()+aY)%8;
    //iNextFrame[byteOffset] |= ( (aOn?0x01:0x00) << bitOffset );

	//Pixel is on if any of the non-alpha component is not null
	bool on = (aPixel&0x00FFFFFF)!=0x00000000;

    if (iOffScreenMode)
        {
        if (on)
            {
            iFrameNext->SetBit(aX*HeightInPixels()+aY);
            }
        else
            {
            iFrameNext->ClearBit(aX*HeightInPixels()+aY);
            }
        }
    else
        {
        //Just specify a one pixel block
        //TODO
        }
	}

/**
Clear our client side back buffer.
Call to SwapBuffers must follow to actually clear the display.
*/
void MDM140AA::Clear()
    {
	//That one also clear the symbols
    SetAllPixels(0x00);
	//SendCommandClear(); //Clear icons too
    }

/**
Turn on all pixels.
Must be followed by a SwapBuffers call.
*/
void MDM140AA::Fill()
	{
	SetAllPixels(0xFF);
	}

/**
Set all pixels on our screen to the desired value.
This operation is performed off screen to avoid tearing.
@param 8 pixels pattern
*/
void MDM140AA::SetAllPixels(unsigned char aPattern)
	{
	//With a single buffer
	//unsigned char screen[2048]; //One screen worth of pixels
	//memset(screen,0xFF,sizeof(screen));
	//SetPixelBlock(0,0,63,sizeof(screen),screen);


    if (iOffScreenMode)
        {
        memset(iFrameNext->Ptr(),aPattern,FrameBufferSizeInBytes());
        }
    else
        {
        //Using pattern SetPixelBlock variant.
        //TODO
        }
	//
	}






/**
Whole display RAM areas including invisible area are filled with 00H data.
(Include the symbol)
SL: Though there is no invisible area with that device.
*/
void MDM140AA::SendCommandClear()
	{
    //Send Clear Display Command
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x02; //Report length
	report[2]=0x1B; //Command ID
	report[3]=0x50; //Command ID
	Write(report);
	}

/**
Check if accurate clock data is needed and update display clock if system clock seconds are zero.
This is intended to be called every frame from our SwapBuffers function.
*/
void MDM140AA::AttemptClockSynchronization()
	{
	//Check if accurate clock data is needed
	if (!iNeedAccurateClockData)
		{
		return;
		}

	//Fetch local time
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//If our seconds are zero we synchronize our display clock
	if (timeinfo->tm_sec==0)
		{
		SendCommandSetClockData(timeinfo->tm_hour,timeinfo->tm_min);
		//Our clock is as accurate as it can for the time being
		iNeedAccurateClockData=false;
		}
	}

/**
Put our off screen buffer on screen.
On screen buffer goes off screen.
*/
void MDM140AA::SwapBuffers()
	{
	//We need to synchronize our clock seconds
	AttemptClockSynchronization();

	//Only perform buffer swapping if off screen mode is enabled
	if (OffScreenMode())
		{
		//Send next frame to our display RAM
		//We could attempt to implement a frame differencing algorithm much like we did for GP1212A01.
		//However we see little point doing that since we already run at above 20 FPS.
		SendCommandWriteGraphicData(FrameBufferSizeInBytes(),iFrameNext->Ptr());

        //Cycle through our frame buffers
        //We keep track of previous frame which is in fact our device back buffer.
        //We can then compare previous and next frame and send only the differences to our device.
        //This mechanism allows us to reduce traffic over our USB bus thus improving our frame rate from 14 FPS to 30 FPS.
        //Keep our previous frame pointer
        BitArrayLow* previousFrame=iFramePrevious;
        //Current frame becomes the previous one
        iFramePrevious = iFrameCurrent;
        //Next frame becomes the current one
        iFrameCurrent = iFrameNext;
        //Next frame is now our former previous
        iFrameNext = previousFrame;
		}
	}


//Define the edge of our pixel block
//Pixel blocks of 32x32 seems to run almost as fast as full screen update in worse case scenarii.
//Though I wonder if in some situations 16 could be better. Make this an attribute at some point if need be.
const int KPixelBlockEdge = 32;
const int KPixelBlockSizeInBits = KPixelBlockEdge*KPixelBlockEdge;
const int KPixelBlockSizeInBytes = KPixelBlockSizeInBits/8;


/**
*/
void MDM140AA::Request(TMiniDisplayRequest aRequest)
	{
	switch (aRequest)
		{
	case EMiniDisplayRequestDeviceId:
		RequestDeviceId();
		break;
	case EMiniDisplayRequestFirmwareRevision:
		RequestFirmwareRevision();
		break;
	case EMiniDisplayRequestPowerSupplyStatus:
		RequestPowerSupplyStatus();
		break;
	default:
		//Not supported
		break;
		};
	}


/**
*/
void MDM140AA::ResetBuffers()
	{
    //iNextFrame->ClearAll();
    //memset(iFrameAlpha,0x00,sizeof(iFrameAlpha));
	//memset(iFrameBeta,0x00,sizeof(iFrameBeta));
	}

/**
*/
void MDM140AA::RequestDeviceId()
    {
	//Not supported
    }

/**
*/
void MDM140AA::RequestFirmwareRevision()
    {
	//Not supported
    }

/**
*/
void MDM140AA::RequestPowerSupplyStatus()
    {
	//Not supported
    }


/**
This is for development purposes only.
Production application should stick to off-screen mode to avoid tearing.
*/
void MDM140AA::ToggleOffScreenMode()
	{
    SetOffScreenMode(!iOffScreenMode);
	}

/**
 * @brief MDM140AA::SetOffScreenMode
 * @param aOn
 * @return
 */
void MDM140AA::SetOffScreenMode(bool aOn)
    {
    if (aOn==iOffScreenMode)
    {
        //Nothing to do here
        return;
    }

    iOffScreenMode=aOn;

    //Clean up our buffers upon switching modes
    Clear();
    SwapBuffers();
    Clear();
    }

/**
Set our screen brightness.
@param The desired brightness level. Must be between MinBrightness and MaxBrightness.
*/
void MDM140AA::SetBrightness(int aBrightness)
    {
    if (aBrightness<MinBrightness()||aBrightness>MaxBrightness())
        {
        //Brightness out of range.
        //Just ignore that request.
        return;
        }

    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x03; //Report size
    report[2]=0x1B; //Command ID
    report[3]=0x40; //Command ID
    report[4]=aBrightness; //Brightness level
    Write(report);
    }


/**
*/
void MDM140AA::ShowClock()
	{
	//Assuming display clock is at least roughly set since we do it when opening our display connection.
	//We will need accurate clock data next we get a chance.
	//This should guarantee that if our display remain open for weeks our clock will be synchronized whenever we switch back from clock mode to render mode.
	iNeedAccurateClockData=true;
	//Show clock using specified styles
	SendCommandClockDisplay(EClockLarge,EClock24);
	}

/**
*/
void MDM140AA::HideClock()
	{
	SetAllPixels(0x00);
	SwapBuffers();
	}

/**
*/
int MDM140AA::IconCount(TMiniDisplayIconType aIcon)
	{
	return KSegmentsPerIcon[aIcon];
	}

int MDM140AA::IconStatusCount(TMiniDisplayIconType aIcon)
	{
	return KStatusPerIcon[aIcon];
	}

void MDM140AA::SetIconStatus(TMiniDisplayIconType aIcon, int aIndex, int aStatus)
	{
	if (aIcon<0||aIcon>=KMaxIconType||(KFunctionPerIcon[aIcon]==NULL))
		{
		//Out of range or no function pointer for that icon
		return;
		}

	(this->*KFunctionPerIcon[aIcon])(aIndex,aStatus);
	}

/**
*/
void MDM140AA::SetIconDvd(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconDvd])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconDvd),(aStatus==0?EIconOff:EIconOn));
	}

/**
*/
void MDM140AA::SetIconRewind(int aIndex, int aStatus)
{
    if (aIndex < 0 || aIndex >= KSegmentsPerIcon[EMiniDisplayIconRewind])
    {
        //Out of range
        return;
    }

    SendCommandSymbolControl((TIconId)(aIndex + EIconRewind), (aStatus == 0 ? EIconOff : EIconOn));
}

/**
*/
void MDM140AA::SetIconForward(int aIndex, int aStatus)
{
    if (aIndex < 0 || aIndex >= KSegmentsPerIcon[EMiniDisplayIconForward])
    {
        //Out of range
        return;
    }

    SendCommandSymbolControl((TIconId)(aIndex + EIconForward), (aStatus == 0 ? EIconOff : EIconOn));
}

/**
*/
void MDM140AA::SetIconInternet(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconInternet])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+ EIconInternet),(aStatus==0?EIconOff:EIconOn));
	}

/**
*/
void MDM140AA::SetIconCd(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconCd])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconCd),(aStatus==0?EIconOff:EIconOn));
	}

/**
*/
void MDM140AA::SetIconVcd(int aIndex, int aStatus)
{
    if (aIndex < 0 || aIndex >= KSegmentsPerIcon[EMiniDisplayIconVcd])
    {
        //Out of range
        return;
    }

    SendCommandSymbolControl((TIconId)(aIndex + EIconCd), (aStatus == 0 ? EIconOff : EIconOn));
    SendCommandSymbolControl((TIconId)(aIndex + EIconVcd), (aStatus == 0 ? EIconOff : EIconOn));
}



/**
*/
void MDM140AA::SetIconMute(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconMute])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconMute),(aStatus==0?EIconOff:EIconOn));
	}

/**
*/
void MDM140AA::SetIconVolume(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconVolume])
		{
		//Out of range
		return;
		}

	if (aStatus<EIconOff)
		{
		//Assuming we just want to turn it off then
		aStatus=EIconOff;
		}

	//Make sure we cap at our highest status value
	aStatus = MIN(EIconOn,aStatus);

	SendCommandSymbolControl((TIconId)(aIndex+EIconVolumeLevel01),(TIconStatus)aStatus);
	}


/**
*/
void MDM140AA::SetIconVolumeLabel(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconMute])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconVolumeLabel),(aStatus==0?EIconOff:EIconOn));
	}


/**
*/
void MDM140AA::SetIconPlay(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconPlay])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconPlay),(aStatus==0?EIconOff:EIconOn));
	}


/**
*/
void MDM140AA::SetIconPause(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconPause])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconPause),(aStatus==0?EIconOff:EIconOn));
	}


/**
*/
void MDM140AA::SetIconRecording(int aIndex, int aStatus)
	{
	if (aIndex<0||aIndex>=KSegmentsPerIcon[EMiniDisplayIconRecording])
		{
		//Out of range
		return;
		}

	SendCommandSymbolControl((TIconId)(aIndex+EIconRecording),(aStatus==0?EIconOff:EIconOn));
	}

/**
Set all our icons to the corresponding status.
*/
void MDM140AA::SetAllIcons(TIconStatus aStatus)
	{
	for (int i=EIconFirst;i<=EIconLast;i++)
		{
		SendCommandSymbolControl((TIconId)i,aStatus);
		}
	}

/**
Symbols control
Segment On/Off and Grayscale/Brightness
[Code]1BH,30H,Ps,Pb
*/
void MDM140AA::SendCommandSymbolControl(TIconId aIconId, TIconStatus aStatus)
	{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x04; //Report size
    report[2]=0x1B; //Command ID
    report[3]=0x30; //Command ID
	report[4]=aIconId;
	report[5]=aStatus;

    Write(report);
	}


/**
Clock setting 
[Code]1BH,00H,Pm,Ph 
[Function]Setting the clock data. The setting data is cleared, if the Reset command is input or power is turned off.
Ph = hour 
Pm = minute 
*/
void MDM140AA::SendCommandSetClockData(unsigned char aHour, unsigned char aMinute)
	{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x04; //Report size
    report[2]=0x1B; //Command ID
    report[3]=0x00; //Command ID

	//Minutes and Hours needs to be in hexadecimal view
	//To get 21:59 you need to pass in 0x21:0x59
	//Weirdest format ever, I know 
	report[4]=(aMinute/10*16)+aMinute%10;
	report[5]=(aHour/10*16)+aHour%10;

    Write(report);
	}

/**
Set display clock data according to local system time.
This will only provide 30s accuracy.
In fact display clock seconds are set to zero whenever clock data is set.
So you would only get second accuracy if this function was called when system time is at zero second.
It's the responsibility of AttemptClockSynchronization function to obtain second accuracy.
The present function is intended to provide only rough clock synchronization.

@note Unfortunately this command also turns on clock display.
*/
void MDM140AA::SetClockData()
	{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	//Adjust minute as best as we can so that we have a 30 seconds offset at most rather a than a full minute.
	if (timeinfo->tm_sec>30)
		{
		//Use the next minute then
		timeinfo->tm_min++;
		if (timeinfo->tm_min==60)
			{
			//Use the next hour then
			timeinfo->tm_hour++;
			timeinfo->tm_min=0;
			if (timeinfo->tm_hour==24)
				{
				//Move to the next day then
				timeinfo->tm_hour=0;
				}
			}
		}

	//Send hours and minutes to our display
	SendCommandSetClockData(timeinfo->tm_hour,timeinfo->tm_min);
	}


/**
Clock display
[Code] 1BH,Ps,aL,aH,Pf
[Function] Clock is displayed small or big.
*/
void MDM140AA::SendCommandClockDisplay(TClockSize aClockSize, TClockFormat aClockFormat)
	{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x03; //Report size
    report[2]=0x1B; //Command ID
    report[3]=aClockSize; //
    report[4]=aClockFormat; //

    Write(report);
	}


/**
Display RAM filled with 00H.
Address Counter is set by 00H.
Dimming is set to 50%.
Turn off all icons segments.

This does not reset our clock settings.
*/
void MDM140AA::SendCommandReset()
	{
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x01; //Report length.
	report[2]=0x1F; //Command ID
	Write(report);
	}


/**
Set Address Counter (AC) values: 1BH + 60H + xxH
xxH: 00 ~ BFH
AC value represents the start address for graphic data.
There are 192 bytes as display RAM. It can be set on anywhere even if AC value is not visible area.
The default value is 00H.
Default: 00H
When clock is displayed, AC value is set 00H.
*/
void MDM140AA::SendCommandSetAddressCounter(unsigned char aAddressCounter)
	{
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x03; //Report length.
	report[2]=0x1B; //Command ID
	report[3]=0x60; //Command ID
	report[4]=aAddressCounter;
	Write(report);
	}


/**
Set the defined pixel block to the given value.

@param The size of our pixel data. Number of pixels divided by 8.
@param Pointer to our pixel data.
*/
void MDM140AA::SendCommandWriteGraphicData(int aSize, unsigned char* aPixels)
    {
	//TODO: Remove that at some point
	SendCommandSetAddressCounter(0);

	const int KMaxPixelBytes=48;
	const int KHeaderSize=3;
	
	int remainingSize=aSize;
	int sizeWritten=0;

	while (remainingSize>0)
		{
		//Only send a maximum of 48 bytes worth of pixels per report
		const int KPixelDataSize=(remainingSize<=KMaxPixelBytes?remainingSize:KMaxPixelBytes);

		FutabaVfdReport report;
		report[0]=0x00; //Report ID
		report[1]=KPixelDataSize+KHeaderSize; //Report length. +3 is for our header first 3 bytes.
		report[2]=0x1B; //Command ID
		report[3]=0x70; //Command ID
		report[4]=KPixelDataSize; //Size of pixel data in bytes		
		memcpy(report.Buffer()+5, aPixels+sizeWritten, KPixelDataSize);
		Write(report);
		//Advance
		sizeWritten+=KPixelDataSize;
		remainingSize-=KPixelDataSize;
		}
    }
