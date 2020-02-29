cd $PSScriptRoot
$CPP_FILES =
   ("../src/cpp/blah/blah.cpp",
    "../src/cpp/tom/tom.cpp",
    "../src/cpp/ax/reflectable.cpp",
    "../src/cpp/ax/field.cpp",
    "../src/cpp/ax/type_descriptor.cpp",
    "../src/cpp/ax/parser.cpp",
    "../src/cpp/ax/address.cpp",
    "../src/cpp/ax/type.cpp",
    "../src/cpp/ax/name.cpp",
    "../src/cpp/ax/string.cpp",
    "../src/cpp/ax/system.cpp",
    "../src/cpp/ax/type_descriptors.cpp",
    "../src/cpp/ax/unparser.cpp")
if (!(Test-Path "bin")) { mkdir -p bin }
clang++ `
    -std=c++17 -Wall -Wextra -pedantic -g -O2 -pthread -march=native `
    -D BLAH_NO_THREAD_SUPPORT `
    -I ../include `
    -c $CPP_FILES
mv *.o bin
llvm-ar rcs bin/libax.a bin/*.o
rm bin/*.o