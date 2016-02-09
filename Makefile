CPPFLAGS += -std=c++14 -Wall -Wextra -pedantic
CPPFLAGS += -g -O2
CPPFLAGS += -pthread
CPPFLAGS += -march=native
#CPPFLAGS += -fsanitize=undefined,address
#CPPFLAGS += -DNDEBUG

CPP_FILES = \
	src/cpp/blah/blah.cpp \
	src/cpp/tom/tom.cpp \
	src/cpp/ax/reflectable.cpp \
	src/cpp/ax/field.cpp \
	src/cpp/ax/type_descriptor.cpp \
	src/cpp/ax/parser.cpp \
	src/cpp/ax/address.cpp \
	src/cpp/ax/tests.cpp \
	src/cpp/ax/type.cpp \
	src/cpp/ax/name.cpp \
	src/cpp/ax/string.cpp \
	src/cpp/ax/type_descriptors.cpp \
	src/cpp/main.cpp

OBJ_FILES=$(subst src,bin/obj,$(subst .cpp,.o,$(CPP_FILES)))


all: bin/ax

bin/ax: $(OBJ_FILES)
	$(CXX) $(CPPFLAGS) $^ -o bin/ax $(LDFLAGS)

clean:
	rm -rf bin

.PRECIOUS: $(OBJ_FILES)


%.S: %.cpp
	$(CXX) $(CPPFLAGS) $^ -S -masm=intel -o - | egrep -v '\s*\.' | c++filt > $@

%.i: %.cpp
	$(CXX) $(CPPFLAGS) $^ -E -o $@

bin/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $^ -c -o $@

%.so: %.o
	$(CXX) $(CPPFLAGS) $^ -shared -o $@ $(LDFLAGS)
