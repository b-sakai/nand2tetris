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
// Chapter 8
// 8.1: Program Flow
// 8.2: Function Call
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Parser.hpp"
#include "CodeWriter.hpp"

using namespace std;
int main() {
    // 読み込むファイルのパスを指定
    string ifilename = "../08/ProgramFlow/BasicLoop/BasicLoop.vm";
    //string ofilename = "./output/SimpleAdd.asm";
    string ofilename = "../08/ProgramFlow/BasicLoop/BasicLoop.asm";

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
            case C_LABEL:
                writer.writeLabel(parser.arg1);
                break;
            case C_GOTO:
                writer.writeGoto(parser.arg1);
                break;
            case C_IF:
                writer.writeIf(parser.arg1);
                break;
            case C_FUNCTION:
                writer.writeFunction(parser.arg1, parser.arg2);
                break;
            case C_RETURN:
                writer.writeReturn();
                break;
            case C_CALL:
                writer.writeCall(parser.arg1, parser.arg2);
                break;
            default:
                break;
        }
    }
    writer.closeFile();
    return 0;
}


