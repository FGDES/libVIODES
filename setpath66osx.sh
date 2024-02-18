echo ==================== "select qt release"

export PATH=~/Qt/6.6.2/macos/bin:$PATH
echo using "$(which qmake)"

echo ==================== set version in qmake database

. ./VERSION
VIODES_VERSION=$VIODES_VERSION_MAJOR.$VIODES_VERSION_MINOR
qmake -set VIODES_VERSION $VIODES_VERSION 
qmake -set VIODES_VERSION_MAJOR $VIODES_VERSION_MAJOR 
qmake -set VIODES_VERSION_MINOR $VIODES_VERSION_MINOR 

