# Bidirectional C ↔ NASM Compiler

A bidirectional compiler that translates between C source code and NASM assembly, and vice versa. This project aims to provide a reliable way to analyze, decompile, and regenerate source code from either language, with a focus on clarity, portability, and educational insight.

## Features
- ✅ C to NASM: Convert standard C code to clean, x86 NASM-compatible assembly.
- ✅ NASM to C: Reverse-engineer NASM assembly back into human-readable C code.
- ⚙️ Supports a subset of C (e.g., arithmetic, branching, functions).
- 📄 Intermediate Representation (IR) used for modular and extensible compilation.
- 🧪 Built-in test suite for validating transformations.
- 🔁 Round-trip transformations (C → NASM → C and NASM → C → NASM).

## Use cases
- 🔍 Reverse Engineering
- 📚 Learning Tool for understanding compilation and low-level code.
- 🔄 Cross-verification of assembly and C implementations.
- 🛠️ Compiler Design Experiments

## Usage
C to NASM

`bidic example.c -o example.asm`

NASM to C

`bidic -r example.asm -o example.c`

## Directory Structure

```
.
├── src/                # Contains all the source code
├── include/            # Libraries for the project
├── tests/              # Unit and integration tests
└── README.md
```

## Limitations
- Currently supports only a subset of the C language and x86 NASM instructions.
- Focus is on clarity and bidirectionality over performance or optimization.
- Complex C constructs (e.g., structs, pointers, macros) have limited or no support (yet).

## Roadmap


## License

## Acknowledgments

