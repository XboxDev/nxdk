PATH C:\tools\msys64\MINGW64\bin;C:\tools\msys64\usr\bin;%PATH%
set MSYS2_ARCH=x86_64
set MSYSTEM=MINGW64
C:\tools\msys64\usr\bin\bash.exe -lc "sh ./.ci_build_samples.sh"
