CC?=clang++-8
CFLAGS=-emit-llvm -O0 -DDIRECT -fno-exceptions -c
INCLUDE_DIRS=-I . -I ../../../include/ -I ../openssl/build/include/

KLEE_COMMAND=/usr/bin/time -v klee -allocate-determ -allocate-determ-start-address=0x00040000000 -allocate-determ-size=1000 \
							--disable-verify -write-sym-paths -dump-call-traces -dump-constraint-tree \
							--libc=uclibc --posix-runtime -taint=direct -link-llvm-lib=../openssl/build/crypto/libcrypto.bc \
							-solver-backend=z3 -exit-on-error -max-memory=750000 -search=dfs -condone-undeclared-havocs \
                                                        -const-array-opt=true -optimize-array=all

clean:
	rm -Rf klee-*
	rm -f *.bc

verify:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SSL_TARGET).cpp -o $(SSL_TARGET).bc
	$(KLEE_COMMAND) $(SSL_TARGET).bc
