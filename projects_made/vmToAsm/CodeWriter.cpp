#include <cassert>
#include "CodeWriter.hpp"

CodeWriter::CodeWriter(string filename) {
    file.open(filename);
    writeInit();
}

// VMの初期化を行うアセンブリコードを書く。
void CodeWriter::writeInit() {
    file << "@256" << endl;
    file << "D=A" << endl;
    file << "@SP" << endl;
    file << "M=D" << endl;
    writeCall("Sys.init", 0);
}

void CodeWriter::setFileName(string filename) {
    curScope = filename.substr(filename.find_last_of("/\\") + 1);
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
        file << "@" << curScope << "ST" << index << endl;
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
        file << "@" << curScope << "ST" << index << endl;
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
    // 引数を渡す
    // 呼び出し側の状態をスタック上に格納する push
    // 呼び出された側で、ローカル変数のためのメモリ空間を用意する
    // 呼び出された側のサブルーチンへ実行を移す（jump）
    // 呼び出された側から呼び出し側へ値を返す
    // リターン時に、呼び出されたが話のサブルーチンによって使われたメモリ空間を再利用できるようにする
    // 呼び出し側の状態を復帰させる
    // サブルーチンの次の場所に実行を移す（jump)

    // 渡すアドレスはすでにスタックにプッシュされている

    // リターンアドレスをプッシュ
    string returnLabel = "RETURN_LABEL_" + to_string(returnLabelNum++);
    file << "@" << returnLabel << endl;
    file << "D=A" << endl;
    // push d register
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    // 復元用のLCLアドレスをプッシュ
    file << "@LCL" << endl;
    file << "D=M" << endl;
    // push d register
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    // 復元用のARGアドレスをプッシュ
    file << "@ARG" << endl;
    file << "D=M" << endl;
    // push d register
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;

    // 復元用のTHISアドレスをプッシュ
    file << "@THIS" << endl;
    file << "D=M" << endl;  
    // push d register
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;      

    // 復元用のTHATアドレスをプッシュ
    file << "@THAT" << endl;
    file << "D=M" << endl;  
    // push d register
    file << "@SP" << endl;
    file << "A=M" << endl;
    file << "M=D" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl;          

    // LCL, ARGアドレスを移動
    file << "@SP" << endl;
    file << "D=M" << endl;  
    file << "@LCL" << endl;
    file << "M=D" << endl;              
    file << "@" << 5 + numArgs << endl;              
    file << "D=D-A" << endl;      
    file << "@ARG" << endl;                  
    file << "M=D" << endl; // ARG = SP - numArgs - 5

    // サブルーチンへ移動
    file << "@" << functionName << endl;
    file << "0;JMP" << endl;
    file << "(" << returnLabel << ")" << endl;
}

// returnコマンドを行うアセンブリコードを書く
void CodeWriter::writeReturn() {
    // 呼び出し元のリターンアドレス = *(LCL - 5)
    // 呼び出し元のLCLアドレス = *(LCL - 4)
    // 呼び出し元のARGアドレス = *(LCL - 3)
    // 呼び出し元のTHISアドレス = *(LCL - 2)
    // 呼び出し元のTHATアドレス = *(LCL - 1)
    // 戻り値の格納されるアドレス = ARG-1
    file << 
"@LCL\n" // リターンアドレスの取得
"D=M\n"
"@R13\n" // R13 = FRAME = LCL
"M=D\n"
"@R13\n"
"D=M\n"
"@5\n"
"D=D-A\n"
"A=D\n"
"D=M\n" // D = *(FRAME-5) = return-address
"@R14\n"
"M=D\n" // R14 = return-address
"@SP\n"
"AM=M-1\n"
"D=M\n"
"@ARG\n"
"A=M\n"
"M=D\n" // ARGの位置に戻り値をセットする
"@ARG\n"
"D=M\n"
"@SP\n"
"M=D+1\n" // SP = ARG+1
"@R13\n"
"D=M\n"
"@1\n"
"D=D-A\n"
"A=D\n"
"D=M\n"
"@THAT\n"
"M=D\n" // THAT = *(FRAME-1)
"@R13\n"
"D=M\n"
"@2\n"
"D=D-A\n"
"A=D\n"
"D=M\n"
"@THIS\n"
"M=D\n" // THIS = *(FRAME-2)
"@R13\n"
"D=M\n"
"@3\n"
"D=D-A\n"
"A=D\n"
"D=M\n"
"@ARG\n"
"M=D\n" // ARG= *(FRAME-3)
"@R13\n"
"D=M\n"
"@4\n"
"D=D-A\n"
"A=D\n"
"D=M\n"
"@LCL\n"
"M=D\n" // LCL = *(FRAME-4)
"@R14\n"
"A=M\n"
"0;JMP" // GOTO return-address
<< endl;
}

// functionコマンドを行うアセンブリコードを書く
void CodeWriter::writeFunction(string functionName, int numLocals) {
    file << "(" << functionName << ")" << endl;
    for (int i=0; i<numLocals; i++) {
        file << "D=0" << endl;
        file << "@SP" << endl;
        file << "A=M" << endl;
        file << "M=D" << endl;
        file << "@SP" << endl;
        file << "M=M+1" << endl;
    }
}