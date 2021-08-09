CC=clang++-8 -emit-llvm

all: direct

direct: taint_ecdhe_direct.o
		echo
		echo Testing...
		echo There should be no assertion fails!
		../../../build/bin/klee --libc=uclibc --posix-runtime -taint=direct -max-memory=12000 -link-llvm-lib=../openssl/build/crypto/libcrypto.bc taint_ecdhe_direct.o -solver-backend=z3
		echo

taint_ecdhe_direct.o: ./ecdhe.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./ecdhe.cpp -I . -I ../../../include/ -I ../openssl/build/include/ -o taint_ecdhe_direct.o

clean:
		rm -Rf klee-*
		rm -f taint_ecdhe_*.o

