# Features
- **Native code, which allows us to have...**
- **... up to 380% speed increase from CHMM2**
- **but most importantly, no references to ErmanSaying**
- ZIP support (ignores invalid ZIPs)
- BGM preview support (CHMM is usually hit or miss with this one)
- Instant theme preview
- Fullscreen preview
- QR Code reader
- Install without BGM
- Theme shuffle
- Theme dumping
- Multilingual (French, German, Italian, Portuguese, Russian, and Spanish available)

# Dependencies & Building
- DevkitARM r46
- `master` branch of ctrulib
- `next` branch of ctrulib
- Latest version of sf2dlib
- Latest version of sftdlib
- Latest version of sfillib
- zlib, libpng, libjpeg-turbo, freetype, libogg, and tremor from 3ds-portlibs (https://github.com/javimadgit/3ds_portlibs/ recommended)

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
This software is unlicensed as it already breaks the original license set on the software by ErmanSaying