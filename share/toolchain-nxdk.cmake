if(DEFINED ENV{NXDK_DIR})
    set(NXDK_DIR $ENV{NXDK_DIR})
else()
    message(FATAL_ERROR "The environment variable NXDK_DIR needs to be defined.")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(TOOLCHAIN_PREFIX nxdk)
set(TOOL_OS_SUFFIX "")

if(CMAKE_HOST_WIN32)
    set(TOOL_OS_SUFFIX ".exe")
endif()

set(WIN32 1)

set(NXDK 1)

set(CMAKE_C_COMPILER "${NXDK_DIR}/bin/${TOOLCHAIN_PREFIX}-cc")
set(CMAKE_C_COMPILER_AR "llvm-ar")
set(CMAKE_C_COMPILER_RANLIB "llvm-ranlib")
set(CMAKE_C_STANDARD_LIBRARIES "${NXDK_DIR}/lib/libwinapi.lib ${NXDK_DIR}/lib/xboxkrnl/libxboxkrnl.lib ${NXDK_DIR}/lib/libxboxrt.lib  ${NXDK_DIR}/lib/libpdclib.lib ${NXDK_DIR}/lib/libnxdk_hal.lib ${NXDK_DIR}/lib/libnxdk.lib ${NXDK_DIR}/lib/nxdk_usb.lib") #"${CMAKE_CXX_STANDARD_LIBRARIES_INIT}"
set(CMAKE_C_LINK_EXECUTABLE "${NXDK_DIR}/bin/${TOOLCHAIN_PREFIX}-link <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -out:<TARGET> <LINK_LIBRARIES>")
# Keep in sync with include paths in bin/nxdk-cc
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
        "${NXDK_DIR}/lib"
        "${NXDK_DIR}/lib/xboxrt/libc_extensions"
        "${NXDK_DIR}/lib/pdclib/include"
        "${NXDK_DIR}/lib/pdclib/platform/xbox/include"
        "${NXDK_DIR}/lib/winapi"
        "${NXDK_DIR}/lib/xboxrt/vcruntime"
)

set(CMAKE_CXX_COMPILER "${NXDK_DIR}/bin/${TOOLCHAIN_PREFIX}-cxx")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} ${NXDK_DIR}/lib/libc++.lib")
set(CMAKE_CXX_LINK_EXECUTABLE "${NXDK_DIR}/bin/${TOOLCHAIN_PREFIX}-link <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -out:<TARGET> <LINK_LIBRARIES>")
# Keep in sync with include paths in bin/nxdk-cxx
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
        "${NXDK_DIR}/lib/libcxx/include"
        "${NXDK_DIR}/lib"
        "${NXDK_DIR}/lib/xboxrt/libc_extensions"
        "${NXDK_DIR}/lib/pdclib/include"
        "${NXDK_DIR}/lib/pdclib/platform/xbox/include"
        "${NXDK_DIR}/lib/winapi"
        "${NXDK_DIR}/lib/xboxrt/vcruntime"
)

set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX_C ".lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX ".lib")

# Despite this, ABI detection fails due to https://gitlab.kitware.com/cmake/cmake/-/issues/20163
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_C ".exe")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".exe")

set(CMAKE_SYSROOT "${NXDK_DIR}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(_CMAKE_C_IPO_SUPPORTED_BY_CMAKE YES)
set(_CMAKE_C_IPO_MAY_BE_SUPPORTED_BY_COMPILER YES)
set(CMAKE_C_COMPILE_OPTIONS_IPO -flto)

set(PKG_CONFIG_EXECUTABLE "${NXDK_DIR}/bin/nxdk-pkg-config" CACHE STRING "Path to pkg-config")
