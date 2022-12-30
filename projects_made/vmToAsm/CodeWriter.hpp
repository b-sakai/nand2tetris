#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CodeWriter {
public:
    ofstream file;
    int countForSymbol = 0;

public:
    // コンストラクタ
    // @param filename　出力ファイルパス
    CodeWriter(string filename);
    // 新しいファイルパスを指定する
    // @param filename 新しいファイルパス
    void setFileName(string filename);
    // 出力ファイルを閉じる
    void closeFile();
    // 与えられた算術コマンドをアセンブリコードに変換し、それを書き込む
    void writeArithmetic(string command);
    // C_PUSH C_POPコマンドをアセンブリコードに変換し、それを書き込む
    void writePushPop(string command, string segment, int index);
};
