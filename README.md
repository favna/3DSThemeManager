<div align="center"><img src="https://github.com/ErmanSayin/Themely/raw/master/meta/banner_3ds.png" alt="Themely"/><br/>
<img src="https://3dsthem.es/app/screenshot.png" alt="Themely screenshot"/><br/>
**Themely** is a custom home menu manager for the Nintendo 3DS.<br/><br/>
<a href="https://github.com/ErmanSayin/Themely/releases/latest">Download</a><br/>
<a href="https://3dsthem.es/app">Bleeding edge builds</a></div>

# Features
- **Native code, which allows us to have...**
- **... up to 380% speed increase from CHMM2**
- ZIP support (ignores invalid ZIPs)
- BGM preview support (CHMM is usually hit or miss with this one)
- Instant theme preview
- Fullscreen preview
- QR Code reader*****
- Install without BGM
- Theme shuffle
- Theme dumping
- Multilingual (French, German, Italian, Portuguese, Russian, and Spanish available)*****
- Self updater

(***** = new in the latest stable release)

<h3 align="center">
[Planned features](https://github.com/ErmanSayin/Themely/issues?q=is%3Aissue+is%3Aopen+label%3Aenhancement) - [Known bugs](https://github.com/ErmanSayin/Themely/issues?q=is%3Aissue+is%3Aopen+label%3Abug)
</h3>

# Dependencies & Building
- DevkitARM r46
- `master` branch of ctrulib
- `next` branch of ctrulib
- Latest version of sf2dlib
- Latest version of sftdlib
- Latest version of sfillib
- zlib, libpng, freetype, libogg, and tremor from 3ds-portlibs (https://github.com/ErmanSayin/3ds_portlibs/ recommended)

You can run the `deps.sh` script to install `sf2dlib`, `sftdlib`, `sfillib`, and `3ds-portlibs` automatically.

Run `make` to build the 3DSX, CIA, and ZIP files,  `make run` to build & run on Citra. A debug build with debugging symbols will be built by default, append `DEBUG=0` to the make command to build a production build.

# Thanks
- People at Nintendo Homebrew discord #dev channel (especially MarcusD)
- DevkitPro contributors
- ctrulib contributors
- xerpi for sf2dlib and sftdlib
- ZetaDesigns for German translation
- theheroGAC for Italian translation
- Jubash for Portuguese translation
- Ann0ying for Russian translation
- umbjolt for Spanish translation

# License
This software is licensed under GNU GPLv3. Check LICENSE.

Copyright (c) 2017 Erman SAYIN
