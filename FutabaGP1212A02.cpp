//
//
//

#include "FutabaGP1212A02.h"


const int KNumberOfFrameBeforeDiffAlgo = 3;

//
// class GP1212A02A
//

GP1212A02A::GP1212A02A():
	iDisplayPositionX(0),iDisplayPositionY(0),
    iOffScreenMode(true),
    iUseFrameDifferencing(true),
    iFrameNext(NULL),
    iFrameCurrent(NULL),
    iFramePrevious(NULL),
    iFrameAlpha(NULL),
    iFrameBeta(NULL),
    iFrameGamma(NULL),
    iNeedFullFrameUpdate(0),
    iPowerOn(false)
	{
	iDeviceId[0]=0;
	iFirmwareRevision[0]=0;
	//ResetBuffers();
	}

/**
*/
GP1212A02A::~GP1212A02A()
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
    //
    iNeedFullFrameUpdate=0;
	}

/**
*/
int GP1212A02A::Open()
	{
	int success = HidDevice::Open(KFutabaVendorId,KFutabaProductIdGP1212A02A,NULL);
	if (success)
		{
        //Allocate both frames
        delete iFrameAlpha;
        iFrameAlpha=NULL;
        iFrameAlpha=new BitArrayLow(KGP12xFrameBufferPixelCount);
        //
        delete iFrameBeta;
        iFrameBeta=NULL;
        iFrameBeta=new BitArrayLow(KGP12xFrameBufferPixelCount);
        //
        delete iFrameGamma;
        iFrameGamma=NULL;
        iFrameGamma=new BitArrayLow(KGP12xFrameBufferPixelCount);
        //
        iFrameNext=iFrameAlpha;
        iFrameCurrent=iFrameBeta;
        iFramePrevious=iFrameGamma;


        //To make sure it is synced properly
        iNeedFullFrameUpdate=0;
        //
		SetNonBlocking(1);
		//
		SendClearCommand();
		}
	return success;
	}

