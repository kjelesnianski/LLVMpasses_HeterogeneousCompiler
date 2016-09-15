# LLVMpasses_HeterogeneousCompiler

--------
Overview
--------

This repo contains several passes that are run over the LLVM IR in order to
adjust the linkage of somevariables. More specifically, these passes allow for
the compiler used in the Popcorn Linux project to properly generate aligned
binarys to be run on the current x86-64/aarch64 heterogeneous setup.

Associate-stringLiteral pass
-----------------------------

This pass reads over LLVM IR, finds all anonymous string literal (e.g. the
strings showing up within the quotes of printfs), creates a unique symbol and
associates it with the given string literal. This allows the compiler &
alignment tool to align these literals as well as prevent address space clashes
across architectures.

Normally, anonymous string literals are dumped into the .rodata section at the
linker's discretion as a single section leading to mis-alignment and garbage
pointers across architectures (e.g. a pointer to a string literal on aarch64
may point to an address that is the middle of another string literal on x86-64).
This pass resolves that by assigning and giving unique names such that the 
alignement tool can put these literals in separate areas of virtual memory and
avoid clashes.

Example of anonymous string literal:

printf("I'm an anonymous string literal!\n");

The following "I'm an anonymous string literal" is the anonymous string literal
in that line of code.

Assign-staticVarSection pass
----------------------------

This pass also reads over the LLVM IR, finds all instances of static global
variables, and attaches the proper prefix such that they are inserted in the
correct section (.rodata, .data, .bss). It also assigns them to their own
section so that each global can be strictly aligned according to the compiler
alignment tool.

By default, clang (the LLVM front end compiler) lumps all static global
variables into one section within their linker section (.rodata, .data, .bss)
not allowing the alignment tool to otherwise align them.

Building & Installing the Passes
--------------------------------

NOTE: These passes were built using LLVM/clang 3.7.1 and are not guaranteed to
work for newer releases of LLVM/clang.

Make sure you have LLVM/clang installed before using these passes as they are
not standalone. For reference the following can be cloned to get the 3.7.1
release. (see [1] for more details)

    $ svn co http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_371/final

To build each of the passes:

1. Configuration with CMake - make & change into a build directory, then
   configure the installation.
```
    $ mkdir <pass_root>/build && cd <pass_root>/build
    
    $ cmake -DLLVM_DIR=/where/your/llvm/dir/is/cmake \
    -DCMAKE_INSTALL_PREFIX=/where/to/put/installed/pass/library \
    -DCMAKE_CXX_FLAGS="-std=c++11 -O3" \
    ..
```

2. Install the pass
```
    $ make install
```
[1] Building LLVM with CMake, http://llvm.org/docs/CMake.html

Running the Passes
------------------

To run the passes by themselves to see them in action:

1. Compile each src file to bitcode with
```
    $ clang -S -emit-llvm code.c
```
2. Run the pass with
```
    $ opt -load yourpass.so -yourpass *.ll
(e.g. $ opt -load Associate-stringLiteral/build/lib/libAssociateStringLiteral.so -associate-literal hellotest.ll )
```
NOTE: To load and register your pass within clang run the following
```
    $ clang -Xclang -load -Xclang /path/to/llvmpass.so
```
