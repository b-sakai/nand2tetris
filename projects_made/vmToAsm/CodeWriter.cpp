#include <cassert>
#include "CodeWriter.hpp"

CodeWriter::CodeWriter(string filename) {
    file.open(filename);
    writeInit();
}

// VMの初期化を行うアセンブリコードを書く。
void CodeWriter::writeInit() {

}



void CodeWriter::setFileName(string filename) {
    file.close();
    file.open(filename);
    writeInit();    
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

// C_PUSHコマンドをアセンブリコードに変換し、それを書き込む
void CodeWriter::writePush(string command, string segment, int index) {
    // レジスタへ値を読み込み
    if (segment == "constant") {
        file << "@" << index << endl;
        file << "D=A" << endl;
    } else if (segment == "static") {
        file << "@ST" << index << endl;
        file << "D=M" << endl;
    } else if (segment == "local") {
        file << "@LCL" << endl;
    } else if (segment == "argument") {
        file << "@ARG" << endl;
    } else if (segment == "this") {
        file << "@THIS" << endl;
    } else if (segment == "that") {
        file << "@THAT" << endl;
    } else if (segment == "pointer") {
        file << "@R" << 3 + index << endl;
        file << "D=M" << endl;
    } else if (segment == "temp") {
        file << "@R" << 5 + index << endl;
        file << "D=M" << endl;
    }
    if (segment == "local" || segment == "argument" || segment == "this"
     || segment == "that") {
        file << "D=M" << endl;
        file << "@" << index << endl;
        file << "A=D+A" << endl;
        file << "D=M" << endl;
    }

    // スタックポインタに書き込み
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;    
    // スタックポインタの位置を修正
    file << "@SP" << endl;
    file << "M=M+1" << endl;            
}

// C_POPコマンドをアセンブリコードに変換し、それを書き込む
void CodeWriter::writePop(string command, string segment, int index) {
    if (segment == "static") {
        file << "@ST" << index << endl;
    } else if (segment == "local") {
        file << "@LCL" << endl;
    } else if (segment == "argument") {
        file << "@ARG" << endl;
    } else if (segment == "this") {
        file << "@THIS" << endl;
    } else if (segment == "that") {
        file << "@THAT" << endl;
    } else if (segment == "pointer") {
        file << "@R" << 3 + index << endl;        
    } else if (segment == "temp") {
        file << "@R" << 5 + index << endl;
    }
    if (segment == "local" || segment == "argument" || segment == "this"
     || segment == "that") {
        file << "D=M" << endl;
        file << "@" << index << endl;
        file << "A=D+A" << endl;        
    }
    file << "D=M" << endl;
    file << "D=A" << endl;
    file << "@R13" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
}

// Methods For Program Flow and Function (chapter 8)
// labelコマンドを行うアセンブリコードを書く
void CodeWriter::writeLabel(string label) {
    file << "(" << label << ")" << endl;
}

// gotoコマンドを行うアセンブリコードを書く
void CodeWriter::writeGoto(string label) {
    file << "@" << label << endl;
    file << "0;JMP" << endl;
}

// if-gotoコマンドを行うアセンブリコードを書く
void CodeWriter::writeIf(string label) {
    cout << "writeIf : " << label << endl;
    file << "@SP" << endl;
    file << "AM=M-1" << endl;
    file << "D=M" << endl;
    file << "@" << label << endl;
    file << "D;JNE" << endl;
}

// callコマンドを行うアセンブリコードを書く
void CodeWriter::writeCall(string functionName, int numArgs) {

}

// returnコマンドを行うアセンブリコードを書く
void CodeWriter::writeReturn() {

}

// functionコマンドを行うアセンブリコードを書く
void CodeWriter::writeFunction(string functionName, int numLocals) {

}