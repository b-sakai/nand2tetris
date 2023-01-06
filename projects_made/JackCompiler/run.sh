#!/bin/bash
g++ JackAnalyzer.cpp CompilationEngine.cpp JackTokenizer.cpp SymbolTable.cpp VMWriter.cpp --std=c++17 -w -lstdc++fs && ./a.out && cat ./output.vm 