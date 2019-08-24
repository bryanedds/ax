CPP_FILES="\
../src/cpp/blah/blah.cpp \
../src/cpp/tom/tom.cpp \
../src/cpp/ax/reflectable.cpp \
../src/cpp/ax/field.cpp \
../src/cpp/ax/type_descriptor.cpp \
../src/cpp/ax/parser.cpp \
../src/cpp/ax/address.cpp \
../src/cpp/ax/type.cpp \
../src/cpp/ax/name.cpp \
../src/cpp/ax/string.cpp \
../src/cpp/ax/system.cpp \
../src/cpp/ax/type_descriptors.cpp \
../src/cpp/ax/unparser.cpp "
mkdir -p ./bin
g++ -std=c++14 -Wall -Wextra -pedantic -g -O2 -pthread -march=native -D BLAH_NO_THREAD_SUPPORT -c $CPP_FILES
mv *.o ./bin
ar rcs ./bin/libax.a ./bin/*.o
rm ./bin/*.o
