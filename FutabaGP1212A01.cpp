//
//
//

#include "FutabaGP1212A01.h"

const int KNumberOfFrameBeforeDiffAlgo = 3;

//
// class GP1212A01A
//

GP1212A01A::GP1212A01A():
	iDisplayPositionX(0),iDisplayPositionY(0),
    iOffScreenMode(true),
    iUseFrameDifferencing(true),
    iFrameNext(NULL),
    iFrameCurrent(NULL),
    iFramePrevious(NULL),
    iFrameAlpha(NULL),
    iFrameBeta(NULL),
    iFrameGamma(NULL),
    iNeedFullFrameUpdate(0)
	{
	//ResetBuffers();
	}

/**
*/
GP1212A01A::~GP1212A01A()
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
int GP1212A01A::Open()
	{
	int success = HidDevice::Open(KFutabaVendorId,KFutabaProductIdGP1212A01A,NULL);
	if (success)
		{
        //Allocate both frames
        delete iFrameAlpha;
        iFrameAlpha=NULL;
        iFrameAlpha=new BitArray(KGP12xFrameBufferPixelCount);
        //
        delete iFrameBeta;
        iFrameBeta=NULL;
        iFrameBeta=new BitArray(KGP12xFrameBufferPixelCount);
        //
        delete iFrameGamma;
        iFrameGamma=NULL;
        iFrameGamma=new BitArray(KGP12xFrameBufferPixelCount);
        //
        iFrameNext=iFrameAlpha;
        iFrameCurrent=iFrameBeta;
        iFramePrevious=iFrameGamma;


        //To make sure it is synced properly
        iNeedFullFrameUpdate=0;
        //
		SetNonBlocking(1);
        //Since we can't get our display position we force it to our default
		//This makes sure frames are in sync from the start
        //Clever clients will have taken care of putting back frame (0,0) before closing
		SetDisplayPosition(iDisplayPositionX,iDisplayPositionY);
		}
	return success;
	}

/**
*/
void GP1212A01A::SetPixel(unsigned char aX, unsigned char aY, bool aOn)
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
        SetPixelBlock(aX,aY,0x00,0x01,aOn);
        }
	}

/**
*/
void GP1212A01A::BitBlit(const BitArray& aBitmap, int aSrcWidth, int aSrcHeight, int aTargetX, int aTargetY) const
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

/**
Clear our client side back buffer.
Call to SwapBuffers must follow to actually clear the display.
*/
void GP1212A01A::Clear()
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
void GP1212A01A::Fill()
	{
	SetAllPixels(0xFF);
	}

/**
Set all pixels on our screen to the desired value.
This operation is performed off screen to avoid tearing.
@param 8 pixels pattern
*/
void GP1212A01A::SetAllPixels(unsigned char aPattern)
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
        SetPixelBlock(0,0,63,FrameBufferSizeInBytes(),aPattern);
        }
	//
	}


/**
Set the defined pixel block to the given value.
@param X coordinate of our pixel block starting point.
@param Y coordinate of our pixel block starting point.
@param The height of our pixel block.
@param The size of our pixel data. Number of pixels divided by 8.
@param The value set to 8 pixels used as a pattern.
*/
void GP1212A01A::SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char aValue)
	{
	OffScreenTranslation(aX,aY);
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=(aSize<=report.Size()-10?aSize+0x08:64); //Report length. -10 is for our header first 10 bytes. +8 is for our Futaba header size
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=0xF0; //Command ID
    report[5]=aX;   //X
    report[6]=aY;   //Y
    report[7]=aHeight; //Y length before return. Though outside the specs, setting this to zero apparently allows us to modify a single pixel without touching any other.
	report[8]=aSize>>8; //Size of pixel data in bytes (MSB)
	report[9]=aSize;	//Size of pixel data in bytes (LSB)
    int sizeWritten=MIN(aSize,report.Size()-10);
    memset(report.Buffer()+10, aValue, sizeWritten);
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
        memset(report.Buffer()+2, aValue, sizeWritten);
        Write(report);
        }
	}

