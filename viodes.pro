# ##########################################
# libVIODES project file 
# tmoor 202401 transitioning to Qt6
# ##########################################

# figure version
isEmpty( VIODES_VERSION_MAJOR ): VIODES_VERSION_MAJOR = $$[VIODES_VERSION_MAJOR]
isEmpty( VIODES_VERSION_MINOR ): VIODES_VERSION_MINOR = $$[VIODES_VERSION_MINOR]
isEmpty( VIODES_VERSION_MAJOR ): error("=== error libVIODES major version not configured")
isEmpty( VIODES_VERSION_MINOR ): error("=== error: libVIODES minor version not configured")

# say hello
message("=== libVIODES applications project file")
message("=== using Qt at "$$[QT_INSTALL_BINS])
message("=== building version "$${VIODES_VERSION_MAJOR}"."$${VIODES_VERSION_MINOR})

# do subdirs
CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS  = ./viocore  \
           ./plugins/viogen/qtspline3 \ 
           ./plugins/viogen \ 
#           ./plugins/viohio \
#           ./plugins/viomtc \
#           ./plugins/viodiag \
           ./plugins/viosim \
           ./plugins/violua 

unix:SUBDIRS += ./vioedit



