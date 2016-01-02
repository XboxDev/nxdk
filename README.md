nxdk - the *new* open source xdk
================================

What?
-----

nxdk is a software development kit for the original Xbox. nxdk is a revitalisation of [OpenXDK](http://openxdk.maturion.de/), 

Notable features:

- No complicated cross-compiling or big library dependencies! Builds with `make` and just needs standard tools and llvm.

- 3D graphics! nxdk includes and expands on pbkit, a library for interfacing with the Xbox GPU.

nxdk is currently in early stages of development (read: it was hacked together over a weekend).

How?
----

You need:

- make

- A recent version of [clang](http://clang.llvm.org/) and [lld](http://lld.llvm.org/)

- [GNU bison](https://www.gnu.org/software/bison/) and [flex](http://flex.sourceforge.net/)

- *Temporarilly because I'm lazy*, [wine](https://www.winehq.org/) to invoke the NVIDIA Cg compiler

On OSX with [Homebrew](http://brew.sh/), this might do the job (XCode ships with make and bison and flex):

    brew install wine llvm --with-lld --with-clang

Now you can compile your application and nxdk:

    git submodule init
    git submodule update
    make tools
    ln -s samples/0ldskoo1 app
    make

Credits
-------

- [OpenXDK](http://openxdk.maturion.de/) is inspiration for nxdk, and large parts are reused.
    - License: GPLv2
- Large parts of [pbkit](http://forums.xbox-scene.com/index.php?/topic/573524-pbkit) by openxdkman are included, with modifications.
    - License: Academic Free License
- A very barebones libc is included based on [lib43](https://github.com/lunixbochs/lib43)
    - License: MIT
- vp20compiler is based on nvvertparse.c from [Mesa](http://www.mesa3d.org/)
    - License: MIT
- fp20compiler is based on nvparse from the [NVIDIA SDK 9.52](https://www.nvidia.com/object/sdk-9.html).
- A (windows) copy of the NVIDIA Cg compiler 1.3 from the NVIDIA SDK is bundled.

Code Overview
-------------

`app/` - code for your application. Currently contains the code for the "0ldskoo1" reversing challenge from 9447 CTF.

`bin/` - the output directory

`pbkit/` - A low level library for interfacing with the Xbox GPU.

`hal/` - Barebones Hardware Abstraction Layer for the Xbox, from OpenXDK.

`tools/cxbe` - Simple converter for PE executables to the Xbox executable format, from OpenXDK.

`tools/fp20compiler` - Translates register combiner descriptions to Xbox pushbuffer commands.

`tools/vp20compiler` - Translates vertex program assembly to Xbox microcode.

`usb/` - USB support from OpenXDK. Hacked together parts of an old Linux OHCI stack.

`xboxkrnl` - stubs and import library for the interfacing with the Xbox kernel.

`xboxrt` - A very simple libc implementation.
