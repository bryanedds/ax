cd $PSScriptRoot
$CPP_FILES = ("../src/cpp/tests/tests.cpp")
if (!(Test-Path "./bin")) { mkdir -p ./bin }
clang++ `
    -std=c++17 -Wall -Wextra -pedantic -g -O2 -pthread -march=native `
    -D BLAH_NO_THREAD_SUPPORT `
    -I "../include" `
    -o bin/ax_tests.exe `
    $CPP_FILES bin/libax.a
