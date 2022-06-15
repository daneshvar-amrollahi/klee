#!/bin/bash
# See usage().
set -eux

# Note that opam needs bash, not just sh
# Also it uses undefined variables so let's set them now otherwise it'll fail due to strict mode
if [ -z ${PERL5LIB+x} ]; then
  export PERL5LIB=''
fi
if [ -z ${MANPATH+x} ]; then
  export MANPATH=''
fi
if [ -z ${PROMPT_COMMAND+x} ]; then
  export PROMPT_COMMAND=''
fi

KLEE_RELEASE='master'
KLEE_UCLIBC_RELEASE='klee_uclibc_v1.2'
LLVM_RELEASE=8
Z3_RELEASE='z3-4.5.0'
OCAML_RELEASE='4.06.0'

## Utility functions

# Stop script if we do not have root access
check_sudo()
{
	echo 'Checking for sudo rights:'
	if ! sudo -v;
	then
		echo 'sudo rights not obtained, or sudo not installed.' >&2;
		exit 1;
	fi
}

# Detect the running operating system
# stdout: 'windows', 'docker' or 'linux'
detect_os()
{
	# Detect WSL
	case $(uname -r) in
		*Microsoft*)
			echo 'windows'
			return 0
			;;
	esac

	# Or docker ?
	if grep docker /proc/1/cgroup -qa;
	then
		echo 'docker'
		return 0
	fi

	# Use generic "linux" tag for the rest.
	# XXX: Support some more distributions ?
	echo 'linux'
	return 0
}


# Checks if a variable is set in a file. If it is not in the file, add it with
# given value, otherwise change the value to match the current one.
# $1 : the name of the file
# $2 : the name of the variable
# $3 : the value to set
line()
{
	if grep "^export $2" "$1" >/dev/null;
	then
		# Using sed directly to change the value would be dangerous as
		# we would need to correctly escape the value, which is hard.
		sed -i "/^export $2/d" "$1"
	fi
	echo "export ${2}=${3}" >> "$1"
}

# Same as line, but without the unicity checks.
# $1 : the name of the file
# $2 : the name of the variable
# $3 : the value to set
line_multi()
{
	if ! grep "^export ${2}=${3}" "$1" >/dev/null;
	then
		echo "export ${2}=${3}" >> "$1"
	fi
}

# Install arguments using system's package manager.
# XXX: Make the package manager depend on "$OS".
# shellcheck disable=SC2086
package_install()
{
	# Concatenate arguments into a list
	old_ifs="$IFS"
	IFS=' '
	packages="$*"
	IFS="$old_ifs"

	sudo apt-get install -yqq $packages
}

# Update list of available packages.
# XXX: Make the package manager depend on "$OS".
package_sync()
{
	sudo apt-get update -qq
}

# Print script usage.
usage()
{
	cat <<- EOF
	Usage: $0 [-p <dir>] [-i <component>] [-c <component>]
	       $0 -h
	
	       When invoked with no options, $0 downloads and configures all
	       the required dependencies; else using -i and -c, one can select
	       the components to be installed or cleaned.
	       The -p option sets the directory to use as a root for the project: by default,
	       the parent of the bolt directory is used.
	       This script generates a paths.sh file that must be sourced to
	       obtain the correct working environment.
	       Components: dpdk, pin, z3, klee-uclibc, klee
	EOF
}

## Constants
KLEEDIR="$(dirname "$(realpath "$0")")"
BUILDDIR="$(realpath -e "$KLEEDIR"/..)"
PATHSFILE="$BUILDDIR/paths.sh"
KERNEL_VER=$(uname -r | sed 's/-Microsoft//')
OS="$(detect_os)"

## Clean and installation routines

source_install_z3()
{
	cd "$BUILDDIR"
	if [ -d 'z3/.git' ];
	then
		cd z3;
		git fetch && git checkout "$Z3_RELEASE"
	else
		git clone --depth 1 --branch "$Z3_RELEASE" https://github.com/Z3Prover/z3 "$BUILDDIR/z3"
		cd z3;
	fi

	if  [ ! -f "build/z3" ] || [ ! "z3-$(build/z3 --version | cut -f3 -d' ')" = "$Z3_RELEASE" ];	then
		python scripts/mk_make.py -p "$BUILDDIR/z3/build"
		cd build
		make -kj || make
		make install
	fi
}

clean_z3()
{
	cd "$BUILDDIR"
	rm -rf z3
}

