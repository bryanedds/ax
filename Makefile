all: tests

lib:
	set -v; \
	set -e; \
	cd ./sh; \
	. ./build_lib.sh

tests: lib
	set -v; \
	set -e; \
	cd ./sh; \
	. ./build_tests.sh