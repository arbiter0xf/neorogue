# If 'buildtype' not defined, on Windows 'meson compile' needs to be ran
# multiple times until it finally succeeds. There seem to be some issues with
# generated debug information. The following are examples of errors which are
# seen:
# [401/656] Linking target subprojects/SDL2-2.24.1/test/testaudiohotplug.exe
# FAILED: subprojects/SDL2-2.24.1/test/testaudiohotplug.exe subprojects/SDL2-2.24.1/test/testaudiohotplug.pdb
# LINK : fatal error LNK1102: out of memory
# [411/656] Linking target subprojects/SDL2-2.24.1/test/testerror.exe
# FAILED: subprojects/SDL2-2.24.1/test/testerror.exe subprojects/SDL2-2.24.1/test/testerror.pdb
# Creating library subprojects\SDL2-2.24.1\test\testerror.lib and object subprojects\SDL2-2.24.1\test\testerror.exp
# libsdl2_test.a(src_thread_windows_SDL_sysmutex.c.obj) : fatal error LNK1103: debugging information corrupt; recompile module
project('rogue_forever', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++17',
    'buildtype=debug'])

common_dependencies_game = [
  dependency('sdl2'),
  dependency('sdl2_image'),
]

common_dependencies_test = [
  dependency('sdl2'),
  dependency('sdl2_image'),
  dependency('threads'),
  dependency('gtest'),
]

dependencies_content_server = [
  dependency('boost')
]

if host_machine.system() == 'windows'
  dependencies_game = [
    common_dependencies_game,
    dependency('boost')
  ]
elif host_machine.system() == 'linux'
  dependencies_game = [
    common_dependencies_game
  ]
endif

if host_machine.system() == 'windows'
  dependencies_test = [
    common_dependencies_test,
    dependency('boost')
  ]
elif host_machine.system() == 'linux'
  dependencies_test = [
    common_dependencies_test,
  ]
endif

common_include_dirs = [
  'include/common/',
]

game_include_dirs = [
  'include/game/'
]

server_include_dirs = [
  'include/server/'
]

common_include_dirs_windows = [
  'external/boostorg/winapi/include/',
  'external/boostorg/predef/include/',
]

if host_machine.system() == 'windows'
  common_includes = [
    common_include_dirs,
    common_include_dirs_windows,
  ]
elif host_machine.system() == 'linux'
  common_includes = include_directories([
    common_include_dirs
  ])
endif

game_includes = [
  game_include_dirs
]

server_includes = [
  server_include_dirs
]

includes_rogue_forever = include_directories([
  common_includes,
  game_includes,
])
includes_content_server = include_directories([
  common_includes,
  server_includes,
])
if host_machine.system() == 'windows'
  includes_test = include_directories([
    common_includes,
    game_includes,
  ])
elif host_machine.system() == 'linux'
  includes_test = include_directories([
    common_includes,
    game_includes,
    '/usr/local/include/gtest/'
  ])
endif

game_sources_shared = [
  'src/game/Json.cpp',
  'src/game/Tile.cpp',
  'src/game/GraphicsUtil.cpp',
  'src/game/Spritesheet.cpp',
  'src/game/Level.cpp',
  'src/game/Sdlw.cpp',
  'src/game/Map.cpp',
  'src/game/Log.cpp'
]

game_sources = files([
  game_sources_shared,
  'src/game/main.cpp',
])

content_server_sources = files([
  'src/server/content/main.cpp'
])

test_sources = files([
  game_sources_shared,
  'test/main.cpp',
  'test/TestLevel.cpp',
  'test/TestMap.cpp',
  'test/TestSpritesheet.cpp'
])

game_exe = executable('rogue_forever',
  game_sources,
  include_directories : includes_rogue_forever,
  dependencies : dependencies_game,
  cpp_args : '-DGRAPHICS_ENABLED=1',
  install : true)

content_server_exe = executable('rogue_forever_content_server',
  content_server_sources,
  include_directories : includes_content_server,
  dependencies : dependencies_content_server)

test_exe = executable('rogue_forever_test',
  test_sources,
  include_directories : includes_test,
  dependencies : dependencies_test,
  cpp_args : '-DBUILD_FOR_TESTS=1')

test('Main unit tests', test_exe, workdir : meson.source_root())
