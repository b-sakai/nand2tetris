#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CodeWriter {
public:
    ofstream file;
    string curScope = "";
    int countForSymbol = 0;
    int returnLabelNum = 0;

public:
    // コンストラクタ
    // @param filename　出力ファイルパス
    CodeWriter(string filename);
    // 新しいファイルパスを指定する
    // @param filename 新しいファイルパス
    void setFileName(string filename);
    // 出力ファイルを閉じる
    void closeFile();

    // VMの初期化を行うアセンブリコードを書く。
    void writeInit();

    // Methods For Stack and Arithmetic (chapter 7)
    // 与えられた算術コマンドをアセンブリコードに変換し、それを書き込む
    void writeArithmetic(string command);
    // C_PUSH C_POPコマンドをアセンブリコードに変換し、それを書き込む
    void writePush(string command, string segment, int index);
    // C_POPコマンドをアセンブリコードに変換し、それを書き込む
    void writePop(string command, string segment, int index);

    // Methods For Program Flow and Function (chapter 8)
    // labelコマンドを行うアセンブリコードを書く
    void writeLabel(string label);
    // gotoコマンドを行うアセンブリコードを書く
    void writeGoto(string label);
    // if-gotoコマンドを行うアセンブリコードを書く
    void writeIf(string label);
    // callコマンドを行うアセンブリコードを書く
    void writeCall(string functionName, int numArgs);
    // returnコマンドを行うアセンブリコードを書く
    void writeReturn();
    // functionコマンドを行うアセンブリコードを書く
    void writeFunction(string functionName, int numLocals);
};
