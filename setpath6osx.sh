echo ==================== "select qt release"

export PATH=~/Qt/6.6.2/macos/bin:$PATH
export PATH=~/Qt/6.6.2/macos/libexec:$PATH
echo using "$(which qmake)"

echo ==================== "set for make mode"
export QMAKESPEC=macx-clang

#echo ==================== "set to xcode mode"
#export QMAKESPEC=macx-xcode