/**
*/
void GP1212A02A::SetPixel(unsigned char aX, unsigned char aY, bool aOn)
	{
	//
	//int byteOffset=(aX*HeightInPixels()+aY)/8;
	//int bitOffset=(aX*HeightInPixels()+aY)%8;
    //iNextFrame[byteOffset] |= ( (aOn?0x01:0x00) << bitOffset );

    if (iOffScreenMode)
        {
        if (aOn)
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
void GP1212A02A::BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const
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
void GP1212A02A::Clear()
    {
    //memset(iNextFrame->Ptr(),0x00,FrameBufferSizeInBytes());
    if (iOffScreenMode)
        {
        iFrameNext->ClearAll();
        }
    else
        {
        SendClearCommand();
        }
    }

/**
Turn on all pixels.
Must be followed by a SwapBuffers call.
*/
void GP1212A02A::Fill()
	{
	SetAllPixels(0xFF);
	}

/**
Set all pixels on our screen to the desired value.
This operation is performed off screen to avoid tearing.
@param 8 pixels pattern
*/
void GP1212A02A::SetAllPixels(unsigned char aPattern)
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
Using this function is advised against as is causes tearing.
Use Clear instead.
*/
void GP1212A02A::SetFrame(int aSize, unsigned char* aPixels)
{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=(aSize<=report.Size()-10?aSize+0x08:64); //Report length. -10 is for our header first 10 bytes. +8 is for our Futaba header size
    report[2]=0x1B; //Command ID
    report[3]=0x4A; //Command ID
    report[4]=0x30; //DW Display Window
    report[5]=0x00; //aL = DW lower byte
    report[6]=0x00; //aH = DW upper byte
    report[7]=0x30; //Direction of writing: Y
	report[8]=aSize; //Size of pixel data in bytes (LSB)
	report[9]=aSize>>8;	//Size of pixel data in bytes (MSB)
    int sizeWritten=MIN(aSize,report.Size()-10);
    memcpy(report.Buffer()+10, aPixels, sizeWritten);
    Write(report);

    int remainingSize=aSize;
    //We need to keep on sending our pixel data until we are done
    while (report[1]==64)
        {
        report.Reset();
        remainingSize-=sizeWritten;
        report[0]=0x00; //Report ID
        report[1]=(remainingSize<=report.Size()-2?remainingSize:64); //Report length, should be 64 or the remaining size
        sizeWritten=(report[1]==64?63:report[1]);
        memcpy(report.Buffer()+2, aPixels+(aSize-remainingSize), sizeWritten);
        Write(report);
        }
}

/**
Using this function is advised against as is causes tearing.
Use Clear instead.
*/
void GP1212A02A::SendClearCommand()
	{
    //1BH,4AH,43H,44H
    //Send Clear Display Command
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x04; //Report length
	report[2]=0x1B; //Command ID
	report[3]=0x4A; //Command ID
	report[4]=0x43; //Command ID
	report[5]=0x44; //Command ID
	Write(report);
	}


/**
Provide Y coordinate of our off screen buffer.
*/
unsigned char GP1212A02A::OffScreenY() const
	{
	//Overflowing is fine this is just what we want
	return iDisplayPositionY+HeightInPixels();
	}

/**
Put our off screen buffer on screen.
On screen buffer goes off screen.
*/
void GP1212A02A::SwapBuffers()
	{
	//Only perform buffer swapping if off screen mode is enabled
	if (OffScreenMode())
		{
		//Send host back buffer to device back buffer
		SetFrame(FrameBufferSizeInBytes(),iFrameNext->Ptr());

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
Translate the given pixel coordinate according to our off screen mode.
*/
void GP1212A02A::OffScreenTranslation(unsigned char& aX, unsigned char& aY)
	{
	if (OffScreenMode())
		{
		aX+=WidthInPixels()-iDisplayPositionX;
		aY+=HeightInPixels()-iDisplayPositionY;
		}
	}


/**
*/
void GP1212A02A::ResetBuffers()
	{
    //iNextFrame->ClearAll();
    //memset(iFrameAlpha,0x00,sizeof(iFrameAlpha));
	//memset(iFrameBeta,0x00,sizeof(iFrameBeta));
	}

/**
*/
void GP1212A02A::RequestDeviceId()
    {
	//Not supported
    }

/**
*/
void GP1212A02A::RequestFirmwareRevision()
    {
	//Not supported
    }

/**
*/
void GP1212A02A::RequestPowerSupplyStatus()
    {
	//Not supported
    }


/**
This is for development purposes only.
Production application should stick to off-screen mode to avoid tearing.
*/
void GP1212A02A::ToggleOffScreenMode()
	{
    SetOffScreenMode(!iOffScreenMode);
	}

/**
 * @brief GP1212A02A::SetOffScreenMode
 * @param aOn
 * @return
 */
void GP1212A02A::SetOffScreenMode(bool aOn)
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
Tries to complete our current request if we have one pending.
 */
TMiniDisplayRequest GP1212A02A::AttemptRequestCompletion()
    {
	//That display does not support any requests
	return EMiniDisplayRequestNone;
	}


/**
Set our screen brightness.
@param The desired brightness level. Must be between MinBrightness and MaxBrightness.
*/
void GP1212A02A::SetBrightness(int aBrightness)
    {
    if (aBrightness<MinBrightness()||aBrightness>MaxBrightness())
        {
        //Brightness out of range.
        //Just ignore that request.
        return;
        }

    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x04; //Report size
    report[2]=0x1B; //Command ID
    report[3]=0x4A; //Command ID
    report[4]=0x44; //Command ID
    report[5]=0x30+aBrightness; //Brightness level
    Write(report);
    }

/**
*/
bool GP1212A02A::PowerOn()
	{
	return iPowerOn;
	}

/**
*/
char* GP1212A02A::DeviceId()
	{
	return iDeviceId;
	}

/**
*/
char* GP1212A02A::FirmwareRevision()
	{
	return iFirmwareRevision;
	}
