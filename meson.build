project('rogue_forever', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

dependencies_game = [
  dependency('sdl2'),
  dependency('sdl2_image'),
  dependency('boost'),
]

dependencies_test = [
  dependency('sdl2'),
  dependency('sdl2_image'),
  dependency('boost'),
  dependency('threads'),
  dependency('gtest'),
]

common_includes_dirs = [
  'external/boostorg/json/include/',
  'external/boostorg/config/include/',
  'external/boostorg/assert/include/',
  'external/boostorg/throw_exception/include/',
  'external/boostorg/core/include/',
  'external/boostorg/container/include/',
  'external/boostorg/move/include/',
  'external/boostorg/static_assert/include/',
  'external/boostorg/intrusive/include/',
  'external/boostorg/system/include/',
  'external/boostorg/mp11/include/',
  'external/boostorg/align/include/',
  'include/',
]

common_includes_dirs_windows = [
  'external/boostorg/winapi/include/',
  'external/boostorg/predef/include/',
]

if host_machine.system() == 'windows'
  common_includes = include_directories([
    common_includes_dirs,
    common_includes_dirs_windows,
  ])
elif host_machine.system() == 'linux'
  common_includes = include_directories([
    common_includes_dirs
  ])
endif

includes_game = common_includes
if host_machine.system() == 'windows'
  includes_test = include_directories([
    common_includes_dirs,
  ])
elif host_machine.system() == 'linux'
  includes_test = include_directories([
    common_includes_dirs,
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

test_sources = files([
  game_sources_shared,
  'test/main.cpp',
  'test/TestLevel.cpp',
  'test/TestMap.cpp',
  'test/TestSpritesheet.cpp'
])

game_exe = executable('rogue_forever',
  game_sources,
  include_directories : includes_game,
  dependencies : dependencies_game,
  cpp_args : '-DGRAPHICS_ENABLED=1',
  install : true)

test_exe = executable('rogue_forever_test',
  test_sources,
  include_directories : includes_test,
  dependencies : dependencies_test,
  cpp_args : '-DBUILD_FOR_TESTS=1')

test('Main unit tests', test_exe, workdir : meson.source_root())
