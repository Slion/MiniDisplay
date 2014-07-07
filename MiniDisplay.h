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

//Open & Close functions
extern "C" MDAPI MiniDisplayDevice MiniDisplayOpen();
extern "C" MDAPI void MiniDisplayClose(MiniDisplayDevice aDevice);

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
extern "C" MDAPI void MiniDisplaySetPixel(MiniDisplayDevice aDevice, int aX, int aY, int aValue);

//TODO: Have an API to specify pixel depth


#endif

