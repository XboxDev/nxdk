if (NOT TARGET NXDK::NXDK)
    if (NOT DEFINED NXDK_DIR)
        message(FATAL_ERROR "NXDK_DIR variable must be set")
    endif ()

    add_library(nxdk STATIC IMPORTED)
    set_target_properties(
            nxdk
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libnxdk.lib"
    )

    add_library(nxdk_automount_d STATIC IMPORTED)
    set_target_properties(
            nxdk_automount_d
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libnxdk_automount_d.lib"
    )

    add_library(nxdk_hal STATIC IMPORTED)
    set_target_properties(
            nxdk_hal
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libnxdk_hal.lib"
    )

    add_library(nxdk_net STATIC IMPORTED)
    set_target_properties(
            nxdk_net
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libnxdk_net.lib"
    )
    target_include_directories(
            nxdk_net
            INTERFACE
            "${NXDK_DIR}/lib/net/lwip/src/include"
            "${NXDK_DIR}/lib/net/nforceif/include"
            "${NXDK_DIR}/lib/net/nvnetdrv"
    )

    add_library(nxdk_usb STATIC IMPORTED)
    set_target_properties(
            nxdk_usb
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/nxdk_usb.lib"
    )

    add_library(pbkit STATIC IMPORTED)
    set_target_properties(
            pbkit
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libpbkit.lib"
    )

    add_library(pdclib STATIC IMPORTED)
    set_target_properties(
            pdclib
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libpdclib.lib"
    )

    add_library(winapi STATIC IMPORTED)
    set_target_properties(
            winapi
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libwinapi.lib"
    )

    add_library(winmm STATIC IMPORTED)
    set_target_properties(
            winmm
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/winmm.lib"
    )

    add_library(xboxrt STATIC IMPORTED)
    set_target_properties(
            xboxrt
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libxboxrt.lib"
    )

    add_library(zlib STATIC IMPORTED)
    set_target_properties(
            zlib
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libzlib.lib"
    )

    add_library(NXDK::NXDK INTERFACE IMPORTED)
    target_link_libraries(
            NXDK::NXDK
            INTERFACE
            nxdk
            nxdk_automount_d
            nxdk_hal
            nxdk_net
            nxdk_usb
            pbkit
            pdclib
            winapi
            winmm
            xboxrt
            zlib
    )

    target_compile_definitions(
            NXDK::NXDK
            INTERFACE
            XBOX
    )
endif ()

if (NOT TARGET NXDK::NXDK_CXX)

    add_library(nxdk_cxx STATIC IMPORTED)
    set_target_properties(
            nxdk_cxx
            PROPERTIES
            IMPORTED_LOCATION "${NXDK_DIR}/lib/libc++.lib"
    )

    add_library(NXDK::NXDK_CXX INTERFACE IMPORTED)
    target_link_libraries(NXDK::NXDK_CXX INTERFACE nxdk_cxx)
endif ()
