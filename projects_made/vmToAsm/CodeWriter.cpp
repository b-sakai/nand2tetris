#include <cassert>
#include "CodeWriter.hpp"

CodeWriter::CodeWriter(string filename) {
    file.open(filename);
}

void CodeWriter::setFileName(string filename) {
    file.close();
    file.open(filename);
}

void CodeWriter::closeFile() {
    file.close();
}

// 与えられた算術コマンドをアセンブリコードに変換し、それを書き込む
void CodeWriter::writeArithmetic(string command) {
    if (command == "neg" || command == "not") { // １変数算術のとき
        file << "@SP" << endl;
        file << "A=M-1" << endl;
        if (command == "neg") {
            file << "M=-M" << endl;
        } else if (command == "not") {
            file << "M=!M" << endl;            
        } else {
            assert(false);
        }
    } else { // ２変数算術のとき
        // DレジスタにSP-1の位置の値を格納
        file << "@SP" << endl;
        file << "M=M-1" << endl;
        file << "A=M" << endl;
        file << "D=M" << endl;
        // AレジスタにSP-2の位置のアドレス値を格納        
        file << "@SP" << endl;
        file << "M=M-1" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        // 算術を実行し、Mアドレスに値を格納
        if (command == "add") {
            file << "M=M+D" << endl;
        } else if (command == "sub") {
            file << "M=M-D" << endl;
        } else if (command == "and") {
            file << "M=M&D" << endl;            
        } else if (command == "or") {
            file << "M=M|D" << endl;                        
        } else if (command == "eq" || command == "lt" || command == "gt") {
            file << "D=M-D" << endl;
            file << "@TRUE" << countForSymbol <<  endl;
            if (command == "eq") {
                file << "D;JEQ" << endl; // eq
            } else if (command == "lt") {
                file << "D;JLT" << endl; // lt
            } else if (command == "gt") {
                file << "D;JGT" << endl; // gt
            } else {
                assert(false);
            }
            file << "D=0" << endl;
            file << "@SP" << endl;
            file << "A=M" << endl;
            file << "M=D" << endl;
            file << "@FALSE" << countForSymbol << endl;
            file << "0;JMP" << endl;
            file << "(TRUE" << countForSymbol << ")" << endl;
            file << "D=-1" << endl;
            file << "@SP" << endl;
            file << "A=M" << endl;
            file << "M=D" << endl;
            file << "(FALSE" << countForSymbol << ")" << endl;
            countForSymbol += 1;
        } else {
            assert(false);
        }
        // スタックポインタの位置を修正
        file << "@SP" << endl;
        file << "M=M+1" << endl;        
    }
}

// C_PUSH C_POPコマンドをアセンブリコードに変換し、それを書き込む
void CodeWriter::writePushPop(string command, string segment, int index) {
    if (segment == "constant") {
        file << "@" << index << endl;
        file << "D=A" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
    }
    // スタックポインタの位置を修正
    file << "@SP" << endl;
    file << "M=M+1" << endl;            
}