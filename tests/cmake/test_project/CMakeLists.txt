cmake_minimum_required(VERSION 3.30)

project(nxdk_cmake_test_project LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(_CMAKE_PROCESSING_LANGUAGE "CXX")
set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_TOOLCHAIN_FILE MATCHES "toolchain-nxdk.cmake")
    message(FATAL_ERROR "This project must be built with the nxdk toolchain (`-DCMAKE_TOOLCHAIN_FILE=<YOUR_NXDK_DIR>/share/toolchain-nxdk.cmake`)")
endif ()

include(FindPkgConfig)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
include(XBEUtils REQUIRED)

pkg_check_modules(
        nxdk_packages
        REQUIRED
        SDL2_image
        SDL2_ttf
        libjpeg
        libpng
        nxdk_automount_d
        nxdk_net
        pbkit
        sdl2
        winmm
        zlib
)

add_executable(
        ${PROJECT_NAME}
        src/main.cpp
        src/c_file.c
        src/asm_file.s
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${nxdk_packages_INCLUDE_DIRS}
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${nxdk_packages_LIBRARIES}
)

target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        ${nxdk_packages_CFLAGS}
)

# TODO: Determine if there is an alternative to allow handwritten assembly.
target_link_options(${PROJECT_NAME} PRIVATE "/SAFESEH:NO")


set(EXECUTABLE_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe")
add_xbe(
        xbe_file "${EXECUTABLE_BINARY}"
        TITLE "Useless test xbe"
)
add_xiso(${PROJECT_NAME}_xiso xbe_file)
