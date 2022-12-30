// .vmから.amに変換するプログラム
// コンピュータの理論と実装第7章
// 実装の順番としては以下のように行う
// 7.3.2 VMTranslater設計案
// Class VMTranslater
// Class Parser
// Class CodeWriter
// Chapter 7
// 7.1: Stack Arithmetic
// 7.1.1 add, sub, neg, eq, gt, lt, and, or, not -> 単純に変換すれば良い
// 7.1.2 push constant x -> 単純に変換すれば良い
// Stage 2: Memory Access
// Chapter 8
// Stage 3: Parser Module, CodeWriter Module
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Parser.hpp"
#include "CodeWriter.hpp"

using namespace std;

void writeTest() {
    ofstream file("writeTest.txt");
    vector<string> fruits = {"apple", "strawberry", "pear", "grape" };

    for (const auto fruit : fruits) {
        file << fruit << endl;
    }
}


int main() {
    // 読み込むファイルのパスを指定
    string ifilename = "../07/StackArithmetic/StackTest/StackTestSimple.vm";
    string ofilename = "./output/StackTest.asm";

    Parser parser(ifilename);
    CodeWriter writer(ofilename);    
    while (true) {
        parser.advance();
        if (parser.hasMoreCommands == false) {
            break;
        }
        string cmd = parser.command;
        cout << parser.command << endl;
        cout << "call writeArithmetic " << parser.commandType << endl;
        switch (parser.commandType) {
            case C_ARITHMETIC:
                writer.writeArithmetic(cmd);
                break;
            case C_PUSH:
                writer.writePushPop(cmd, parser.arg1, parser.arg2);
                break;
            case C_POP:
                writer.writePushPop(cmd, parser.arg1, parser.arg2);
                break;
            default:
                break;
        }
    }



    writeTest();

    return 0;
}


