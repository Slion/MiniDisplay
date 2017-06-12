![Build Status](https://slions.visualstudio.com/_apis/public/build/definitions/ad16bbd0-a884-4787-8e3a-85daf30cca16/3/badge)

# MiniDisplay
Library to drive VFD devices.

# Supported hardware
We support the following hardware:
* Noritake GU256x64x-39xx (USB over [Teensy 3.2]) - recommended
* Futaba GP1212A01
* Futaba GP1212A02
* Futaba MDM166AA

See [Arduino Micro & Noritake VFD].

# Releases
See [NuGet Gallery].

# Development 
## Continuous Integration
Commit to master will trigger our Visual Studio Team Service build:
* If the [nuspec] NuGet version was not incremented the build should be partially succesful.
* If the NuGet version was incremented the build should be succesful and published on [NuGet Gallery].
 
[NuGet Gallery]: https://www.nuget.org/packages/SharpLibMiniDisplay
[nuspec]: https://github.com/Slion/MiniDisplay/blob/master/Publish/SharpLibMiniDisplay.nuspec
[Teensy 3.2]: https://www.pjrc.com/teensy/teensy31.html
[Arduino Micro & Noritake VFD]: https://slions.net/threads/arduino-micro-noritake-vfd.40
