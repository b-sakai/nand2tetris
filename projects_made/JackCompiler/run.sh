#!/bin/bash
g++ JackAnalyzer.cpp JackTokenizer.cpp --std=c++17 -w -lstdc++fs `xml2-config --cflags --libs`
./a.out