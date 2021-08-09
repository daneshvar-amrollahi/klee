This repo contains features neccesary for the Bolt project.

KLEE Symbolic Virtual Machine

`KLEE` is a symbolic virtual machine built on top of the LLVM compiler
infrastructure. Currently, there are two primary components:

  1. The core symbolic virtual machine engine; this is responsible for
     executing LLVM bitcode modules with support for symbolic
     values. This is comprised of the code in lib/.

  2. A POSIX/Linux emulation layer oriented towards supporting uClibc,
     with additional support for making parts of the operating system
     environment symbolic.

Additionally, there is a simple library for replaying computed inputs
on native code (for closed programs). There is also a more complicated
infrastructure for replaying the inputs generated for the POSIX/Linux
emulation layer, which handles running native programs in an
environment that matches a computed test input, including setting up
files, pipes, environment variables, and passing command line
arguments.

For further information, see the [webpage](http://klee.github.io/).

## Leakage Analysis
From the *examples/taint* directory:
```bash
git clone https://github.com/ntauth/openssl.git
cd openssl
git checkout 3.0.0-cmake
```
To run an example from the *examples/taint* directory:
```bash
cd aes
make       # Run leakage analysis
make clean # Clean analysis artifacts
```