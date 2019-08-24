CPP_FILES="../src/cpp/tests/tests.cpp "
mkdir -p ./bin
g++ -std=c++14 -Wall -Wextra -pedantic -g -O2 -pthread -march=native -D BLAH_NO_THREAD_SUPPORT $CPP_FILES ./bin/libax.a -o ./bin/ax_tests.exe

