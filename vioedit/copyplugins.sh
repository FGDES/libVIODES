# install plugins and shared objects for vioedit

# viodes base dir
VIODES=..
FAUDES=$VIODES/libFAUDES_for_VIODES

# app bundle dest
VIOAPP=lib

# do copy
mkdir -p $VIOAPP/plugins
mkdir -p $VIOAPP/plugins/viotypes

cp -p $VIODES/libviodes*so $VIOAPP
cp $FAUDES/libfaudes.so $VIOAPP
cp $FAUDES/include/libfaudes.rti $VIOAPP
cp $VIODES/vioedit/examples/vioconfig.txt $VIOAPP
cp $VIODES/libviogen*so $VIOAPP/plugins/viotypes
cp $VIODES/libviohio*so $VIOAPP/plugins/viotypes
cp $VIODES/libviomtc*so $VIOAPP/plugins/viotypes
cp $VIODES/libviosim*so $VIOAPP/plugins/viotypes
cp $VIODES/libviodiag*so $VIOAPP/plugins/viotypes
cp $VIODES/libviolua*so $VIOAPP/plugins/viotypes

