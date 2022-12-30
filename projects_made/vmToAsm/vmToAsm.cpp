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
// 8.1.1: Basic Loop (labe, if-goto)
// 8.1.2: Fibonacci (goto)
// 8.2: Function Call
// 8.2.1: Simple Function (function, return) 
// 8.2.2: FibonacciElement (call, bootstrap, multiple vm file compile)
// 8.2.3: StaticTest (static variable)
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "Parser.hpp"
#include "CodeWriter.hpp"


//using std::filesystem::directory_iterator;
using namespace std;
using namespace std::filesystem;

int main() {
    // 読み込むファイルのパスを指定
    string ifilepath = "../08/FunctionCalls/StaticsTest/";
    vector<string> ifilenames;
    for (const auto& file: directory_iterator(ifilepath)) {
        string pathname = file.path().string();
        if (pathname.find(".vm") != string::npos) {
            ifilenames.push_back(file.path());
        }

    }

    //string ofilename = "./output/SimpleAdd.asm";
    string ofilename = "../08/FunctionCalls/StaticsTest/StaticsTest.asm";

    CodeWriter writer(ofilename);

    // 比較するためのアドホックコード
    reverse(ifilenames.begin(), ifilenames.end());

    for (string ifilename : ifilenames) {
    Parser parser(ifilename);
    cout << ifilename << endl;            
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
    }
    writer.closeFile();
    return 0;
}


