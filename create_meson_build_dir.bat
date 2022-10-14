rem Run with "x64 Native Tools Command Prompt for VS 2022" for meson to use
rem 64-bit toolchain.

setlocal
    rem Shot myself in the foot by trying to set the environment variables on
    rem one line. For example:
    rem 'set BOOST_ROOT=F:\my\libs\boost\boost_1_80_0 && meson setup build'
    rem produces BOOST_ROOT variable which expands to the path followed by
    rem single whitespace character.
    set BOOST_ROOT=F:\my\libs\boost\boost_1_80_0
    set CXX=cl
    set CXX_FOR_BUILD=cl
    meson setup build

    rem Copy a downloaded library file, as currently
    rem 'meson wrap install rem sdl2_image' eventually seems to produce only
    rem .dll, while .lib is rem required.
    copy external\sdl2_image\SDL2_image-devel-2.6.2-VC\SDL2_image-2.6.2\lib\x64\SDL2_image.lib build\subprojects\SDL2_image-2.6.2\sdl2image.lib
    rem Also copy the .dll as it was not copied under build/ by meson.
    copy external\sdl2_image\SDL2_image-devel-2.6.2-VC\SDL2_image-2.6.2\lib\x64\SDL2_image.dll build\subprojects\SDL2_image-2.6.2\
endlocal
