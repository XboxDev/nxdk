# NOTE: custom FindThreads module is a requirement due to CMake's FindThreads module is looking for pThreads.
# nxdk's toolchain is using system named Generic instead of "Windows" which could had work but inaccurate name.
# xboxkrnl and pdclib have C11 thread support out of the box
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(Threads_FOUND TRUE)
add_library(Threads::Threads INTERFACE IMPORTED)
