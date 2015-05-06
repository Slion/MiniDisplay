using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MiniDisplayInterop
{
	public class MiniDisplay
    {

		//[Serializable]
		public enum Type
		{
			AutoDetect, /*Not yet implemented*/
			//[EnumMember(Value = "EMiniDisplayFutabaGP1212A01")]
			FutabaGP1212A01,
			//[EnumMember(Value = "EMiniDisplayFutabaGP1212A01")]
			FutabaGP1212A02
		};

		/// <summary>
		/// 
		/// </summary>
		public enum Request
		{
			None,
			DeviceId,
			FirmwareRevision,
			PowerSupplyStatus
		};


		/// <summary>
		/// Define the various type of icons we support.
		/// For binary compatibility new entries must be added at the end.
		/// </summary>
		public enum IconType
		{
			NetworkSignal = 0,
			Internet,
			Email,
			Mute,
			Volume,
			VolumeLabel,
			Play,
			Pause,
			Recording
		};

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayOpen", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr Open(Type aType);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayClose", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void Close(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayTypeCount", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int TypeCount();

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayTypeName", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr TypeName(Type aType);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayClear", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void Clear(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayFill", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void Fill(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySwapBuffers", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SwapBuffers(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySetBrightness", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetBrightness(IntPtr aDevice, int aBrightness);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayMinBrightness", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MinBrightness(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayMaxBrightness", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int MaxBrightness(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayWidthInPixels", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int WidthInPixels(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayHeightInPixels", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int HeightInPixels(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySetPixel", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int SetPixel(IntPtr aDevice, int aX, int aY, uint aValue);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayVendor", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr Vendor(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayProduct", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr Product(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySerialNumber", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr SerialNumber(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayDeviceId", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr DeviceId(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayFirmwareRevision", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr FirmwareRevision(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayPowerSupplyStatus", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool PowerSupplyStatus(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayRequest", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SendRequest(IntPtr aDevice, Request aRequest);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayAttemptRequestCompletion", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern Request AttemptRequestCompletion(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayCurrentRequest", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern Request CurrentRequest(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayCancelRequest", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void CancelRequest(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayClose", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void PowerOn(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayPowerOff", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void PowerOff(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySupportPowerOnOff", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool SupportPowerOnOff(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayShowClock", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void ShowClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayHideClock", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void HideClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySupportClock", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.I1)]
		public static extern bool SupportClock(IntPtr aDevice);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayIconCount", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int IconCount(IntPtr aDevice, IconType aIcon);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplayIconStatusCount", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern int IconStatusCount(IntPtr aDevice, IconType aIcon);

		[DllImport("MiniDisplay.dll", EntryPoint = "MiniDisplaySetIconStatus", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetIconStatus(IntPtr aDevice, IconType aIcon, int aIndex, int aStatus);


    }
}
