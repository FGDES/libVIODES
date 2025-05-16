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

# include faudes/viodes headers
DEFINES       += FAUDES_BUILD_APP
DEFINES       += VIODES_BUILD_APP

# link to faudes/viodes DSOs
LIBS          += -L$$VIODES_BASE -lviodes
LIBS          += -L$$VIODES_LIBFAUDES -lfaudes

# qmake paths
INCLUDEPATH += $$VIODES_LIBFAUDES/include 
INCLUDEPATH += $$VIODES_BASE/include 
OBJECTS_DIR = ./obj
MOC_DIR = ./obj

# mac os clang extra configuration
macx {
  LIBS += -rpath @executable_path
}

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

# copy via post link target
defineTest(viodes_copy) {
  src = $$1
  dst = $$2
  QMAKE_POST_LINK += $$QMAKE_MKDIR  $$shell_path($$dst) &
  for(file, src) {
    QMAKE_POST_LINK += $$QMAKE_COPY_FILE $$shell_path($$file) $$shell_path($$dst/) $$escape_expand(\\n\\t)
  }
  export(QMAKE_POST_LINK)
}

# linux: copy libVIODES and libFAUDES
unix:!macx {

  VIOEDIT_LIBS = $$VIODES_LIBFAUDES/libfaudes.so
  VIOEDIT_LIBS += $$VIODES_LIBFAUDES/include/libfaudes.rti
  VIOEDIT_LIBS += $$VIODES_BASE/libviodes.so
  VIOEDIT_LIBS += $$VIODES_BASE/vioedit/examples/vioconfig.txt

  VIODES_PLUGINS =  $$VIODES_BASE/libviogen.so
  VIODES_PLUGINS += $$VIODES_BASE/libviohio.so
  VIODES_PLUGINS += $$VIODES_BASE/libviomtc.so
  VIODES_PLUGINS += $$VIODES_BASE/libviosim.so
  VIODES_PLUGINS += $$VIODES_BASE/libviodiag.so
  VIODES_PLUGINS += $$VIODES_BASE/libviolua.so

  viodes_copy($$VIOEDIT_LIBS, ./lib)
  viodes_copy($$VIODES_PLUGINS, ./lib/plugins/viotypes)
  viodes_copy($$VIODES_LIBFAUDES/stdflx/*.flx, ./lib/plugins/luaextensions/)

}

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

# mac: fix library paths (disabled, now using strategic clang options)
macx { 
  # install_name_tool replacement commands for all our libraries
  ITF_LIBFAUDES = -change libfaudes.dylib @executable_path/libfaudes.dylib 
  ITF_LIBVIODES = -change libviodes.dylib @executable_path/libviodes.dylib 
  ITF_ALL = $$ITF_LIBFAUDES $$ITF_LIBVIODES 
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
  #QMAKE_POST_LINK += make macfix  ## (disabled)
}


# windows (both msys and msvc) 
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

  viodes_copy($$VIOEDIT_LIBS, ./release)
  viodes_copy($$VIODES_PLUGINS, ./release/plugins)
}

                


