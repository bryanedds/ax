all:ax

$(shell mkdir -p ./bin)

CPPFLAGS+=-std=c++14 -Wall -pedantic -Wextra
CPPFLAGS+=-g -O2
CPPFLAGS+=-pthread
CPPFLAGS+=-march=native
#CPPFLAGS+=-fsanitize=undefined,address
#CPPFLAGS+=-DNDEBUG -fno-omit-frame-pointer

CXX=g++

CPP_FILES= \
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
	../src/cpp/main.cpp

OBJ_FILES=$(patsubst .cpp,.o,$(CPP_FILES))

%.S:%.cpp
	$(CXX) $(CPPFLAGS) $^ -S -masm=intel -o - | egrep -v '\s*\.' | c++filt > $@

%.i:%.cpp
	$(CXX) $(CPPFLAGS) $^ -E -o $@

%.o:%.cpp
	$(CXX) $(CPPFLAGS) $^ -c -o $@

%.so:%.o
	$(CXX) $(CPPFLAGS) $^ -shared -o $@ $(LDFLAGS)

ax:$(OBJ_FILES)
	$(CXX) $(CPPFLAGS) $^ -o ./bin/$@ $(LDFLAGS)

.PRECIOUS: $(OBJ_FILES)
