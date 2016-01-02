NXDK - *the new open source xdk*
================================
**Note:** This is a custom fork of NXDK. The official NXDK repository can be found [here](https://github.com/xqemu/nxdk). Documentation below applies to this fork. 

NXDK is a software development kit for the original Xbox, and compatible with the XQEMU emulator. NXDK is a revitalization of [OpenXDK](http://opeNXDK.maturion.de/).

Notable features:
- No complicated cross-compiling or big library dependencies! Builds with `make` and just needs standard tools and llvm.
- 3D graphics! NXDK includes and expands on pbkit, a library for interfacing with the Xbox GPU.

Status
------
NXDK is currently in early stages of development.

Getting Started
---------------
### Prerequisites
You will need the following tools:
- GNU make
- [clang](http://clang.llvm.org/) and [lld](http://lld.llvm.org/)
- [GNU bison](https://www.gnu.org/software/bison/) and [flex](http://flex.sourceforge.net/)
- [wine](https://www.winehq.org/), to invoke the NVIDIA Cg compiler

#### OS X
On OSX with [Homebrew](http://brew.sh/), this should do the job (XCode ships with make and bison and flex):

    brew install wine llvm --with-lld --with-clang

### Download NXDK

    git clone https://github.com/mborgerson/nxdk.git
    cd nxdk
    git submodule init
    git submodule update
    make tools

### Build Samples

To build the 0ldskoo1 sample, you can run:

    cd samples/0ldskoo1
    make

This will generate a single executable, default.xbe, in the bin/ directory which
can be executed on your Xbox (or XQEMU emulator).

### Generate XISO

To generate an ISO file that can be burned to a disc, or passed to the XQEMU
emulator via the `-drive index=1,media=cdrom,file=/path/to/your.iso` parameter,
specify define the `GEN_XISO` variable with the name of the ISO to be created in
your project Makefile. For example:

    GEN_XISO=$(XBE_TITLE).iso

You can include additional files in the ISO (they must reside in the output
 directory) like this:

    ISO_DEPS = bin/example.txt
    $(GEN_XISO): $(ISO_DEPS)

    bin/example.txt:
        echo "Hello" > $@

    clean: $(ISO_DEPS)

For easy ISO generation, you can also just specify it when you run `make`:

    make -C samples/0ldskoo1 GEN_XISO=0ldskoo1.iso

Next Steps
----------
Copy one of the sample directories to get started. In the directory, you can
simply run `make`.

Credits
-------
- [OpenXDK](http://openxdk.maturion.de/) is inspiration for NXDK, and large parts are reused. (License: GPLv2)
- Large parts of [pbkit](http://forums.xbox-scene.com/index.php?/topic/573524-pbkit) by openxdkman are included, with modifications. (License: Academic Free License)
- A very barebones libc is included based on [lib43](https://github.com/lunixbochs/lib43) (License: MIT)
- vp20compiler is based on nvvertparse.c from [Mesa](http://www.mesa3d.org/) (License: MIT)
- fp20compiler is based on nvparse from the [NVIDIA SDK 9.52](https://www.nvidia.com/object/sdk-9.html).
- A (Windows) copy of the NVIDIA Cg compiler 1.3 from the NVIDIA SDK is bundled.
- extract-xiso developed by in et al. (License: BSD)

Code Overview
-------------
* `lib/hal/` - Barebones Hardware Abstraction Layer for the Xbox, from OpenXDK.
* `lib/pbkit/` - A low level library for interfacing with the Xbox GPU.
* `lib/usb/` - USB support from OpenXDK. Hacked together parts of an old Linux OHCI stack.
* `lib/xboxkrnl` - Stubs and import library for the interfacing with the Xbox kernel.
* `lib/xboxrt` - A very simple libc implementation.
* `tools/cxbe` - Simple converter for PE executables to the Xbox executable format, from OpenXDK.
* `tools/fp20compiler` - Translates register combiner descriptions to Xbox pushbuffer commands.
* `tools/vp20compiler` - Translates vertex program assembly to Xbox microcode.
* `tools/extract-xiso` - Generates and extracts ISO images compatible with the Xbox (and XQEMU).
* `samples/` - Sample applications to get started.
