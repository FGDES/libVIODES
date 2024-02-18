#!/bin/bash

############################################################################
# convenience script to copy, configure and compile libfaudes from source

# this script must be invoked with ./libviodes as the current directory
# hence, the source and destination specification are relative to ./libviodes
# a typical setup will have ./lobfaudes next to ./libviodes

# edit this line to select the libfaudes source location
#FAUDES_SRC=../libfaudes-2_31h
FAUDES_SRC=../libFAUDES

# do not change the libfaudes destination
FAUDES_DST=libFAUDES_for_VIODES


############################################################################
# report to user
echo ==================== copyfaudes.sh
echo "copy, configure and compile libFAUDES for libVIODES"
echo "current directory: " $(pwd)
echo "importing libFAUDES from " $FAUDES_SRC 

# use lsb compilers on linux
#if [ $(uname -s) == "Linux" ]; then
#export FAUDES_PLATFORM="lsb_linux"
#echo override FAUDES_PLATFORM to use lsb-compilers
#fi

echo "press return to proceed or ctrl-c to bail out"
read

############################################################################
# some consistency tests
if [ ! -d viocore ]; then
    echo "error: the current directory appears not to be a libviodes distribution: abort"
    return
fi
if [ ! -f viodes.pro ]; then
    echo "error: the current directory appears not to be a libviodes distribution: abort"
    return
fi
if [ ! -f VERSION ]; then
    echo "error: the current directory appears not to be a libviodes distribution: abort"
    return
fi
if [ ! -f ${FAUDES_SRC}/src/cfl_baseset.h ]; then
    echo "error: the specified source appears to be not a libFAUDES source tree: abort"
    return
fi
if [ ! -d ${FAUDES_SRC}/plugins/synthesis ]; then
    echo "error: the specified source appears to be not a libFAUDES source tree: abort"
    return
fi


############################################################################
# do it

# configure libFAUDES to go with libVIODES/DESTool 
# - minimum plug-ins: luabindings, timed, simulator, iodevice)
# - minimum debug: core_checked core_progress
# - minimum options: core_systime core_network core_thread
# - note: shared linkage is strictly required
export FAUDES_PLUGINS="synthesis observer diagnosis iosystem hiosys multitasking coordinationcontrol timed iodevice simulator luabindings"
export FAUDES_DEBUG="core_checked core_exceptions iop_threads iop_performance sim_sync core_progress"
export FAUDES_OPTIONS="core_systime core_network core_threads"
export FAUDES_LINKING=shared


# do copy/clean/configure/compile
echo ==================== copy source tree
rm -rf $FAUDES_DST
cp -R $FAUDES_SRC $FAUDES_DST 
echo ==================== clean libFAUDES
make -C $FAUDES_DST dist-clean
echo ==================== configure libFAUDES
make -C $FAUDES_DST configure 
echo ==================== build libFAUDES shared object
make -C $FAUDES_DST -j20


# clean environment
unset FAUDES_PLUGINS
unset FAUDES_DEBUG
unset FAUDES_OPTIONS
unset FAUDES_LINKING
unset FAUDES_PLATFROM


############################################################################
# done

if [ ! -f ${FAUDES_DST}/bin/luafaudes ]; then
    echo "error: something went wrong when compiling libFAUDES: abort"
    return
fi

# update version
. ./VERSION
qmake -set VIODES_VERSION_MAJOR $VIODES_VERSION_MAJOR 
qmake -set VIODES_VERSION_MINOR $VIODES_VERSION_MINOR 


echo "===== verify versions for qt by \"qmake -query\""
echo "===== you may proceed with \"qmake viodes.pro\""
echo "===== copy/configure/compile libFAUDES done"

