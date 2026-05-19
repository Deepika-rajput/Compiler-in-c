# Compiler in C

A multi-phase compiler built from scratch in C — no Lex/Yacc, just pure C.

## Phases

| Phase | Module |
|---|---|
| Lexical Analysis | `lexer.c` |
| Parsing (AST) | `parser.c` |
| Semantic Analysis | `semantic.c` |
| TAC Code Generation | `codegen.c` |
| Optimization | `codegen.c` |
| Assembly Output | `codegen.c` |

## Build & Run

```bash
# Using build script
chmod +x build.sh && ./build.sh

# Or manually
gcc main.c lexer.c parser.c semantic.c codegen.c -o compiler

# Run
./compiler <source_file>
```

## Project Structure

```
├── main.c          # Entry point
├── lexer.c/h       # Tokenizer
├── parser.c/h      # Recursive descent parser & AST
├── semantic.c/h    # Type checking & scope analysis
├── codegen.c/h     # TAC generation, optimization, assembly
├── test.c          # Sample input file
└── tokens.txt      # Lexer output
```

## Requirements

- GCC or any C99-compatible compiler
- Linux / macOS / Windows (MinGW or WSL)
