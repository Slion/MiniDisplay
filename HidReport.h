#ifndef HID_REPORT_H
#define HID_REPORT_H

#include <string.h>

/**
Define an HID report.
Can be used as input and output.
*/
template <int S>
class HidReport
    {
public:
    HidReport(){Reset();}
    void Reset();
    inline unsigned char& operator[](int aIndex){return iBuffer[aIndex];}
    const unsigned char* Buffer() const {return iBuffer;}
    unsigned char* Buffer() {return iBuffer;}
    int Size() {return S;}
protected:
    unsigned char iBuffer[S];
    };

template <int S>
void HidReport<S>::Reset()
    {
    memset(iBuffer,0,sizeof(iBuffer));
    }


#endif
