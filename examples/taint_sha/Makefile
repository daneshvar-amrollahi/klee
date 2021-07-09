#CC=llvm-gcc --emit-llvm
CC=clang-8 -emit-llvm

all: direct

direct: taint_sha256_direct.o
		echo 
		echo Testing...
		echo There should be no assertion fails!
		klee -taint=direct -allocate-determ -allocate-determ-start-address=0x00040000000 -allocate-determ-size=1000 --disable-verify -write-sym-paths -dump-call-traces -dump-call-trace-instructions -call-trace-instr-startfn=nf_core_process -call-trace-instr-endfn=nf_core_process -dump-call-trace-tree -dump-constraint-tree -solver-backend=z3 -exit-on-error -max-memory=750000 -search=dfs -condone-undeclared-havocs taint_sha256_direct.o
		echo 

# controlflow: taint_sha256_controlflow.o
# 		echo 
# 		echo Testing...
# 		echo There should be no assertion fails!
# 		klee -taint=controlflow taint_sha256_controlflow.o
# 		echo 

# regions: taint_sha256_regions.o
# 		echo 
# 		echo Testing...
# 		echo There should be no assertion fails!
# 		klee -taint=regions -debug-print-instructions=all:stderr taint_sha256_regions.o
# 		echo 

taint_sha256_direct.o: ./openssl/crypto/sha/sha256.taint.c
		$(CC) -O0 -DDIRECT -c ./openssl/crypto/sha/sha256.taint.c -I ../../include/ -I ./openssl/include/ -o taint_sha256_direct.o

# taint_sha256_controlflow.o: ./openssl/crypto/sha/sha256.taint.c
# 		$(CC) -O0 -DCONTROLFLOW -c ./openssl/crypto/sha/sha256.taint.c -I ../../include/ -I ./openssl/include/ -o taint_sha256_controlflow.o

# taint_sha256_regions.o: ./openssl/crypto/sha/sha256.taint.c
# 		$(CC) -O0 -DREGIONS -c ./openssl/crypto/sha/sha256.taint.c -I ../../include/ -I ./openssl/include/ -o taint_sha256_regions.o

clean:
		rm -Rf klee-*
		rm -f taint.o

