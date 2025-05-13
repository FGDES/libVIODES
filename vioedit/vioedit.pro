#
# project file for vioedit, tmoor 2024
#
#

# set paths for dependant libraries
VIODES_BASE = ..
VIODES_LIBFAUDES = $$VIODES_BASE/libFAUDES_for_VIODES

# target setting
TEMPLATE = app
LANGUAGE = C++
QT += core gui svg widgets

# target name
unix:TARGET  =lib/vioedit.bin
macx:TARGET  =VIOEdit
win32:TARGET = VIOEdit

# lib faudes/viodes
DEFINES       += FAUDES_BUILD_APP
DEFINES       += VIODES_BUILD_APP
LIBS          += -L$$VIODES_BASE -lviodes
LIBS          += -L$$VIODES_LIBFAUDES -lfaudes

# qmake paths
INCLUDEPATH += $$VIODES_LIBFAUDES/include 
INCLUDEPATH += $$VIODES_BASE/include 
OBJECTS_DIR = ./obj
MOC_DIR = ./obj



# lsb compiler options
linux-lsb-g++:LIBS   += --lsb-shared-libs=faudes:luafaudes:viodes

# win32 MSVC extra configuration 
win32-msvc {
  QMAKE_CXXFLAGS += /EHsc 
  LIBS += -lwsock32
  DEFINES += VIO_WINCONSOLE
}

# win32 MSYS extra configuration 
win32-g++ {
  LIBS += -lwsock32
  DEFINES += VIO_WINCONSOLE
}
  
# vioedit sources
HEADERS      += src/vioedit.h                 
SOURCES      += src/vioedit.cpp

# application icon
ICON = ./images/icon_osx.icns 
RC_FILE = ./images/icon_win.rc

##### below this line: copy libraries in place

# mac: copy libfaudes/libviodes to bundle 
macx { 
  ContFiles.files += $$VIODES_LIBFAUDES/libfaudes.dylib
  ContFiles.files += $$VIODES_LIBFAUDES/include/libfaudes.rti 
  ContFiles.files += $$VIODES_BASE/libviodes.dylib
  ContFiles.files += $$VIODES_BASE/vioedit/examples/vioconfig.txt 
  ContFiles.path = Contents/MacOS
  QMAKE_BUNDLE_DATA += ContFiles
  ViopFiles.files +=  $$VIODES_BASE/libviogen.dylib
  ViopFiles.files +=  $$VIODES_BASE/libviohio.dylib
  ViopFiles.files +=  $$VIODES_BASE/libviomtc.dylib
  ViopFiles.files +=  $$VIODES_BASE/libviodiag.dylib
  ViopFiles.files +=  $$VIODES_BASE/libviosim.dylib
  ViopFiles.files +=  $$VIODES_BASE/libviolua.dylib
  ViopFiles.path = Contents/plugins/viotypes
  QMAKE_BUNDLE_DATA += ViopFiles
}

# mac: fix library paths
macx { 
  # install_name_tool replacement commands for all our libraries
  ITF_LIBFAUDES = -change libfaudes.dylib @executable_path/libfaudes.dylib 
  ITF_LIBVIODES = -change libviodes.dylib @executable_path/libviodes.dylib 
#  ITF_LIBVIOGEN = -change libviogen.dylib @executable_path/../plugins/viotypes/libviogen.dylib 
  ITF_ALL = $$ITF_LIBFAUDES $$ITF_LIBVIODES $$ITF_LIBVIOGEN
  QMAKE_EXTRA_TARGETS += macfix
  macfix.target = macfix
  macfix.commands += \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/MacOS/VIOEdit && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/MacOS/libviodes.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviogen.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviohio.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviomtc.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviodiag.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviosim.dylib && \
    install_name_tool $$ITF_ALL VIOEdit.app/Contents/plugins/viotypes/libviolua.dylib
 QMAKE_POST_LINK += make macfix
}


# 
win32 {

  VIOEDIT_LIBS = $$VIODES_LIBFAUDES/faudes.dll
  VIOEDIT_LIBS += $$VIODES_LIBFAUDES/include/libfaudes.rti
  VIOEDIT_LIBS += $$VIODES_BASE/viodes.dll
  VIOEDIT_LIBS += $$VIODES_BASE/vioedit/examples/vioconfig.txt

  VIODES_PLUGINS =  $$VIODES_BASE/viogen.dll
  VIODES_PLUGINS += $$VIODES_BASE/viohio.dll
  VIODES_PLUGINS += $$VIODES_BASE/viomtc.dll
  VIODES_PLUGINS += $$VIODES_BASE/viosim.dll
  VIODES_PLUGINS += $$VIODES_BASE/viodiag.dll
  VIODES_PLUGINS += $$VIODES_BASE/violua.dll

  INSTCMD = \
    cp $$VIODES_PLUGINS ./release/plugins && \
    cp $$VIOEDIT_LIBS ./release && \
    rm -f ./release/qt.conf && \
    rm -f ./release/luafaudes.flx 

  QMAKE_EXTRA_TARGETS += instlibs
  instlibs.target = instlibs
  instlibs.commands += $$INSTCMD
  QMAKE_POST_LINK += make instlibs

}

                


