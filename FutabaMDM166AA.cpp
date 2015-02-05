//
//
//

#include "FutabaMDM166AA.h"

#include <stdio.h>
#include <time.h>



static void sleep(unsigned int mseconds)
	{
    clock_t goal = mseconds + clock();
    while (goal > clock());
	}

//
// class MDM166AA
//

MDM166AA::MDM166AA():
    iOffScreenMode(true),
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
MDM166AA::~MDM166AA()
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
int MDM166AA::Open()
	{
	int success = HidDevice::Open(KTargaVendorId,KFutabaProductIdMDM166AA,NULL);
	if (success)
		{
        //Allocate both frames
        delete iFrameAlpha;
        iFrameAlpha=NULL;
        iFrameAlpha=new BitArrayLow(KMDM166AAFrameBufferPixelCount);
        //
        delete iFrameBeta;
        iFrameBeta=NULL;
        iFrameBeta=new BitArrayLow(KMDM166AAFrameBufferPixelCount);
        //
        delete iFrameGamma;
        iFrameGamma=NULL;
        iFrameGamma=new BitArrayLow(KMDM166AAFrameBufferPixelCount);
        //
        iFrameNext=iFrameAlpha;
        iFrameCurrent=iFrameBeta;
        iFramePrevious=iFrameGamma;
        //
		SetNonBlocking(1);
		//
		SendCommandReset();
		//
		ShowClock();

		}
	return success;
	}

/**
*/
void MDM166AA::SetPixel(unsigned char aX, unsigned char aY, unsigned int aPixel)
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
*/
/*
void MDM166AA::BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const
	{
	//TODO: amend loop values so that we don't keep on looping past our frame buffer dimensions.
	for (int i=0;i<aSrcWidth;i++)
		{
		for (int j=0;j<aSrcHeight;j++)
			{
            iFrameNext->SetBitValue((aTargetX+i)*HeightInPixels()+aTargetY+j,aBitmap[+i*aSrcHeight+j]);
			}
		}
	}
*/

/**
Clear our client side back buffer.
Call to SwapBuffers must follow to actually clear the display.
*/
void MDM166AA::Clear()
    {
	//That one also clear the symbols
    SetAllPixels(0x00);
    }

/**
Turn on all pixels.
Must be followed by a SwapBuffers call.
*/
void MDM166AA::Fill()
	{
	SetAllPixels(0xFF);
	}

/**
Set all pixels on our screen to the desired value.
This operation is performed off screen to avoid tearing.
@param 8 pixels pattern
*/
void MDM166AA::SetAllPixels(unsigned char aPattern)
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
void MDM166AA::SendCommandClear()
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
Put our off screen buffer on screen.
On screen buffer goes off screen.
*/
void MDM166AA::SwapBuffers()
	{
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
void MDM166AA::Request(TMiniDisplayRequest aRequest)
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
void MDM166AA::ResetBuffers()
	{
    //iNextFrame->ClearAll();
    //memset(iFrameAlpha,0x00,sizeof(iFrameAlpha));
	//memset(iFrameBeta,0x00,sizeof(iFrameBeta));
	}

/**
*/
void MDM166AA::RequestDeviceId()
    {
	//Not supported
    }

/**
*/
void MDM166AA::RequestFirmwareRevision()
    {
	//Not supported
    }

/**
*/
void MDM166AA::RequestPowerSupplyStatus()
    {
	//Not supported
    }


/**
This is for development purposes only.
Production application should stick to off-screen mode to avoid tearing.
*/
void MDM166AA::ToggleOffScreenMode()
	{
    SetOffScreenMode(!iOffScreenMode);
	}

/**
 * @brief MDM166AA::SetOffScreenMode
 * @param aOn
 * @return
 */
void MDM166AA::SetOffScreenMode(bool aOn)
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
void MDM166AA::SetBrightness(int aBrightness)
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
void MDM166AA::ShowClock()
	{
	SetClockData();
	SendCommandClockDisplay(EClockLarge,EClock24);
	}

/**
*/
void MDM166AA::HideClock()
	{
	//TODO: or reset
	Clear();
	}


/**
Clock setting 
[Code]1BH,00H,Pm,Ph 
[Function]Setting the clock data. The setting data is cleared, if the Reset command is input or power is turned off.
Ph = hour 
Pm = minute 
*/
void MDM166AA::SendCommandSetClockData(unsigned char aHour, unsigned char aMinute)
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
This needs to be redone whenever we open or turn on our display.
*/
void MDM166AA::SetClockData()
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
void MDM166AA::SendCommandClockDisplay(TClockSize aClockSize, TClockFormat aClockFormat)
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
*/
void MDM166AA::SendCommandReset()
	{
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x01; //Report length.
	report[2]=0x1F; //Command ID
	Write(report);
	//Wait until reset is done. Is that needed?
	//sleep(2000);
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
void MDM166AA::SendCommandSetAddressCounter(unsigned char aAddressCounter)
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
void MDM166AA::SendCommandWriteGraphicData(int aSize, unsigned char* aPixels)
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
