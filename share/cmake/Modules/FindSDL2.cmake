# FPHSA_NAME_MISMATCHED is require to suppress warning message 
set(FPHSA_NAME_MISMATCHED TRUE)
include(FindPkgConfig)
pkg_check_modules(sdl2 REQUIRED sdl2)
unset(FPHSA_NAME_MISMATCHED)

add_library(SDL2::SDL2 INTERFACE IMPORTED)
set(SDL2_INCLUDE_DIRS ${sdl2_INCLUDE_DIRS})
set(SDL2_LIBRARIES ${sdl2_LIBRARIES})
set(SDL2_LINK_LIBRARIES ${sdl2_LINK_LIBRARIES})
set_target_properties(SDL2::SDL2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
  INTERFACE_LINK_LIBRARIES "${SDL2_LINK_LIBRARIES}"
  # NOTE: pkg_check_modules' Cflags definition for "XBOX" did not get included and was passed to CFLAGS_OTHER for some reason... 
  INTERFACE_COMPILE_OPTIONS "${sdl2_CFLAGS}"
)
set(SDL2_FOUND 1)
add_library(SDL2 ALIAS SDL2::SDL2)
