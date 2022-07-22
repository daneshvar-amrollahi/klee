file=$1

# rm klee-last
# rm -r klee-out-*

echo "File TO RUN: $file"

clang -I ../../include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone $1

gdb --args klee -allocate-determ -allocate-determ-start-address=0x00040000000 -allocate-determ-size=1000 -libc=uclibc --external-calls=none --disable-verify -write-sym-paths -dump-call-traces -dump-call-trace-instructions -call-trace-instr-startfn=nf_core_process -call-trace-instr-endfn=nf_core_process -dump-call-trace-tree -dump-constraint-tree -solver-backend=z3 -exit-on-error -max-memory=750000 -search=dfs -condone-undeclared-havocs ${file::-2}.bc
# gdb --args klee ${file::-2}.bc

