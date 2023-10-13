# NOTE: Require to prevent obtaining pre-existing information
#       and calling macro repeatly unintentionally.
if (__TOOLCHAIN_NXDK)
  return()
endif()
set(__TOOLCHAIN_NXDK 1)

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
set(CMAKE_C_STANDARD_LIBRARIES "${NXDK_DIR}/lib/libwinapi.lib ${NXDK_DIR}/lib/xboxkrnl/libxboxkrnl.lib ${NXDK_DIR}/lib/libxboxrt.lib  ${NXDK_DIR}/lib/libpdclib.lib ${NXDK_DIR}/lib/libnxdk_hal.lib ${NXDK_DIR}/lib/libnxdk.lib ${NXDK_DIR}/lib/nxdk_usb.lib ${NXDK_DIR}/lib/libnxdk_net.lib") #"${CMAKE_CXX_STANDARD_LIBRARIES_INIT}"
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

# TODO: CMAKE_MODULE_PATH is intended for project level than toolchain,
#       find out what's wrong with CMAKE_FIND_ROOT_PATH as it is not working for some reason?
set(CMAKE_MODULE_PATH "${NXDK_DIR}/share/cmake/Modules")
set(PKG_CONFIG_EXECUTABLE "${NXDK_DIR}/bin/nxdk-pkg-config" CACHE STRING "Path to pkg-config")

# Additional toolchain steps required for create xbe and xiso files.
macro(add_executable)
  # Perform normal call since there's no modification needed except for additional steps require
  _add_executable(${ARGV})

  # If author request generate iso file, then prepare the command here.
  if(GEN_XISO)
    set(XISO_OUTPUT "${NXDK_DIR}/tools/extract-xiso/build/extract-xiso" -c "$<TARGET_FILE_DIR:${ARGV0}>/bin" "$<TARGET_FILE_DIR:${ARGV0}>/${GEN_XISO}")
  endif()

  # If custom title is not given, use given executable name.
  if(NOT XBE_TITLE)
    set(XBE_TITLE ${ARGV0})
  endif()

  # Attach additional toolchain steps to executable project.
  add_custom_command(TARGET ${ARGV0} POST_BUILD
    COMMAND mkdir -p $<TARGET_FILE_DIR:${ARGV0}>/bin
    # Generate xbe binary conversion
    COMMAND "${NXDK_DIR}/tools/cxbe/cxbe" "-OUT:$<TARGET_FILE_DIR:${ARGV0}>/bin/default.xbe" "-TITLE:${XBE_TITLE}" "$<TARGET_FILE_DIR:${ARGV0}>/${ARGV0}.exe"
    # Optionally generate xiso file if requested
    COMMAND "$<$<BOOL:${XISO_OUTPUT}>:$<JOIN:${XISO_OUTPUT},;>>"
    COMMAND_EXPAND_LISTS
    VERBATIM
  )
endmacro(add_executable)
