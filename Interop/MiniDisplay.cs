using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MiniDisplay
{
    public class Interop
    {

		//[Serializable]
		public enum TMiniDisplayType
		{
			EMiniDisplayAutoDetect, /*Not yet implemented*/
			//[EnumMember(Value = "EMiniDisplayFutabaGP1212A01")]
			EMiniDisplayFutabaGP1212A01,
			//[EnumMember(Value = "EMiniDisplayFutabaGP1212A01")]
			EMiniDisplayFutabaGP1212A02
		};

		/// <summary>
		/// 
		/// </summary>
		public enum TMiniDisplayRequest
		{
			EMiniDisplayRequestNone,
			EMiniDisplayRequestDeviceId,
			EMiniDisplayRequestFirmwareRevision,
			EMiniDisplayRequestPowerSupplyStatus
		};


		/// <summary>
		/// Define the various type of icons we support.
		/// For binary compatibility new entries must be added at the end.
		/// </summary>
		public enum TMiniDisplayIconType
		{
			EMiniDisplayIconNetworkSignal = 0,
			EMiniDisplayIconInternet,
			EMiniDisplayIconEmail,
			EMiniDisplayIconMute,
			EMiniDisplayIconVolume,
			EMiniDisplayIconVolumeLabel,
			EMiniDisplayIconPlay,
			EMiniDisplayIconPause,
			EMiniDisplayIconRecording
		};

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayOpen(TMiniDisplayType aType);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayClose(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayTypeCount();

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayTypeName(TMiniDisplayType aType);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayClear(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayFill(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplaySwapBuffers(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplaySetBrightness(IntPtr aDevice, int aBrightness);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayMinBrightness(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayMaxBrightness(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayWidthInPixels(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayHeightInPixels(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplaySetPixel(IntPtr aDevice, int aX, int aY, uint aValue);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayVendor(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayProduct(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplaySerialNumber(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayDeviceId(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr MiniDisplayFirmwareRevision(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool MiniDisplayPowerSupplyStatus(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayRequest(IntPtr aDevice, TMiniDisplayRequest aRequest);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern TMiniDisplayRequest MiniDisplayAttemptRequestCompletion(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern TMiniDisplayRequest MiniDisplayCurrentRequest(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayCancelRequest(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayPowerOn(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayPowerOff(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool MiniDisplaySupportPowerOnOff(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayShowClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplayHideClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool MiniDisplaySupportClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayIconCount(IntPtr aDevice, TMiniDisplayIconType aIcon);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MiniDisplayIconStatusCount(IntPtr aDevice, TMiniDisplayIconType aIcon);

		[DllImport("MiniDisplay.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MiniDisplaySetIconStatus(IntPtr aDevice, TMiniDisplayIconType aIcon, int aIndex, int aStatus);


    }
}
