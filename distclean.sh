# delete larger bulk for easy rsync

echo ========= libVIODES: make clean
make clean
make -C vioedit clean

echo ========= libVIODES: deleting makefiles
rm -f Makefile*
rm -f viocore/Makefile*
rm -f plugins/*/Makefile*
rm -f plugins/*/*/Makefile*
rm -c vioedit/Makefile*

echo ========= libVIODES: deleting libraries
rm -f libvio*.so
rm -f libvio*.dylib
rm -f libvio*.a
rm -f libvio*.lib
rm -f vio*.dll
rm -f vio*.lib
rm -f plugins/viogen/qtspline3/lib*.a

echo ========= libVIODES: deleting tutorial bins
rm -rf vioedit/Vio*.app
rm -rf vioedit/lib/*

#echo ========= libVIODES: deleting local copy of libfaudes
#rm -rf libFAUDES_for_VIODES