/**
Set the defined pixel block to the given value.
@param X coordinate of our pixel block starting point.
@param Y coordinate of our pixel block starting point.
@param The height of our pixel block.
@param The size of our pixel data. Number of pixels divided by 8.
@param Pointer to our pixel data.
*/
void GP1212A01A::SetPixelBlock(unsigned char aX, unsigned char aY, int aHeight, int aSize, unsigned char* aPixels)
    {
	OffScreenTranslation(aX,aY);
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=(aSize<=report.Size()-10?aSize+0x08:64); //Report length. -10 is for our header first 10 bytes. +8 is for our Futaba header size
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=0xF0; //Command ID
    report[5]=aX;   //X
    report[6]=aY;   //Y
    report[7]=aHeight; //Y length before return. Though outside the specs, setting this to zero apparently allows us to modify a single pixel without touching any other.
	report[8]=aSize>>8; //Size of pixel data in bytes (MSB)
	report[9]=aSize;	//Size of pixel data in bytes (LSB)
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
void GP1212A01A::SendClearCommand()
	{
    //1BH,5BH,32H,4AH
    //Send Clear Display Command
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x04; //Report length
	report[2]=0x1B; //Command ID
	report[3]=0x5B; //Command ID
	report[4]=0x32; //Command ID
	report[5]=0x4A; //Command ID
	Write(report);
	}

/**
Change our display position within our buffer.
*/
void GP1212A01A::SetDisplayPosition(DW aDw,unsigned char aX, unsigned char aY)
    {
    //1BH,5BH,Dw,Px,Py
    //Send Display Position Settings Command
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x05; //Report length
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=aDw;  //Specify our DW
    report[5]=aX;   //X coordinate of our DW top-left corner
    report[6]=aY;   //Y coordinate of our DW top-left corner
    Write(report);
    }

/**
Change our display position within our buffer.
*/
void GP1212A01A::SetDisplayPosition(unsigned char aX, unsigned char aY)
	{
	//Specs apparently says both DW should remain the same
	//Just don't ask
    SetDisplayPosition(GP1212A01A::DW1,aX,aY);
    SetDisplayPosition(GP1212A01A::DW2,aX,aY);
	iDisplayPositionX=aX;
	iDisplayPositionY=aY;
	}

/**
Provide Y coordinate of our off screen buffer.
*/
unsigned char GP1212A01A::OffScreenY() const
	{
	//Overflowing is fine this is just what we want
	return iDisplayPositionY+HeightInPixels();
	}

/**
Put our off screen buffer on screen.
On screen buffer goes off screen.
*/
void GP1212A01A::SwapBuffers()
	{
	//Only perform buffer swapping if off screen mode is enabled
	if (OffScreenMode())
		{
		//Send host back buffer to device back buffer
        if (!iUseFrameDifferencing || iNeedFullFrameUpdate<KNumberOfFrameBeforeDiffAlgo)
            {
            iNeedFullFrameUpdate++;
            SetPixelBlock(0,0,63,FrameBufferSizeInBytes(),iFrameNext->Ptr());
            }
        else
            {
            //Frame diff algo is enabled
            //We are going to send to our device only the differences between next frame and previous frame
            SendModifiedPixelBlocks();
            }
		//Swap device front and back buffer
		SetDisplayPosition(iDisplayPositionX,OffScreenY());

        //Cycle through our frame buffers
        //We keep track of previous frame which is in fact our device back buffer.
        //We can then compare previous and next frame and send only the differences to our device.
        //This mechanism allows us to reduce traffic over our USB bus thus improving our frame rate from 14 FPS to 30 FPS.
        //Keep our previous frame pointer
        BitArray* previousFrame=iFramePrevious;
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
 * @brief GP1212A01A::SendModifiedPixelBlocks
 * Compare our back and front buffer and send to the device only the modified pixels.
 */
void GP1212A01A::SendModifiedPixelBlocks()
    {
    int w=WidthInPixels();
    int h=HeightInPixels();


    //TODO: optimize with memcmp and 16 inc
    /*

    for (int i=0;i<w;i++)
        {
        for (int j=0;j<h;j++)
            {
            //aX*HeightInPixels()+aY
            if ((*iFrameNext)[i*h+j]!=(*iFramePrevious)[i*h+j])
                {
                //We need to update that pixel
                SetPixelBlock(i,j,0,1,((*iFrameNext)[i*h+j]?0x01:0x00));
                //SetDisplayPosition(iDisplayPositionX,OffScreenY());
                //SetDisplayPosition(iDisplayPositionX,OffScreenY());

                //SetPixelBlock(i,j,15,32,iNextFrame->Ptr()+offset);
                }
            }
        }
    */

    BitArray nextBlock(KPixelBlockSizeInBits);
    BitArray previousBlock(KPixelBlockSizeInBits);

    for (int i=0;i<w;i+=KPixelBlockEdge)
        {
        for (int j=0;j<h;j+=KPixelBlockEdge)
            {
            //aX*HeightInPixels()+aY
            //int offset=(i*w/8)+(j/8);

#ifdef DEBUG_FRAME_DIFF
            QImage imagePrevious(KPixelBlockEdge,KPixelBlockEdge,QImage::Format_RGB32);
            QImage imageNext(KPixelBlockEdge,KPixelBlockEdge,QImage::Format_RGB32);
#endif

            //Get both our blocks from our buffers
            for (int x=i;x<i+KPixelBlockEdge;x++)
                {
                for (int y=j;y<j+KPixelBlockEdge;y++)
                    {
                    int blockOffset=(x-i)*KPixelBlockEdge+(y-j);
                    int frameOffset=x*h+y;
                    nextBlock.SetBitValue(blockOffset,(*iFrameNext)[frameOffset]);
                    previousBlock.SetBitValue(blockOffset,(*iFramePrevious)[frameOffset]);

#ifdef DEBUG_FRAME_DIFF
                    imageNext.setPixel(x-i,y-j,(nextBlock[blockOffset]?0xFFFFFFFF:0x00000000));
                    imagePrevious.setPixel(x-i,y-j,(previousBlock[blockOffset]?0xFFFFFFFF:0x00000000));
#endif
                    }
                }

#ifdef DEBUG_FRAME_DIFF
            QString previousName;
            QString nextName;
            QTextStream(&previousName) << "p" << i << "x" << j << ".png";
            QTextStream(&nextName) << "n" << i << "x" << j << ".png";
            imagePrevious.save(previousName);
            imageNext.save(nextName);
#endif


            //if (memcmp(iFrameNext->Ptr()+offset,iFramePrevious->Ptr()+offset,32 )) //32=(16*16/8)
            if (memcmp(nextBlock.Ptr(),previousBlock.Ptr(),KPixelBlockSizeInBytes)!=0)
                {
                //We need to update that block
                SetPixelBlock(i,j,KPixelBlockEdge-1,KPixelBlockSizeInBytes,nextBlock.Ptr());
                //SetPixelBlock(i,j,15,32,0xFF/*nextBlock.Ptr()*/);
                //SetDisplayPosition(iDisplayPositionX,OffScreenY());
                //SetDisplayPosition(iDisplayPositionX,OffScreenY());

                //SetPixelBlock(i,j,15,32,iFrameNext->Ptr()+offset);
                }
            }
        }

    }

/**
Translate the given pixel coordinate according to our off screen mode.
*/
void GP1212A01A::OffScreenTranslation(unsigned char& aX, unsigned char& aY)
	{
	if (OffScreenMode())
		{
		aX+=WidthInPixels()-iDisplayPositionX;
		aY+=HeightInPixels()-iDisplayPositionY;
		}
	}


/**
*/
void GP1212A01A::ResetBuffers()
	{
    //iNextFrame->ClearAll();
    //memset(iFrameAlpha,0x00,sizeof(iFrameAlpha));
	//memset(iFrameBeta,0x00,sizeof(iFrameBeta));
	}


/**
*/
void GP1212A01A::Request(TMiniDisplayRequest aRequest)
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
void GP1212A01A::RequestDeviceId()
    {
    if (RequestPending())
        {
        //Abort silently for now
        return;
        }

    //1BH,5BH,63H,49H,44H
    //Send Read ID command
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x05; //Report length
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=0x63; //Command ID
    report[5]=0x49; //Command ID
    report[6]=0x44; //Command ID
    if (Write(report)==report.Size())
        {
        SetRequest(EMiniDisplayRequestDeviceId);
        }
    }

/**
*/
void GP1212A01A::RequestFirmwareRevision()
    {
    if (RequestPending())
        {
        //Abort silently for now
        return;
        }

    //1BH,5BH,63H,46H,52H
    //Send Software Revision Read Command
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x05; //Report length
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=0x63; //Command ID
    report[5]=0x46; //Command ID
    report[6]=0x52; //Command ID
    if (Write(report)==report.Size())
        {
        SetRequest(EMiniDisplayRequestFirmwareRevision);
        }
    }

/**
*/
void GP1212A01A::RequestPowerSupplyStatus()
    {
    if (RequestPending())
        {
        //Abort silently for now
        return;
        }
    //1BH,5BH,63H,50H,4DH
    //Send Power Suppply Monitor Command
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x05; //Report length
    report[2]=0x1B; //Command ID
    report[3]=0x5B; //Command ID
    report[4]=0x63; //Command ID
    report[5]=0x50; //Command ID
    report[6]=0x4D; //Command ID
    if (Write(report)==report.Size())
        {
        SetRequest(EMiniDisplayRequestPowerSupplyStatus);
        }
    }


/**
This is for development purposes only.
Production application should stick to off-screen mode to avoid tearing.
*/
void GP1212A01A::ToggleOffScreenMode()
	{
    SetOffScreenMode(!iOffScreenMode);
	}

/**
 * @brief GP1212A01A::SetOffScreenMode
 * @param aOn
 * @return
 */
void GP1212A01A::SetOffScreenMode(bool aOn)
    {
    if (aOn==iOffScreenMode)
    {
        //Nothing to do here
        return;
    }

    iOffScreenMode=aOn;

    //Clean up our buffers upon switching modes
    SetDisplayPosition(0,0);
    Clear();
    SwapBuffers();
    Clear();
    }

/**
Tries to complete our current request if we have one pending.
 */
TMiniDisplayRequest GP1212A01A::AttemptRequestCompletion()
    {
    if (!RequestPending())
        {
        return EMiniDisplayRequestNone;
        }

    int res=Read(iInputReport);

    if (!res)
        {
        return EMiniDisplayRequestNone;
        }

    //Process our request
    if (CurrentRequest()==EMiniDisplayRequestPowerSupplyStatus)
        {
        if (iInputReport[1]==0x4F && iInputReport[2]==0x4E)
            {
            iPowerOn = true;
            }
        else if (iInputReport[1]==0x4F && iInputReport[2]==0x46 && iInputReport[3]==0x46)
            {
            iPowerOn = false;
            }
        }
	else if (CurrentRequest()==EMiniDisplayRequestDeviceId)
		{
			unsigned char* ptr=&iInputReport[1];
			strcpy(iDeviceId,(const char*)ptr);
		}
	else if (CurrentRequest()==EMiniDisplayRequestFirmwareRevision)
		{
			unsigned char* ptr=&iInputReport[1];
			strcpy(iFirmwareRevision,(const char*)ptr);
		}

    TMiniDisplayRequest completed=CurrentRequest();
    //Our request was completed
    SetRequest(EMiniDisplayRequestNone);

    return completed;
    }


/**
Set our screen brightness.
@param The desired brightness level. Must be between MinBrightness and MaxBrightness.
*/
void GP1212A01A::SetBrightness(int aBrightness)
    {
    if (aBrightness<MinBrightness()||aBrightness>MaxBrightness())
        {
        //Brightness out of range.
        //Just ignore that request.
        return;
        }

    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x06; //Report size
    report[2]=0x1B; //Command ID
    report[3]=0x5C; //Command ID
    report[4]=0x3F; //Command ID
    report[5]=0x4C; //Command ID
    report[6]=0x44; //Command ID
    report[7]=0x30+aBrightness; //Brightness level
    Write(report);
    }


