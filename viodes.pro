# ##########################################
# libVIODES project file 
# tmoor 202401 transitioning to Qt6
# ##########################################

# source common setting
VIODES_BASE=./
VIODES_TARGET="all"
include( "viodes.pri" )

# say hello
message("=== libVIODES base at" $${VIODES_BASE})
message("=== libVIODES version" $${VIODES_VERSION})
message("=== using Qt at "$$[QT_INSTALL_BINS])

# do subdirs
CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS  = ./viocore  \
           ./plugins/viogen/qtspline3 \ 
           ./plugins/viogen \ 
           ./plugins/viohio \
           ./plugins/viomtc \
           ./plugins/viodiag \
           ./plugins/viosim \
           ./plugins/violua 

SUBDIRS += ./vioedit



