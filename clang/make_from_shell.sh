mkdir -p ./bin
clang++ -std=c++14 -Wall -Wno-vexing-parse -pedantic -g -O2 -pthread -march=native \
../src/cpp/blah/blah.cpp \
../src/cpp/tom/tom.cpp \
../src/cpp/ax/inspectable.cpp \
../src/cpp/ax/field.cpp \
../src/cpp/ax/type_descriptor.cpp \
../src/cpp/ax/parser.cpp \
../src/cpp/ax/address.cpp \
../src/cpp/ax/tests.cpp \
../src/cpp/ax/type.cpp \
../src/cpp/ax/name.cpp \
../src/cpp/ax/string.cpp \
../src/cpp/ax/type_descriptors.cpp \
../src/cpp/main.cpp \
-o ./bin/ax