bin_install_llvm()
{
	package_install bison flex zlib1g-dev libncurses5-dev libpcap-dev
  package_install clang-$LLVM_RELEASE llvm-$LLVM_RELEASE llvm-$LLVM_RELEASE-dev llvm-$LLVM_RELEASE-tools
}

source_install_klee_uclibc()
{
	cd "$BUILDDIR"
	if [ -d 'klee-uclibc/.git' ];
	then
		cd klee-uclibc
		git fetch && git checkout "$KLEE_UCLIBC_RELEASE"
	else
		git clone --depth 1 --branch "$KLEE_UCLIBC_RELEASE" https://github.com/klee/klee-uclibc.git "$BUILDDIR/klee-uclibc";
		cd klee-uclibc
	fi

	./configure \
		--make-llvm-lib \
		--with-llvm-config="/usr/bin/llvm-config-$LLVM_RELEASE" \
		--with-cc="/usr/bin/clang-$LLVM_RELEASE"

	cp "$KLEEDIR/klee-uclibc.config" '.config'
	make -kj
}

clean_klee_uclibc()
{
	cd "$BUILDDIR"
	rm -rf klee-uclibc
}

source_install_klee()
{
  pushd "$KLEEDIR"
    git checkout $KLEE_RELEASE
    
    [ -d "$KLEEDIR/build" ] || mkdir "$KLEEDIR/build"

    pushd "$KLEEDIR/build"

        [ -f "Makefile" ] || CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" \
                            CMAKE_PREFIX_PATH="$KLEEDIR/../z3/build" \
                            CMAKE_INCLUDE_PATH="$KLEEDIR/../z3/build/include/" \
                            cmake \
                                -DENABLE_UNIT_TESTS=OFF \
                                -DBUILD_SHARED_LIBS=OFF \
                                -DLLVM_CONFIG_BINARY="/usr/bin/llvm-config-$LLVM_RELEASE" \
                                -DLLVMCC="/usr/bin/clang-$LLVM_RELEASE" \
                                -DLLVMCXX="/usr/bin/clang++-$LLVM_RELEASE" \
                                -DENABLE_SOLVER_Z3=ON \
                                -DENABLE_KLEE_UCLIBC=ON \
                                -DKLEE_UCLIBC_PATH="$KLEEDIR/../klee-uclibc" \
                                -DENABLE_POSIX_RUNTIME=ON \
                                -DCMAKE_BUILD_TYPE=Debug \
                                -DENABLE_KLEE_ASSERTS=ON \
                                -DENABLE_DOXYGEN=ON \
                                ..


        make -kj $(nproc)

    popd

    # Instruction Tracer
    pushd trace-instructions
        make clean && make
    popd

    # Dependencies for tree generation
    package_install python3 python3-pip
    sudo python3 -m pip install anytree sympy delegator
    opam install core

    line_multi "$PATHSFILE" 'PATH' "$KLEEDIR/build/bin:\$PATH"

    . "$PATHSFILE"
  popd
}

bin_install_ocaml()
{
	# we depend on an OCaml package that needs libgmp-dev
	package_install opam m4 libgmp-dev

	opam init -y
	eval "$(opam config env)"
	# Opam 1.x doesn't have "create", later versions require it but only the first time
	if opam --version | grep '^1.' >/dev/null ; then
		opam switch $OCAML_RELEASE
	else
		opam switch list
		if ! opam switch list 2>&1 | grep -Fq 4.06 ; then
			opam switch create $OCAML_RELEASE
		fi
	fi

	line_multi "$PATHSFILE" 'PATH' "$HOME/.opam/system/bin:\$PATH"
    # `|| :` at the end of the following command ensures that in the event the
    # init.sh script fails, the shell will not exit. opam suggests we do this.
	echo ". $HOME/.opam/opam-init/init.sh || :" >> "$PATHSFILE"
	. "$PATHSFILE"

	# Codegenerator dependencies.
	opam install goblint-cil core -y
	opam install ocamlfind num -y
	opam install ocamlfind sexplib menhir -y

}

clean_ocaml()
{
	rm -rf $HOME/.opam
}

# Options

# Environment
check_sudo
package_sync

# Common dependencies
package_install \
	build-essential \
	curl \
	git \
	libgoogle-perftools-dev \
	python2.7 \
	python3-minimal \
	python3-pip \
	parallel \
	gcc-multilib \
	graphviz \
	libnuma-dev \
	cmake

#Install things
source_install_z3
bin_install_llvm
source_install_klee_uclibc
bin_install_ocaml
source_install_klee