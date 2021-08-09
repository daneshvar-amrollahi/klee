CC=clang++-8 -emit-llvm

all: direct

direct: taint_aria_direct.o
		echo
		echo Testing...
		echo There should be no assertion fails!
		../../../build/bin/klee --libc=uclibc --posix-runtime -taint=direct -max-memory=12000 -link-llvm-lib=../openssl/build/crypto/libcrypto.bc taint_aria_direct.o
		echo

taint_aria_direct.o: ./aria.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./aria.cpp -I . -I ../../../include/ -I ../openssl/build/include/ -o taint_aria_direct.o

clean:
		rm -Rf klee-*
		rm -f taint_aria_*.o

