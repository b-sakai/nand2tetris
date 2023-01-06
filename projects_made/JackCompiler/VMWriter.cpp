#include <unordered_map>
#include "VMWriter.hpp"

unordered_map<Segment, string> segString = {
    {SEG_CONST, "constant"},
    {SEG_ARG, "argument"},
    {SEG_LOCAL, "local"},
    {SEG_STATIC, "static"},
    {SEG_THIS, "this"},
    {SEG_THAT, "that"},
    {SEG_POINTER, "pointer"},
    {SEG_TEMP, "temp"}
};

unordered_map<ArithmeticCommand, string> acString = {
    {AC_ADD, "add"},
    {AC_SUB, "sub"},
    {AC_NEG, "neg"},
    {AC_EQ, "eq"},
    {AC_GT, "gt"},
    {AC_LT, "lt"},
    {AC_AND, "and"},
    {AC_OR, "or"},
    {AC_NOT, "not"}
};

// コンストラクタ
VMWriter::VMWriter(string ofilename) {
    file.open(ofilename);
}

// デストラクタ
VMWriter::~VMWriter() {
    close();
}

// pushコマンドを書く
void VMWriter::writePush(Segment segment, int index) {
    file << "push " << segString.at(segment) << " " << index << endl;
}

// popコマンドを書く
void VMWriter::writePop(Segment segment, int index) {
    file << "pop " << segString.at(segment) << " " << index << endl;    
}

// 算術コマンドを書く
void VMWriter::writeArithmetic(ArithmeticCommand command) {
    file << acString.at(command) << endl;
}

// labelコマンドを書く
void VMWriter::writeLabel(string label) {
    file << "label " << label << endl;
}   

// gotoコマンドを書く
void VMWriter::writeGoto(string label) {
    file << "goto " << label << endl;
}

// If-gotoコマンドを書く
void VMWriter::writeIf(string label) {
    file << "if-goto " << label << endl;
}

// callコマンドを書く
void VMWriter::writeCall(string name, int nArgs) {
    file << "call " << name << " " << nArgs << endl;
}

// functionコマンドを書く
void VMWriter::writeFunction(string name, int nLocals) {
    file << "function " << name << " " << nLocals << endl;
}

// returnコマンドを書く
void VMWriter::writeReturn() {
    file << "return" << endl;
}

// 出力ファイルを閉じる
void VMWriter::close() {
    file.close();
}