//
//
//

#ifndef HID_DEVICE_H
#define HID_DEVICE_H

#include "HidReport.h"
#include "hidapi.h"

const int KMaxHidStringChar=256;

/**
TODO: move to another header
*/
class HidDevice
    {
public:
    HidDevice();
    int Open(const char* aPath);
    int Open(unsigned short aVendorId, unsigned short aProductId, const wchar_t* aSerialNumber);
    void Close();
    bool IsOpen();
    //
    int SetNonBlocking(int aNonBlocking);
    //Read
    template<int S>
    int Read(HidReport<S>& aInputReport);
    //Write
    template<int S>
    int Write(const HidReport<S>& aOutputReport);
    //
    const wchar_t* Error();
    //
    wchar_t* Vendor();
    wchar_t* Product();
    wchar_t* SerialNumber();

private:
    void FetchStrings();

private:
    ///Our USB HID device
    hid_device* iHidDevice;
    //
    wchar_t iVendor[KMaxHidStringChar];
    wchar_t iProduct[KMaxHidStringChar];
    wchar_t iSerialNumber[KMaxHidStringChar];
    };


/**
*/
template<int S>
int HidDevice::Write(const HidReport<S>& aOutputReport)
    {
    return hid_write(iHidDevice,aOutputReport.Buffer(),S);
    }

/**
*/
template<int S>
int HidDevice::Read(HidReport<S>& aInputReport)
    {
    return hid_read(iHidDevice,aInputReport.Buffer(),S);
    }


#endif