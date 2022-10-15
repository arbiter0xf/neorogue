# rogue_forever
A dungeon crawler

How to run
==========
```
Linux
-----
./create_meson_build_dir.sh
cd build && PATH="${PATH}:${HOME}.local/bin" meson compile && cd ..
./run_rogue_forever.sh

Windows
-------
create_meson_build_dir.bat
cd build
meson compile
cd ..
run_rogue_forever.bat
```

How to get required assets
==========================
Ask the owner of the repo for the assets. In the future asset getting is to be
done automatically by client on startup.
See progress from:
	* https://github.com/arbiter0xf/rogue_forever/issues/18
	* https://github.com/arbiter0xf/rogue_forever/tree/initial_asset_server
