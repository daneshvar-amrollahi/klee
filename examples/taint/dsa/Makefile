CC=clang++-8 -emit-llvm

all: direct

direct: taint_dsa_direct.o
		echo
		echo Testing...
		echo There should be no assertion fails!
		../../../build/bin/klee --libc=uclibc --posix-runtime -taint=direct -max-memory=16000 -link-llvm-lib=../openssl/build/crypto/libcrypto.bc taint_dsa_direct.o --external-calls=none -solver-backend=z3
		echo

taint_dsa_direct.o: ./dsa.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./dsa.cpp -I . -I ../../../include/ -I ../openssl/build/include/ -o taint_dsa_direct.o

clean:
		rm -Rf klee-*
		rm -f taint_dsa_*.o

