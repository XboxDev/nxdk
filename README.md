nxdk - *the new open source xdk*
================================
nxdk is a software development kit for the original Xbox. nxdk is a revitalization of [OpenXDK](https://web.archive.org/web/20170624051336/http://openxdk.sourceforge.net:80/). 
It is maintained by the [XboxDev](https://github.com/XboxDev/XboxDev) community.

Notable features:
- Portable toolchain that works on modern versions of Windows, macOS and Linux.
- No complicated cross-compiling or big library dependencies! Builds with `make` and just needs standard tools and llvm.
- Modern C / C++ standards and compiler features.
- Supports popular APIs like Windows API and BSD sockets.
- SDL2 support for input, audio and 2D graphics.
- Custom API for 3D graphics using NVIDIA-designed shader-languages (with additional Xbox extensions).
- Open-Source drivers which can be modified to get the most out of the hardware.
- Modifiable startup code, for as much system control as necessary.
- Supported by an active community that can help with problems and responds to bug reports.

Build Status
------
![CI Status for "Build Samples"](https://github.com/XboxDev/nxdk/workflows/Build%20Samples/badge.svg?branch=master&event=push)

Status
------
While nxdk still is in early stages of development, it can already be used in many projects.
Take a look at the [list of projects](https://github.com/XboxDev/nxdk/wiki/Projects-using-nxdk) that are build on top of nxdk. Additionally, the [provided samples](https://github.com/XboxDev/nxdk/tree/master/samples) show how to use common features.

Getting Started
---------------
### Prerequisites
You will need the following tools:
- GNU make
- [clang](http://clang.llvm.org/)
- [GNU bison](https://www.gnu.org/software/bison/) and [flex](http://flex.sourceforge.net/)
- [lld](http://lld.llvm.org/)
- [Git](http://git-scm.com/)
- [CMake](https://cmake.org/)

OS-specific instructions for installing these prerequisites can be found in the [Wiki](https://github.com/XboxDev/nxdk/wiki/Install-the-Prerequisites)

### Download nxdk
    git clone --recursive https://github.com/XboxDev/nxdk.git

### Build Samples
Samples are easily built by running the Makefile in one of the sample directories. Details can be found in the [Wiki](https://github.com/XboxDev/nxdk/wiki/Build-a-Sample). nxdk also supports automatic [creation of ISO files](https://github.com/XboxDev/nxdk/wiki/Create-an-XISO).

Next Steps
----------
Copy one of the sample directories to get started. You can copy it anywhere you
like, but make sure that the `NXDK_DIR` variable in the Makefile points to
correct place. Then, in the directory, you can simply run `make`.

Credits
-------
- [OpenXDK](https://web.archive.org/web/20170624051336/http://openxdk.sourceforge.net:80/) is the inspiration for nxdk, and large parts of it have been reused. (License: GPLv2)
- Large parts of [pbkit](https://web.archive.org/web/20141024145308/http://forums.xbox-scene.com/index.php?/topic/573524-pbkit/), by openxdkman, are included, with modifications. (License: LGPL)
- A network stack is included based on [lwIP](http://savannah.nongnu.org/projects/lwip/) (License: Modified BSD)
- A libc is included based on [PDCLib](https://github.com/DevSolar/pdclib) (License: CC0)
- Large parts of the runtime library are derived from LLVM's [compiler-rt](https://compiler-rt.llvm.org/) library (License: MIT)
- vp20compiler is based on nvvertparse.c from [Mesa](http://www.mesa3d.org/) (License: MIT)
- fp20compiler is based on nvparse from the [NVIDIA SDK 9.52](https://www.nvidia.com/object/sdk-9.html).
- The [NVIDIA Cg compiler](https://developer.nvidia.com/cg-toolkit) is bundled.
- extract-xiso developed by in et al. (License: BSD)

Code Overview
-------------
* `lib/hal` - Barebones Hardware Abstraction Layer for the Xbox, from OpenXDK.
* `lib/net` - Network stack for the Xbox based on lwIP.
* `lib/pdclib` - Xbox port of PDCLib, a CC0-licensed C standard library.
* `lib/pbkit` - A low level library for interfacing with the Xbox GPU.
* `lib/sdl` - Xbox ports of SDL2 and SDL_ttf.
* `lib/usb` - USB support from OpenXDK. Hacked together parts of an old Linux OHCI stack.
* `lib/winapi` - Xbox specific implementations of common useful WinAPI-functions.
* `lib/xboxkrnl` - Header and import library for interfacing with the Xbox kernel.
* `lib/xboxrt` - Miscellaneous functionality for debugging etc.
* `tools/cxbe` - Simple converter for PE executables to the Xbox executable format, from OpenXDK.
* `tools/fp20compiler` - Translates register combiner descriptions to Xbox pushbuffer commands.
* `tools/vp20compiler` - Translates vertex program assembly to Xbox microcode.
* `tools/extract-xiso` - Generates and extracts ISO images compatible with the Xbox (and XQEMU).
* `samples/` - Sample applications to get started.
