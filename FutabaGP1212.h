//
//
//

#ifndef FUTABA_GP1212_H
#define FUTABA_GP1212_H

#include "FutabaVfd.h"

const int KGP12xWidthInPixels = 256;
const int KGP12xHeightInPixels = 64;
const int KGP12xPixelsPerByte = 8;
const int KGP12xFrameBufferSizeInBytes = KGP12xWidthInPixels*KGP12xHeightInPixels/KGP12xPixelsPerByte; //256*64/8=2048
const int KGP12xFrameBufferPixelCount = KGP12xWidthInPixels*KGP12xHeightInPixels;


/**
Common functionality between GP1212A01A and GP1212A02A
*/
class GP1212XXXX : public FutabaGraphicDisplay
	{
public:
	//From GraphicDisplay
    virtual int MinBrightness() const {return 0;}
    virtual int MaxBrightness() const {return 5;}
    virtual int WidthInPixels() const {return KGP12xWidthInPixels;}
    virtual int HeightInPixels() const {return KGP12xHeightInPixels;}

	};



#endif
