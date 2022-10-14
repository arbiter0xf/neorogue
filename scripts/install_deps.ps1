$install_sdl = 'TRUE'
$install_gtest = 'TRUE'
$install_boostorg_win = 'TRUE'
$install_boostorg_json = 'TRUE'
$install_boostorg_full = 'TRUE'

$sdl_release_name = 'SDL2-devel-2.24.1-VC'
$sdl_release_zip = "$sdl_release_name.zip"
$sdl_image_release_name = 'SDL2_image-devel-2.6.2-VC'
$sdl_image_release_zip = "$sdl_image_release_name.zip"
$boostorg_dest_dir = 'F:\my\libs\boost'
$boostorg_full_name = 'boost_1_80_0'
$boostorg_full_zip = "$boostorg_full_name.zip"

pushd $PSScriptRoot

pushd ../
if (-Not (Test-Path -Path subprojects)) {
    mkdir subprojects
}

if (-Not (Test-Path -Path external)) {
    mkdir external
}

if (-Not (Test-Path -Path external\boostorg)) {
    mkdir external\boostorg
}

if (-Not (Test-Path -Path external\sdl2)) {
    mkdir external\sdl2
}

if (-Not (Test-Path -Path external\sdl2_image)) {
    mkdir external\sdl2_image
}
popd

if ("TRUE" -ceq "$install_sdl") {
    pushd ../
    # NOTE: To search meson wrap packages, use: meson wrap search <str>
    meson wrap install sdl2
    meson wrap install sdl2_image
    meson wrap install libpng
    meson wrap install zlib
    meson wrap install libjpeg-turbo
    popd

    pushd ../external/sdl2
    Invoke-WebRequest -Uri https://github.com/libsdl-org/SDL/releases/download/release-2.24.1/$sdl_release_zip -OutFile ./$sdl_release_zip
    Expand-Archive $sdl_release_zip -DestinationPath ./$sdl_release_name
    popd
    pushd ../external/sdl2_image
    Invoke-WebRequest -Uri https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.2/$sdl_image_release_zip -OutFile ./$sdl_image_release_zip
    Expand-Archive $sdl_image_release_zip -DestinationPath ./$sdl_image_release_name
    popd
}

if ("TRUE" -ceq "$install_gtest") {
    meson wrap install gtest
}

# TODO:
# Write meson.build so that on Windows, the boost libraries are not expected to
# be found under external/. Instead, full boost package is downloaded and
# installed. Now requiring to partly have the libraries in two locations.
pushd ../external/boostorg
if ("TRUE" -ceq "$install_boostorg_win") {
    git clone https://github.com/boostorg/winapi.git
    git clone https://github.com/boostorg/predef.git
}

if ("TRUE" -ceq "$install_boostorg_json") {
    git clone https://github.com/boostorg/json.git
    # Dependencies for using boostorg json headeronly follow
    git clone https://github.com/boostorg/config.git
    git clone https://github.com/boostorg/assert.git
    git clone https://github.com/boostorg/throw_exception.git
    git clone https://github.com/boostorg/core.git
    git clone https://github.com/boostorg/container.git
    git clone https://github.com/boostorg/move.git
    git clone https://github.com/boostorg/static_assert.git
    git clone https://github.com/boostorg/intrusive.git
    git clone https://github.com/boostorg/system.git
    git clone https://github.com/boostorg/mp11.git
    git clone https://github.com/boostorg/align.git
}
popd

if ("TRUE" -ceq "$install_boostorg_full") {
    if (-Not (Test-Path -Path $boostorg_dest_dir)) {
        mkdir $boostorg_dest_dir
    }

    Invoke-WebRequest -Uri https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/$boostorg_full_zip -OutFile $boostorg_dest_dir\$boostorg_full_zip
    Expand-Archive $boostorg_dest_dir\$boostorg_full_zip -DestinationPath $boostorg_dest_dir
}

popd
