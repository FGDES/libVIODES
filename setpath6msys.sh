echo ==================== "select qt release and matching toolchain"

export PATH=/C/Qt/Tools/mingw1310_64/bin:$PATH
echo using MinGW "$(which g++)"
export PATH=/C/Qt/6.8.3/mingw_64/bin:$PATH
echo using Qt "$(which qmake)"


