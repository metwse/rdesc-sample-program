# librdesc sample program
This repository contains a sample program to demonstrate the basic usage of
librdesc.

librdesc is integrated using Make. Check out the
[Quick Integration Guide](https://github.com/metwse/rdesc/tree/8ada5cd61955e3d60cdcf5c1e2a453d6fb9bc05e?tab=readme-ov-file#quick-integration-guide)
for details.

## Code Organization
`lex.c` and `lex.h` contain a simple lexer. It recognizes the logic symbols
`true`, `false`, `¬`, `∧`, `∨`, `→`, and `↔`, along with parentheses and
semicolons. A minimal lexer is included to keep the example self-contained.

`grammar.h` declares the `librdesc` grammar, and `grammar.c` defines it. The
main function in `main.c` drives the parser pump. Check the
[Quick Start](https://metwse.github.io/rdesc/quick_start.html) guide for
implementation details.

## Features
The grammar evaluates basic logical expressions. The operators are ordered by
descending precedence:
1. Unary NOT `¬`
2. Binary AND `∧` (left-associative)
3. Binary OR `∨` (left-associative)
4. Binary IF `→` (right-associative)
5. Binary IFF `↔` (left-associative)

## Building
Just run `make`!

There are two executable targets:
- `balg` (default): The main interpreter.
- `balg-tests`: The test suite executable.

Running `make` (or `make balg`) will automatically build librdesc and compile
the main program. `-j` flag is recommend (e.g., `make -j`) for faster (thanks
to parallelism) builds.
