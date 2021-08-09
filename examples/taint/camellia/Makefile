CC=clang++-8 -emit-llvm

all: direct

direct: taint_camellia_direct.o
		echo
		echo Testing...
		echo There should be no assertion fails!
		../../../build/bin/klee --libc=uclibc --posix-runtime -taint=direct -max-memory=12000 -link-llvm-lib=../openssl/build/crypto/libcrypto.bc taint_camellia_direct.o
		echo

taint_camellia_direct.o: ./camellia.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./camellia.cpp -I . -I ../../../include/ -I ../openssl/build/include/ -o taint_camellia_direct.o

clean:
		rm -Rf klee-*
		rm -f taint_camellia_*.o

