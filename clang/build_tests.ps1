cd $PSScriptRoot
$CppFiles = ("../src/cpp/tests/tests.cpp")
clang++ `
    -std=c++17 -Wall -Wextra -pedantic -g -O2 -pthread -march=native `
    -D BLAH_NO_THREAD_SUPPORT `
    -I ../include -I ../src/hpp `
    -o bin/ax_tests.exe `
    $CppFiles `
    bin/libax.a
