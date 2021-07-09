#CC=llvm-gcc --emit-llvm
CC=clang++-8 -emit-llvm
LD=llvm-link-8

all: direct

direct: taint_rsa_direct.o
		echo 
		echo Testing...
		echo There should be no assertion fails!
		klee -taint=direct taint_rsa_direct.o
		echo 

taint_rsa_direct.o: ./rsa_key_gen.cpp
		$(CC) -O0 -DDIRECT -fno-exceptions -c ./rsa_key_gen.cpp -I ../../include/ -I ./openssl/include/ -o taint_rsa_direct.o
		$(LD) -o taint_rsa_direct.o ./taint_rsa_direct.o ./openssl/libcrypto.bc
clean:
		rm -Rf klee-*
		rm -f taint_rsa_*.o

