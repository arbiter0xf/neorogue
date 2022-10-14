# How to enable powershell scripts
# 1. Run powershell as administrator
# 2. Run 'set-executionpolicy remotesigned'

$install_vim = 'TRUE'
$install_git_portable = 'FALSE'
$install_git_standalone = 'TRUE'
$install_win_sdk = 'TRUE'
$install_cpp_build_tools = 'TRUE'
$install_python = 'TRUE'
$install_meson_deps = 'TRUE'
$install_vcpkg = 'FALSE'
$install_meson = 'TRUE'
$install_ripgrep = 'TRUE'

$vim_exe_name = 'gvim90.exe'
$git_standalone_exe_name = 'Git-2.38.0-64-bit.exe'
$git_portable_exe_name = 'PortableGit-2.38.0-64-bit.7z.exe'
$cpp_build_tools_installer_exe_name = 'vs_BuildTools.exe'
$win_sdk_exe_name = 'win_sdk.exe'
$python_exe_name = 'python-3.10.8-amd64.exe'
$meson_exe_name = 'meson-0.63.3-64.msi'
$pkg_config_lite_dir = 'pkg-config-lite-0.28-1_bin-win32'
$pkg_config_lite_zip = 'pkg-config-lite-0.28-1_bin-win32.zip'
$ripgrep_dir = 'ripgrep-13.0.0-x86_64-pc-windows-msvc'
$ripgrep_zip = "$ripgrep_dir.zip"

$download_destination = $PSScriptRoot

if ("TRUE" -ceq "$install_git_standalone") {
    Write-Output 'Installing git'
    Invoke-WebRequest -UseBasicParsing -Uri https://github.com/git-for-windows/git/releases/download/v2.38.0.windows.1/$git_standalone_exe_name -OutFile $download_destination\$git_standalone_exe_name
    start -Wait $download_destination\$git_standalone_exe_name

    # Refresh PATH for git command to be found
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
}

if ("TRUE" -ceq "$install_git_portable") {
    Write-Output 'Installing git'
    Invoke-WebRequest -UseBasicParsing -Uri https://github.com/git-for-windows/git/releases/download/v2.38.0.windows.1/$git_portable_exe_name -OutFile $download_destination\$git_portable_exe_name
}

if ("TRUE" -ceq "$install_vim") {
    Write-Output 'Installing vim'
    Invoke-WebRequest -UseBasicParsing -Uri https://ftp.nluug.nl/pub/vim/pc/$vim_exe_name -OutFile $download_destination\$vim_exe_name
    start -Wait $download_destination\$vim_exe_name
    mkdir ~\vimfiles\bundle
    git clone https://github.com/VundleVim/Vundle.vim.git $HOME\vimfiles\bundle\Vundle.vim
    git clone https://github.com/altercation/vim-colors-solarized.git $HOME\vimfiles\bundle\vim-colors-solarized
    cp $HOME\vimfiles\bundle\vim-colors-solarized\colors\solarized.vim $HOME\vimfiles\colors\
}

if ("TRUE" -ceq "$install_win_sdk") {
    Invoke-WebRequest -Uri https://go.microsoft.com/fwlink/?linkid=2120843 -OutFile $download_destination\$win_sdk_exe_name
    start -Wait $win_sdk_exe_name
}

if ("TRUE" -ceq "$install_cpp_build_tools") {
    Write-Output 'Installing C++ build tools'
    Invoke-WebRequest -Uri https://aka.ms/vs/17/release/$cpp_build_tools_installer_exe_name -OutFile $download_destination\$cpp_build_tools_installer_exe_name
    start -Wait $download_destination\$cpp_build_tools_installer_exe_name
}

if ("TRUE" -ceq "$install_python") {
    Write-Output 'Installing python'
    Invoke-WebRequest -Uri https://www.python.org/ftp/python/3.10.8/$python_exe_name -OutFile $download_destination\$python_exe_name
    start -Wait $download_destination\$python_exe_name
}

if ("TRUE" -ceq "$install_meson_deps") {
    Write-Output 'Installing meson dependencies'
    Invoke-WebRequest -Uri https://netix.dl.sourceforge.net/project/pkgconfiglite/0.28-1/$pkg_config_lite_zip -OutFile $download_destination\$pkg_config_lite_zip
    Expand-Archive $download_destination\$pkg_config_lite_zip -DestinationPath $download_destination\$pkg_config_lite_dir
    mkdir $download_destination\win_binaries_to_path
    cp $download_destination\$pkg_config_lite_dir\pkg-config-lite-0.28-1\bin\pkg-config.exe $download_destination\win_binaries_to_path
    Add-Type -AssemblyName PresentationFramework
    [System.Windows.MessageBox]::Show("Please add $download_destination\win_binaries_to_path to your PATH variable.")
}

if ("TRUE" -ceq "$install_vcpkg") {
    Write-Output 'Installing vcpkg'
    git clone https://github.com/microsoft/vcpkg F:\my\tools\vcpkg
    pushd F:\my\tools\vcpkg
    .\bootstrap-vcpkg.bat
    popd
}

if ("TRUE" -ceq "$install_meson") {
    Write-Output 'Installing meson'
    Invoke-WebRequest -Uri https://github.com/mesonbuild/meson/releases/download/0.63.3/$meson_exe_name -OutFile $download_destination\$meson_exe_name
    start -Wait $download_destination\$meson_exe_name
}

if ("TRUE" -ceq "$install_ripgrep") {
    Write-Output 'Installing ripgrep'
    Invoke-WebRequest -Uri https://github.com/BurntSushi/ripgrep/releases/download/13.0.0/$ripgrep_zip -OutFile $download_destination\$ripgrep_zip
    Expand-Archive $download_destination\$ripgrep_zip -DestinationPath $download_destination\$ripgrep_dir
    cp $download_destination\$ripgrep_dir\$ripgrep_dir\rg.exe $download_destination\win_binaries_to_path
}
