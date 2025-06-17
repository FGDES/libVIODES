# ##########################################
# libVIODES project file 
# tmoor 202401 transitioning to Qt6
# ##########################################

# source common setting
VIODES_BASE=./
VIODES_TARGET="vio"
include( "vio.pri" )

# say hello
message("=== libVIODES base at" $${VIODES_BASE})
message("=== libVIODES version" $${VERSION})
message("=== using Qt at "$$[QT_INSTALL_BINS])

# do subdirs
CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS  = ./viodes  \
           ./viogen/qtspline3 \ 
           ./viogen \ 
           ./viohio \
           ./viomtc \
           ./viodiag \
           ./viosim \
           ./violua 

SUBDIRS += ./vioedit



