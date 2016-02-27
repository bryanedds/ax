all: lib tests

lib:
	set -v; \
	set -e; \
	cd ./sh; \
	. ./build_lib.sh

tests:
	set -v; \
	set -e; \
	cd ./sh; \
	. ./build_tests.sh