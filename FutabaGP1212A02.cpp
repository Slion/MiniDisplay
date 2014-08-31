//
//
//

#include "FutabaGP1212A02.h"


const int KNumberOfFrameBeforeDiffAlgo = 3;

const unsigned short KMaxDataMemoryAddress = 0x4FFF;
const unsigned short KFrameSizeInBytes = 0x800;

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

		//Setup BMP box
		BmpBoxSetting(EBmpBoxIdOne,0x0000,256,64);

		//Select current BMP box
		BmpBoxSelect(EBmpBoxIdOne);

		iNextFrameAddress = 0x0000;

		}
	return success;
	}

/**
 Setting the BMP box
[Code] 1BH,5CH,42H,Pn,aL,aH,Pw,Ph
[Function] Setting the BMP box. BMP box can be defined the 3 area to DW. The position of BMP 
box is set based on the address of DW. 
* To write data in BMP box, BMP box select is necessary. 
* Specifiable horizontal size is 256dot (100H) MAX. If horizontal size specify 256dot, Pw = 00H 
Pn = Number of a BMP box 
aL = Lower byte of address 
aH = Upper byte of address 
Pw = BMP box width 
Ph = BMP box height 

[Definable area]
Pn = 31H - BMP box 1
Pn = 32H - BMP box 2
Pn = 33H - BMP box 3
0000H <= aL + aH * 100 <= 07FFH 
01H <= Pw <= 00H (=100H) 
01H <= Ph <= 08H
*/
void GP1212A02A::BmpBoxSetting(TBmpBoxId aBoxId, unsigned short aAddress, int aWidth, int aHeight)
	{
	//TODO: check parameters validity
	//1BH,5CH,42H,Pn,aL,aH,Pw,Ph
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x08; //Report length.
	report[2]=0x1B; //Command ID
	report[3]=0x5C; //Command ID
	report[4]=0x42; //Command ID
	report[5]=aBoxId; 
	report[6]=(unsigned char)aAddress; //aL = DM lower byte
	report[7]=aAddress>>8; //aH = DM upper byte
	report[8]=(aWidth==256?0x00:aWidth); //Pw = BMP box width 00==256
	report[9]=aHeight/8; //Ph = BMP box height.
	Write(report);
	}

