#!/bin/bash
gcc -o compiler main.c lexer.c parser.c semantic.c codegen.c -I. 
echo "Build complete. Run with ./compiler <test.c>"
