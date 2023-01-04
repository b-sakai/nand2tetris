#!/bin/bash
g++ JackAnalyzer.cpp CompilationEngine.cpp JackTokenizer.cpp --std=c++17 -w -lstdc++fs && ./a.out