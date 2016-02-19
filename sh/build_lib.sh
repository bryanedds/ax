. ./declare_lib_cpp_files.sh
mkdir -p ./bin
$CXX -std=c++14 -Wall -Wextra -pedantic -g -O2 -pthread -march=native -c $LIB_CPP_FILES
mv *.o ./bin
ar rcs ./bin/libax.a ./bin/*.o
rm ./bin/*.o