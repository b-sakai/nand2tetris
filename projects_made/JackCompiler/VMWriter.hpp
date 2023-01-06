#include <string>
#include <fstream>
using namespace std;

enum Segment {
    SEG_CONST,
    SEG_ARG,
    SEG_LOCAL,
    SEG_STATIC,
    SEG_THIS,
    SEG_THAT,
    SEG_POINTER,
    SEG_TEMP
};

enum ArithmeticCommand {
    AC_ADD,
    AC_SUB,
    AC_NEG,
    AC_EQ,
    AC_GT,
    AC_LT,
    AC_AND,
    AC_OR,
    AC_NOT
};

class VMWriter {
public:
    ofstream file;

public:    
    // コンストラクタ
    VMWriter(string ofilename);
    // デストラクタ
    ~VMWriter();
    // pushコマンドを書く
    void writePush(Segment segment, int index);
    // popコマンドを書く
    void writePop(Segment segment, int index);
    // 算術コマンドを書く
    void writeArithmetic(ArithmeticCommand command);
    // labelコマンドを書く
    void writeLabel(string label);
    // gotoコマンドを書く
    void writeGoto(string label);
    // If-gotoコマンドを書く
    void writeIf(string label);
    // callコマンドを書く
    void writeCall(string name, int nArgs);
    // functionコマンドを書く
    void writeFunction(string name, int nLocals);
    // returnコマンドを書く
    void writeReturn();    
    // 出力ファイルを閉じる
    void close();
};