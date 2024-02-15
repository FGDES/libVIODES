echo ==================== set qt path (wher relevant resides)

export PATH=~/Qt/6.6.2/macos/bin:$PATH

echo ==================== set version in qmake database

. ./VERSION
VIODES_VERSION=$VIODES_VERSION_MAJOR.$VIODES_VERSION_MINOR
qmake -set VIODES_VERSION $VIODES_VERSION 
qmake -set VIODES_VERSION_MAJOR $VIODES_VERSION_MAJOR 
qmake -set VIODES_VERSION_MINOR $VIODES_VERSION_MINOR 

