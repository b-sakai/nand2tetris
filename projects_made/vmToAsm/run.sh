#!/bin/bash
g++ Parser.cpp CodeWriter.cpp vmToAsm.cpp --std=c++17 -lstdc++fs && ./a.out