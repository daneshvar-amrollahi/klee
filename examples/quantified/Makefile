# Get current dir, see https://stackoverflow.com/a/8080530
SELF_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# very hacky but oh well
KLEE_DIR=$(shell dirname $$(which klee))/../..

SUFFIX=.c

CC=clang


CFLAGS+=-emit-llvm -O0 -DDIRECT -fno-exceptions -c -Xclang -disable-O0-optnone
INCLUDE_DIRS+=-I . -I $(SELF_DIR)/../../include/

KLEE_COMMAND=/usr/bin/time -v klee -allocate-determ -allocate-determ-start-address=0x00040000000 -allocate-determ-size=1000 \
							--disable-verify -write-sym-paths -dump-call-traces -dump-call-trace-tree -dump-constraint-tree \
							-solver-backend=z3 -exit-on-error -max-memory=750000 -search=dfs -condone-undeclared-havocs \
              -dump-call-trace-instructions -call-trace-instr-startfn=main -call-trace-instr-endfn=main \

clean:
	rm -Rf klee-*
	rm -f *.bc

verify:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(QUANTIFIER_TARGET)$(SUFFIX) -o $(QUANTIFIER_TARGET).bc
	$(KLEE_COMMAND) $(QUANTIFIER_TARGET).bc

SHELL=/bin/bash