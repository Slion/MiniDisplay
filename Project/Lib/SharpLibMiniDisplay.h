// SharpLibMiniDisplay.h

#pragma once

#include "Display.h"
#include "MiniDisplay.h"

using namespace System;



namespace SharpLib::MiniDisplay
{

    //TODO: get proper definition
    public enum class IconType
    {
        NetworkSignal = 0,
        Internet = 1,
        Email = 2,
        Mute = 3,
        Volume = 4,
        VolumeLabel = 5,
        Play = 6,
        Pause = 7,
        Recording = 8
    };

    //TODO: get proper definition
    public enum class Request
    {
        None = 0,
        DeviceId = 1,
        FirmwareRevision = 2,
        PowerSupplyStatus = 3
    };

    //TODO: have a single enum
    public enum class Type
    {
        AutoDetect = EMiniDisplayAutoDetect,
        FutabaGP1212A01 = EMiniDisplayFutabaGP1212A01,
        FutabaGP1212A02 = EMiniDisplayFutabaGP1212A02,
        FutabaMDM166AA = EMiniDisplayFutabaMDM166AA,
        NoritakeGU256X64D39XX = EMiniDisplayNoritakeGU256X64D39XX
    };




    /// <summary>
    /// Provide access to our display hardware through MiniDisplay API.
    /// </summary>
    public ref class Display
    {
        //public delegate void OnOpenedHandler(Display aDisplay);
        //public event OnOpenedHandler OnOpened;

        //public delegate void OnClosedHandler(Display aDisplay);
        //public event OnClosedHandler OnClosed;
       
        //TODO: could have just one type?
    public:
        delegate void OnOpenedHandler(Display^ aDisplay);
        delegate void OnClosedHandler(Display^ aDisplay);

        //
        event OnOpenedHandler^ OnOpened;

        //
        event OnClosedHandler^ OnClosed;

        //Our display device handle
        MiniDisplayDevice iDevice;

    public:
        //static functions
        static int TypeCount()
        {
            return MiniDisplayTypeCount();
        }

        static String^ TypeName(Type aType)
        {
            wchar_t* name = MiniDisplayTypeName((TMiniDisplayType)aType);
            return gcnew String(name);
        }

        //Constructor
        Display()
        {
            iDevice = nullptr;
        }

        //
        bool Open(Type aType)
        {
            if (IsOpen())
            {
                //Already open return an error
                return false;
            }

            iDevice = MiniDisplayOpen((TMiniDisplayType)aType);

            bool success = iDevice != nullptr;
            if (success)
            {
                //Broadcast opened event
                OnOpened(this);
            }

            return success;
        }

        void Close()
        {
            if (!IsOpen())
            {
                //Pointless
                return;
            }

            //
            MiniDisplayClose(iDevice);
            iDevice = nullptr;
            //Broadcast closed event
            OnClosed(this);
        }

        bool IsOpen()
        {
            return iDevice != nullptr;
        }

        void Clear()
        {
            MiniDisplayClear(iDevice);
        }

        void Fill()
        {
            MiniDisplayFill(iDevice);
        }

        void SwapBuffers()
        {
            MiniDisplaySwapBuffers(iDevice);
        }

        int MaxBrightness()
        {
            return MiniDisplayMaxBrightness(iDevice);
        }

        int MinBrightness()
        {
            return MiniDisplayMinBrightness(iDevice);
        }

        void SetBrightness(int aBrightness)
        {
            if (!IsOpen()) return;

            MiniDisplaySetBrightness(iDevice, aBrightness);
        }

        int WidthInPixels()
        {
            return MiniDisplayWidthInPixels(iDevice);
        }

        int HeightInPixels()
        {
            return MiniDisplayHeightInPixels(iDevice);
        }

        void SetPixel(int aX, int aY, unsigned int aValue)
        {
            MiniDisplaySetPixel(iDevice, aX, aY, aValue);
        }

        void RequestPowerSupplyStatus()
        {
            MiniDisplayRequest(iDevice, (TMiniDisplayRequest)Request::PowerSupplyStatus);
        }

        void RequestDeviceId()
        {
            MiniDisplayRequest(iDevice, (TMiniDisplayRequest)Request::DeviceId);
        }

        void RequestFirmwareRevision()
        {
            MiniDisplayRequest(iDevice, (TMiniDisplayRequest)Request::FirmwareRevision);
        }

        void PowerOn()
        {
            MiniDisplayPowerOn(iDevice);
        }

        void PowerOff()
        {
            MiniDisplayPowerOff(iDevice);
        }

        bool SupportPowerOnOff()
        {
            return MiniDisplaySupportPowerOnOff(iDevice);
        }

        void ShowClock()
        {
            MiniDisplayShowClock(iDevice);
        }

        void HideClock()
        {
            MiniDisplayHideClock(iDevice);
        }

        bool SupportClock()
        {
            return MiniDisplaySupportClock(iDevice);
        }

        bool PowerSupplyStatus()
        {
            bool res = MiniDisplayPowerSupplyStatus(iDevice);
            return res;
        }

        Request AttemptRequestCompletion()
        {
            return (Request)MiniDisplayAttemptRequestCompletion(iDevice);
        }

        Request CurrentRequest()
        {
            return (Request)MiniDisplayCurrentRequest(iDevice);
        }

        bool IsRequestPending()
        {
            return ((Request)CurrentRequest()) != Request::None;
        }

        //
        int IconCount(IconType aIcon)
        {
            return MiniDisplayIconCount(iDevice, (TMiniDisplayIconType)aIcon);
        }

        int IconStatusCount(IconType aIcon)
        {
            return MiniDisplayIconStatusCount(iDevice, (TMiniDisplayIconType)aIcon);
        }

        void SetIconStatus(IconType aIcon, int aIndex, int aStatus)
        {
            MiniDisplaySetIconStatus(iDevice, (TMiniDisplayIconType)aIcon, aIndex, aStatus);
        }

        void SetIconOn(IconType aIcon, int aIndex)
        {
            MiniDisplaySetIconStatus(iDevice, (TMiniDisplayIconType)aIcon, aIndex, IconStatusCount(aIcon) - 1);
        }

        void SetIconOff(IconType aIcon, int aIndex)
        {
            MiniDisplaySetIconStatus(iDevice, (TMiniDisplayIconType)aIcon, aIndex, 0);
        }


        void SetAllIconsStatus(int aStatus)
        {
            for each(IconType icon in Enum::GetValues(IconType::typeid))
            {
                int count = IconCount(icon);
                for (int i = 0; i < count; i++)
                {
                    SetIconStatus(icon, i, aStatus);
                }
            }
        }

        /// <summary>
        /// Set all elements of an icon to the given status.
        /// </summary>
        /// <param name="aIcon"></param>
        /// <param name="aStatus"></param>
        void SetIconStatus(IconType aIcon, int aStatus)
        {
            int iconCount = IconCount(aIcon);
            for (int i = 0; i < iconCount; i++)
            {
                SetIconStatus(aIcon, i, aStatus);
            }
        }

        /// <summary>
        /// Set all elements of an icon to be either on or off.
        /// </summary>
        /// <param name="aIcon"></param>
        /// <param name="aOn"></param>		
        void SetIconOnOff(IconType aIcon, bool aOn)
        {
            if (aOn)
            {
                SetIconOn(aIcon);
            }
            else
            {
                SetIconOff(aIcon);
            }
        }

        /// <summary>
        /// Set all elements of an icon to there maximum status.
        /// </summary>
        /// <param name="aIcon"></param>
        void SetIconOn(IconType aIcon)
        {
            int iconCount = IconCount(aIcon);
            for (int i = 0; i < iconCount; i++)
            {
                SetIconStatus(aIcon, i, IconStatusCount(aIcon) - 1);
            }
        }

        /// <summary>
        /// Turn off all elements of an icon.
        /// </summary>
        /// <param name="aIcon"></param>
        void SetIconOff(IconType aIcon)
        {
            int iconCount = IconCount(aIcon);
            for (int i = 0; i < iconCount; i++)
            {
                SetIconStatus(aIcon, i, 0);
            }
        }


        //
        String^ Vendor()
        {
            return gcnew String(MiniDisplayVendor(iDevice));
        }

        String^ Product()
        {
            return gcnew String(MiniDisplayProduct(iDevice));
        }

        String^ SerialNumber()
        {
            return gcnew String(MiniDisplaySerialNumber(iDevice));
        }

        String^ DeviceId()
        {
            return gcnew String(MiniDisplayDeviceId(iDevice));
        }

        String^ FirmwareRevision()
        {
            return gcnew String(MiniDisplayFirmwareRevision(iDevice));
        }
    };

}
