rm -rf tmp;
mkdir tmp;
cd tmp;

git clone https://github.com/devkitPro/3ds_portlibs.git;
git clone https://github.com/xerpi/sf2dlib.git;
git clone https://github.com/xerpi/sftdlib.git;

cd 3ds_portlibs;
make zlib;
make install-zlib;
make libogg;
make install;
make libpng freetype tremor;
sudo -E make install;
cd ../..;

cd sf2dlib/libsf2d;
make -j4;
sudo -E make install;
cd ../..;

cd sftdlib/libsftd;
make -j4;
sudo -E make install;

cd ../../../;

rm -rf tmp;
