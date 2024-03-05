echo ==================== use local qt

#export PATH=/usr/lib/qt5/bin:$PATH
export PATH=~/Qt5_12_12/5.12.12/gcc_64/bin:$PATH
echo using $(which qmake)


echo ==================== set version
. VERSION
qmake -set VIODES_VERSION_MAJOR $VIODES_VERSION_MAJOR
qmake -set VIODES_VERSION_MINOR $VIODES_VERSION_MINOR

