//
//
//

#ifndef FUTABA_GP1212_H
#define FUTABA_GP1212_H

#include "Display.h"


/**
Common functionality between GP1212A01A and GP1212A02A
*/
class GP1212XXXX : public GraphicDisplay
	{
public:
	//From FutabaVfd
    virtual int MinBrightness() const {return 0;}
    virtual int MaxBrightness() const {return 5;}
	};



#endif
