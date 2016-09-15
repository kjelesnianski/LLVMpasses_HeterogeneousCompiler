# LLVMpasses_HeterogeneousCompiler

--------
Overview
--------

This repo contains several passes that are run over the LLVM IR in order to
adjust the linkage of somevariables. More specifically, these passes allow for
the compiler used in the Popcorn Linux project to properly generate aligned
binarys to be run on the current x86-64/aarch64 heterogeneous setup.

Associate-symbol2Literal pass
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