/**
[Code]1BH,5CH,48H,Pn
[Function]Select of BMP box 
* Execution "BMP box select" is necessary before "Setting the Text box". 
* In case of writing by the specified dot writing, it is necessary to cancel this command. 
[Definable area]
Pn = 30H - Remove the BMP box 
Pn = 31H - BMP box 1
Pn = 32H - BMP box 2
Pn = 33H - BMP box 3
*/
void GP1212A02A::BmpBoxSelect(TBmpBoxId aBoxId)
	{
	//TODO: check parameters validity 
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
	report[1]=0x04; //Report length.
	report[2]=0x1B; //Command ID
	report[3]=0x5C; //Command ID
	report[4]=0x48; //Command ID
	report[5]=aBoxId; //BMP box ID
	Write(report);
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
BMP data input 
[Code] 1BH,4AH,Pm,aL,aH,Ps,nL,nH,Pd...Pd
[Function] The BMP data is written in the DW(Display Window) or the Data memory. 
Pm= DW or Data memory 
aL = DW lower byte 
aH = DW upper byte 
Ps = Direction of writing 
nL = number of BMP data length lower byte 
nH = number of BMP data length upper byte 
Pd = BMP data 
* If X direction is selected as Ps and data is written in the last address, the data in the last address is 
overwritten with the remaining data.  
[Definable area] Pm = 30H : DW
 Pm = 31H: Data memory 
0000H <= aL + aH * 100 <= 07FFH (DW)
0000H <= aL + aH * 100 <= 4FFFH (Data memory) 
Ps = 30H: Y direction 
Ps = 31H: X direction 
0001H <= nL + nH * 100 <= 0100H(DW: X direction) 
0001H <= nL + nH * 100 <= 0800H(DW: Y direction) 
0001H <= nL + nH * 100 <= 0A00H(Data memory: X direction) 
0001H <= nL + nH * 100 <= 5000H(Data memory: Y direction) 
*/
void GP1212A02A::BmpDataInput(TTarget aTarget, unsigned short aAddress, TDirection aDirection, unsigned short aSize, unsigned char* aPixels)
{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=(aSize<=report.Size()-10?aSize+0x08:64); //Report length. -10 is for our header first 10 bytes. +8 is for our Futaba header size
    report[2]=0x1B; //Command ID
    report[3]=0x4A; //Command ID
    report[4]=aTarget; //Display Window or Data Memory
    report[5]=(unsigned char)aAddress; //aL = DW lower byte
    report[6]=aAddress>>8; //aH = DW upper byte
    report[7]=aDirection; //Direction of writing: Y or X
	report[8]=(unsigned char)aSize; //Size of pixel data in bytes (LSB)
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
Data memory transfer
[Code] 1BH,5CH,44H,aL,aH
[Function] BMP data transfer from Data memory to DW. 
Although source data is updated, data in BMP box is not updated. To reflect the update, 
re-executing this command is necessary. 
aL = Lower byte of address 
aH = Upper byte of address 
[Definable area]
0000H <= aL + aH * 100 <= 4FFFH 
*/
void GP1212A02A::BmpBoxDataMemoryTransfer(unsigned short aAddress)
	{
	FutabaVfdReport report;
	report[0]=0x00; //Report ID
    report[1]=0x05; //Report length.
    report[2]=0x1B; //Command ID
    report[3]=0x5C; //Command ID
    report[4]=0x44; //Command ID
    report[5]=(unsigned char)aAddress; //aL = DM lower byte
    report[6]=aAddress>>8; //aH = DM upper byte
	Write(report);
	}

/**
Input BMP data in the BMP box 
[Code] 1BH,5CH,5DH,nL,nH,Pd...Pd
[Function] BMP data is written the BMP box 
* Number of definable data is due to BMP box size. If the data is over range, the over range data is 
rewritten the final address. 
nL = Lower byte of number of definition byte 
nH = Upper byte of number of definition byte 
Pd = BMP data 
[Definable area] Pn : BMP box size (Pw * Ph)
*/
void GP1212A02A::BmpBoxDataInput(unsigned short aSize, unsigned char* aPixels)
	{
	FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=(aSize<=report.Size()-7?aSize+0x05:64); //Report length. -7 is for our header first 10 bytes. +5 is for our Futaba header size
    report[2]=0x1B; //Command ID
    report[3]=0x5C; //Command ID
    report[4]=0x5D; //Display Window or Data Memory
	report[5]=(unsigned char)aSize; //Size of pixel data in bytes (LSB)
	report[6]=aSize>>8;	//Size of pixel data in bytes (MSB)
    int sizeWritten=MIN(aSize,report.Size()-7);
    memcpy(report.Buffer()+7, aPixels, sizeWritten);
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
		//Send pixel directly into BMP box
		//BmpBoxDataInput(FrameBufferSizeInBytes(),iFrameNext->Ptr());
		//Send pixel data directly into the display window
		//BmpDataInput(ETargetDisplayWindow,0x0000,EDirectionY, FrameBufferSizeInBytes(),iFrameNext->Ptr());
		//Send pixel data first to Data Memory then copy into the selected BMP box	
		//BmpDataInput(ETargetDataMemory,0x0000,EDirectionY, FrameBufferSizeInBytes(),iFrameNext->Ptr());
		//BmpBoxDataMemoryTransfer(0x0000);
		//Send pixel data first to Data Memory then copy into the selected BMP box, cycling through our Data Memory frmae
		BmpDataInput(ETargetDataMemory,iNextFrameAddress,EDirectionY, FrameBufferSizeInBytes(),iFrameNext->Ptr());
		BmpBoxDataMemoryTransfer(iNextFrameAddress);
		iNextFrameAddress+=KFrameSizeInBytes;
		if (iNextFrameAddress>KMaxDataMemoryAddress)
		{
			iNextFrameAddress=0x0000;
		}

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
void GP1212A02A::Request(TMiniDisplayRequest aRequest)
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
ID code 
[Code] 1BH,6AH,49H,44H
[Function] Send the ID code to the Host system. ID code is software version.
*/
void GP1212A02A::RequestFirmwareRevision()
    {
    if (RequestPending())
        {
        //Abort silently for now
        return;
        }

    //1BH,6AH,49H,44H
    //Send Software Revision Read Command
    FutabaVfdReport report;
    report[0]=0x00; //Report ID
    report[1]=0x04; //Report length
    report[2]=0x1B; //Command ID
    report[3]=0x6A; //Command ID
    report[4]=0x49; //Command ID
    report[5]=0x44; //Command ID
    if (Write(report)==report.Size())
        {
        SetRequest(EMiniDisplayRequestFirmwareRevision);
        }

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
	if (CurrentRequest()==EMiniDisplayRequestFirmwareRevision)
		{
			unsigned char* ptr=&iInputReport[2];
			iInputReport[7]=0x00;
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
