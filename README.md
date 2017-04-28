<p align="center"><img src="https://github.com/ErmanSayin/Themely/raw/master/meta/banner_3ds.png"/></p>

# Description

**Themely** is a custom home menu manager for the Nintendo 3DS.

# Dependencies & Building

- DevkitARM r46
- ctrulib 1.2.1
- Latest version of sf2dlib
- Latest version of sftdlib
- zlib, libpng, freetype, libogg, and tremor from 3ds-portlibs (https://github.com/devkitPro/3ds_portlibs/ recommended)

You can run the `deps.sh` script to install sf2dlib, sftdlib, and 3ds-portlibs automatically (`git` and DevkitARM required).

Run `make` to build the 3DSX, CIA, and ZIP files.
Run `make run` to build & run on Citra. (debugging only -- requires shared_font.bin)

# Known bugs

- The 3DSX version crashes on 11.4 with a 11.4 payload. (A lightweight version of Themely is planned to remedy this issue)

# License

This software is licensed under GNU GPLv3. Check LICENSE.

Copyright (c) 2017 Erman SAYIN
