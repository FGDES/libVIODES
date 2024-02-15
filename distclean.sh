# delete larger bulk for easy rsync

echo ========= make clean
make clean
make -C vioedit clean

echo ========= deleting makefiles
rm Makefile*
rm viocore/Makefile*
rm plugins/*/Makefile*
rm plugins/*/*/Makefile*
rm vioedit/Makefile*

echo ========= deleting dlls
rm libvio*.so
rm libvio*.dylib
rm libvio*.a
rm vio*.dll
rm plugins/viogen/qtspline3/lib*.a

echo ========= deleting tutorial bins
rm -rf vioedit/Vio*.app
rm -rf vioedit/lib/*

echo ========= deleting local copy of libfaudes
rm -rf libFAUDES_for_VIODES


