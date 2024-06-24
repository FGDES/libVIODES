echo ==================== "select qt release and matching toolchain"

export PATH=/C/Qt/Tools/mingw1120_64/bin:$PATH
echo using MinGW "$(which g++)"
export PATH=/C/Qt/6.6.2/mingw_64/bin:$PATH
echo using Qt "$(which qmake)"

echo ==================== set version in qmake database

. ./VERSION
PLANTSIM_VERSION=$PLANTSIM_VERSION_MAJOR.$PLANTSIM_VERSION_MINOR
qmake -set PLANTSIM_VERSION $PLANTSIM_VERSION 
qmake -set PLANTSIM_VERSION_MAJOR $PLANTSIM_VERSION_MAJOR 
qmake -set PLANTSIM_VERSION_MINOR $PLANTSIM_VERSION_MINOR 

