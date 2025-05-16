# ##########################################
# qmake include for libFAUDES based libraries
# 
# This project file supports the configuration of Qt library projects 
# that link against libFAUDES. It is complemented by the shell scripts 
# "copyfaudes.sh" and "distclean.sh", operational on MacOsX and Linux 
# platforms. Effectively, a properly configured and compiled libFAUDES
# is expected to reside in the specified path.
# 
# Relevant settings in this version are tailored for directory layout
# used for libVIODES including its Qt plug-ins
#
# Referenced variables, to be set in the main project file
# - VIODES_TARGET          target name, e.g., viodes, viogen etc.
# - VIODES_BASE            base dir of libVIODES, must contain ./libFAUDES_for_VIODES
#
# tmoor 202505
# ##########################################

# retrieve version
! include( VERSION ) {
  error("vio.pri: VERSION not found")
}
VERSION = $${VIODES_VERSION_MAJOR}.$${VIODES_VERSION_MINOR}

# say hello
message("=== libVIODES component" $${VIODES_TARGET})

# qmake target setting
TEMPLATE = lib
CONFIG += shared plugin
TARGET = $$VIODES_TARGET
DESTDIR = $$VIODES_BASE
LANGUAGE = C++
QT += core gui svg widgets printsupport

# have libFAUDES and viodes core
VIODES_LIBFAUDES = $$VIODES_BASE/libFAUDES_for_VIODES
VIODES_INCLUDE = $$VIODES_BASE/include

# dll export/import switch
DEFINES += $$upper($${VIODES_TARGET})_BUILD_LIB
DEFINES += FAUDES_BUILD_APP

# platform dependent library names
unix {
  VIODES_LIBFAUDES_DSO = -L$${VIODES_LIBFAUDES} -lfaudes
  VIODES_LIBVIODES_DSO = -L$${VIODES_BASE} -lviodes
  VIODES_LIBVIOGEN_DSO = -L$${VIODES_BASE} -lviogen
}
win32-g++ {
  VIODES_LIBFAUDES_DSO = $${VIODES_LIBFAUDES}\\faudes.lib
  VIODES_LIBVIODES_DSO = $${VIODES_BASE}\\libviodes.a
  VIODES_LIBVIOGEN_DSO = $${VIODES_BASE}\\libviogen.a
}
win32-msvc {
  VIODES_LIBFAUDES_DSO = $${VIODES_LIBFAUDES}\faudes.lib
  VIODES_LIBVIODES_DSO = $${VIODES_BASE}\viodes.lib
  VIODES_LIBVIOGEN_DSO = $${VIODES_BASE}\viogen.lib
}

# link to libviodes and libfaudes
LIBS += $${VIODES_LIBFAUDES_DSO} 
!equals( VIODES_TARGET , viodes ) {
  LIBS += $${VIODES_LIBVIODES_DSO}
}

# for thorse who want to link with viogen
VIODES_LIBVIOGEN_INC =  $${VIODES_BASE}/viogen/include

# macx force clang to have install name with rpath
macx {
  LIBS += -install_name @rpath/lib$${VIODES_TARGET}.dylib 
}

# force win32 plugin to release mode
win32: CONFIG -= debug
win32: CONFIG += release

# extra cflags
#win32: QMAKE_CXXFLAGS += /EHsc 


# pass on directory layout to qmake
INCLUDEPATH += $$VIODES_LIBFAUDES/include 
INCLUDEPATH += $$VIODES_INCLUDE
INCLUDEPATH += ./include
INCLUDEPATH += ./src
OBJECTS_DIR = ./obj
MOC_DIR = ./obj


# pass on config to compiler
DEFINES += VIODES_VERSION='\\"$${VERSION}\\"'
DEFINES += VIODES_TARGET='\\"$${VIODES_TARGET}\\"'

