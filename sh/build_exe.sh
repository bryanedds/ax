. ./declare_exe_cpp_files.sh
mkdir -p ./bin
$CXX -std=c++14 -Wall -Wextra -pedantic -g -O2 -pthread -march=native $EXE_CPP_FILES -o ./bin/ax.exe
