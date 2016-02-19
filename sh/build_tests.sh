CPP_FILES="../src/cpp/ax/tests.cpp "
mkdir -p ./bin
$CXX -std=c++14 -Wall -Wextra -pedantic -g -O2 -pthread -march=native $CPP_FILES ./bin/libax.a -o ./bin/ax_tests.exe
