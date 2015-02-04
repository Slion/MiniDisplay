//
//
//

#ifndef MINI_DISPLAY_H
#define MINI_DISPLAY_H

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32)
  #if defined(MiniDisplay_EXPORTS)
    #define  MDAPI __declspec(dllexport)
  #else
    #define  MDAPI __declspec(dllimport)
  #endif /* MyLibrary_EXPORTS */
#else /* defined (_WIN32) */
 #define MDAPI
#endif

typedef void* MiniDisplayDevice;

typedef enum
    {
	EMiniDisplayAutoDetect=0,
    EMiniDisplayFutabaGP1212A01,
    EMiniDisplayFutabaGP1212A02,
	//EMiniDisplayFutabaMDM166AA,
	EMiniDisplayAutoDetectFailed
    }
TMiniDisplayType;

typedef enum
    {
    EMiniDisplayRequestNone,
    EMiniDisplayRequestDeviceId,
    EMiniDisplayRequestFirmwareRevision,
    EMiniDisplayRequestPowerSupplyStatus
    }
TMiniDisplayRequest;

/**
Attempt to establish a connection to with a display of the given type.
Supports display auto-detection too.

@param [IN] The display type we want to connect to.
@return Handle to the device we connected to on success, null otherwise.
*/
extern "C" MDAPI MiniDisplayDevice MiniDisplayOpen(TMiniDisplayType aType);

/**
Close the connection with the given display device.

@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayClose(MiniDisplayDevice aDevice);

/**
Provides the number of display types supported.
That includes the pseudo 'Auto-Detect' display.
@return The number of display type supported.
*/
extern "C" MDAPI int MiniDisplayTypeCount();

/**
Provides the human readable name of the given display type.

@param [IN] The display type we want to get the name for.
@return The name of the given display type.
*/
extern "C" MDAPI wchar_t* MiniDisplayTypeName(TMiniDisplayType aType);

/**
Clear our MiniDisplay.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayClear(MiniDisplayDevice aDevice);

/**
Fill our MiniDisplay.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayFill(MiniDisplayDevice aDevice);

/**
Swap our MiniDisplay buffers committing our back buffer content to the screen.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplaySwapBuffers(MiniDisplayDevice aDevice);

/**
Provide maximum brightness level for the given device.
@param [IN] The device to apply this command to.
@return Maximum brightness level.
*/
extern "C" MDAPI int MiniDisplayMaxBrightness(MiniDisplayDevice aDevice);

/**
Provide minimum brightness level for the given device.
@param [IN] The device to apply this command to.
@return Minimum brightness level.
*/
extern "C" MDAPI int MiniDisplayMinBrightness(MiniDisplayDevice aDevice);

/**
Set device brightness level.
@param [IN] The device to apply this command to.
@param [IN] Brightness level
*/
extern "C" MDAPI void MiniDisplaySetBrightness(MiniDisplayDevice aDevice, int aBrightness);

/**
Provide pixels width of our display.
@param [IN] The device to apply this command to.
@return Width in pixels.
*/
extern "C" MDAPI int MiniDisplayWidthInPixels(MiniDisplayDevice aDevice);

/**
Provide pixels height of our display.
@param [IN] The device to apply this command to.
@return Height in pixels.
*/
extern "C" MDAPI int MiniDisplayHeightInPixels(MiniDisplayDevice aDevice);

/**
Set our given pixel.
@param [IN] The device to apply this command to.
@param [IN] Pixel X coordinate.
@param [IN] Pixel Y coordinate.
@param [IN] Pixel value.
*/
extern "C" MDAPI void MiniDisplaySetPixel(MiniDisplayDevice aDevice, int aX, int aY, unsigned int aPixel);

//TODO: Have an API to specify pixel depth

/**
Provide vendor name.
@param [IN] The device to apply this command to.
@return Vendor name.
*/
extern "C" MDAPI wchar_t* MiniDisplayVendor(MiniDisplayDevice aDevice);

/**
Provide product name.
@param [IN] The device to apply this command to.
@return Product name.
*/
extern "C" MDAPI wchar_t* MiniDisplayProduct(MiniDisplayDevice aDevice);

/**
Provide Serial number.
@param [IN] The device to apply this command to.
@return Serial number name.
*/
extern "C" MDAPI wchar_t* MiniDisplaySerialNumber(MiniDisplayDevice aDevice);

/**
Request device ID.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayRequest(MiniDisplayDevice aDevice, TMiniDisplayRequest aRequest);

/**
Tell whether or not a request is pending.
@param [IN] The device to apply this command to.
@return true if we have a request pending, false otherwise.
*/
extern "C" MDAPI bool MiniDisplayRequestPending(MiniDisplayDevice aDevice);


/**
Provide the current request if any.
@param [IN] The device to apply this command to.
@return The current request if any.
*/
extern "C" MDAPI TMiniDisplayRequest MiniDisplayCurrentRequest(MiniDisplayDevice aDevice);


/**
Cancel any pending request.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayCancelRequest(MiniDisplayDevice aDevice);

/**
Attempt request completion.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI TMiniDisplayRequest MiniDisplayAttemptRequestCompletion(MiniDisplayDevice aDevice);

/**
Provide device ID.
@param [IN] The device to apply this command to.
@return Device ID name.
*/
extern "C" MDAPI char* MiniDisplayDeviceId(MiniDisplayDevice aDevice);

/**
Provide firmware revision.
@param [IN] The device to apply this command to.
@return Firmware revision name.
*/
extern "C" MDAPI char* MiniDisplayFirmwareRevision(MiniDisplayDevice aDevice);

/**
Get power supply status.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI bool MiniDisplayPowerSupplyStatus(MiniDisplayDevice aDevice);

/**
Turn device Power ON.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayPowerOn(MiniDisplayDevice aDevice);

/**
Turn device Power OFF.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayPowerOff(MiniDisplayDevice aDevice);

/**
Specifies whether or not this display supports power ON/OFF functions.
@param [IN] The device to apply this command to.
@return True if one can turn display power on and off, false otherwise.
*/
extern "C" MDAPI bool MiniDisplaySupportPowerOnOff(MiniDisplayDevice aDevice);

/**
Show built-in clock.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayShowClock(MiniDisplayDevice aDevice);

/**
Hide built-in clock.
@param [IN] The device to apply this command to.
*/
extern "C" MDAPI void MiniDisplayHideClock(MiniDisplayDevice aDevice);

/**
Specifies whether or not this display supports clock functions.
@param [IN] The device to apply this command to.
@return True if this display supports built-in clock, false otherwise.
*/
extern "C" MDAPI bool MiniDisplaySupportClock(MiniDisplayDevice aDevice);


#endif

