# delete larger bulk for easy rsync

echo ========= libVIODES: make clean
make clean
make -C vioedit clean

echo ========= libVIODES: deleting makefiles
rm -rf .qmake.stash
rm -f Makefile*
rm -f vio*/Makefile*
rm -f viogen/qtspilne3/Makefile*

echo ========= libVIODES: deleting xcode projects
rm -rf *.xcodeproj
rm -rf vio*/Release
rm -rf vio*/Debug
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
rm -rf vioedit/vioedit.xcodeproj
rm -rf vioedit/*.app
rm -rf vioedit/*/*.app
rm -rf vioedit/lib/*

echo ========= libVIODES: deleting all obj dirs
rm -rf obj 
rm -rf viocore/obj 
rm -rf vioedit/obj 
rm -rf vio*/obj 

#echo ========= libVIODES: deleting local copy of libfaudes
#rm -rf libFAUDES_for_VIODES


