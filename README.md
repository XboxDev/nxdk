nxdk - *the new open source xdk*
================================
nxdk is a software development kit for the original Xbox. nxdk is a revitalization of [OpenXDK](http://openxdk.maturion.de/).

Notable features:
- No complicated cross-compiling or big library dependencies! Builds with `make` and just needs standard tools and llvm.
- 3D graphics! nxdk includes and expands on pbkit, a library for interfacing with the Xbox GPU.

Status
------
nxdk is currently in early stages of development.

Getting Started
---------------
### Prerequisites
You will need the following tools:
- GNU make
- [clang](http://clang.llvm.org/)
- [GNU bison](https://www.gnu.org/software/bison/) and [flex](http://flex.sourceforge.net/)
- [wine](https://www.winehq.org/), to invoke the NVIDIA Cg compiler
- [lld](http://lld.llvm.org/) or the [MinGW](http://www.mingw.org/) flavor of GNU ld.
- [Git](http://git-scm.com/)

#### OS X
On OS X with [Homebrew](http://brew.sh/), this should do the job (XCode ships with make and bison and flex):

    brew install wine llvm --with-lld --with-clang

#### Linux (Ubuntu)
On Ubuntu, the requried packages can be downloaded from the standard repositories:

    sudo apt-get install build-essential flex bison g++ clang wine binutils-mingw-w64 git

### Download nxdk
    git clone https://github.com/xqemu/nxdk.git
    cd nxdk
    git submodule init
    git submodule update --recursive

nxdk comes with a set of tools necessary for building. Build them with:

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
define the `GEN_XISO` variable with the name of the ISO to be created in your
project Makefile. For example:

    GEN_XISO=$(XBE_TITLE).iso

You can include additional files in the ISO (they must reside in the output
 directory) like this:

    ...
    ISO_DEPS = $(OUTPUT_DIR)/example.txt
    GEN_XISO = $(XBE_TITLE).iso

    include $(NXDK_DIR)/Makefile

    $(GEN_XISO): $(ISO_DEPS)
    $(OUTPUT_DIR)/example.txt:
        echo "Hello" > $@

    clean: clean_iso_deps
    .PHONY: clean_iso_deps
    clean_iso_deps:
        rm -f $(ISO_DEPS)

For easy ISO generation, you can also just define it when you run `make`:

    make -C samples/0ldskoo1 GEN_XISO=0ldskoo1.iso

Next Steps
----------
Copy one of the sample directories to get started. You can copy it anywhere you
like, but make sure that the `NXDK_DIR` variable in the Makefile points to
correct place. Then, in the directory, you can simply run `make`.

Credits
-------
- [OpenXDK](http://openxdk.maturion.de/) is the inspiration for nxdk, and large parts of it have been reused. (License: GPLv2)
- Large parts of [pbkit](http://forums.xbox-scene.com/index.php?/topic/573524-pbkit), by openxdkman, are included, with modifications. (License: Academic Free License)
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

