# delete larger bulk for easy rsync

echo ========= libVIODES: make clean
make clean
make -C vioedit clean

echo ========= libVIODES: deleting makefiles
rm -rf .qmake.stash
rm -f Makefile*
rm -f viocore/Makefile*
rm -f plugins/*/Makefile*
rm -f plugins/*/*/Makefile*
rm -f vioedit/Makefile*

echo ========= libVIODES: deleting xcode projects
rm -rf viodes.xcodeproj
rm -rf vioedit/vioedit.xcodeproj
rm -rf vio*/*.xcodeproj
rm -rf vio*/*/*.xcodeproj

echo ========= libVIODES: deleting libraries
rm -f libvio*.so
rm -f libvio*.dylib
rm -f libvio*.a
rm -f libvio*.lib
rm -f vio*.dll
rm -f vio*.lib
rm -f plugins/viogen/qtspline3/lib*.a

echo ========= libVIODES: deleting tutorial bins
rm -rf vioedit/*.app
rm -rf vioedit/lib/*

echo ========= libVIODES: deleting all obj dirs
rm -rf obj 
rm -rf viocore/obj 
rm -rf vioedit/obj 
rm -rf vio*/obj 

#echo ========= libVIODES: deleting local copy of libfaudes
#rm -rf libFAUDES_for_VIODES


