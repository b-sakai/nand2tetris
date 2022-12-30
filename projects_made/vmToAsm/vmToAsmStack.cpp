// .vmから.amに変換するプログラム（※7章の内容まで対応）
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
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Parser.hpp"
#include "CodeWriter.hpp"

using namespace std;
int main() {
    // 読み込むファイルのパスを指定
    string ifilename = "../07/MemoryAccess/PointerTest/PointerTest.vm";
    //string ofilename = "./output/SimpleAdd.asm";
    string ofilename = "../07/MemoryAccess/PointerTest/PointerTest.asm";

    Parser parser(ifilename);
    CodeWriter writer(ofilename);    
    while (true) {
        parser.advance();
        if (parser.hasMoreCommands == false) {
            break;
        }
        string cmd = parser.command;
        switch (parser.commandType) {
            case C_ARITHMETIC:
                writer.writeArithmetic(cmd);
                break;
            case C_PUSH:
                writer.writePush(cmd, parser.arg1, parser.arg2);
                break;
            case C_POP:
                writer.writePop(cmd, parser.arg1, parser.arg2);
                break;
            default:
                break;
        }
    }
    writer.closeFile();
    return 0;
}


