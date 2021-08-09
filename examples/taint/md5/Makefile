CC=clang++-8 -emit-llvm

all: direct

direct: taint_md5_direct.o
		echo
		echo Testing...
		echo There should be no assertion fails!
		../../../build/bin/klee --libc=uclibc --posix-runtime -taint=direct -max-memory=12000 -link-llvm-lib=../openssl/build/crypto/libcrypto.bc taint_md5_direct.o
		echo

taint_md5_direct.o: ./md5.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./md5.cpp -I . -I ../../../include/ -I ../openssl/build/include/ -o taint_md5_direct.o

clean:
		rm -Rf klee-*
		rm -f taint_md5_*.o

