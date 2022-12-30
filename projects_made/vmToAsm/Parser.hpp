#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "CommandType.hpp"

using namespace std;

class Parser {
public:
    // 読み込むファイル
    ifstream file;
    // 入力の居て、さらにコマンドが存在するか
    bool hasMoreCommands = true;
    // 現VMコマンドの種類
    // 算術コマンドはすべてC_ARITHMETICが返される
    CommandType commandType = C_NONE;
    // 現VMコマンド（文字列）
    string command;
    // 現VMコマンドの最初の引数
    // C_ARITHMETICの場合、コマンド自体(add, subなど)が返される。
    // 現コマンドがC_RETURNの場合、本ルーチンは呼ばないようにする。
    string arg1;
    // 現コマンドの２番目の引数
    // 現コマンドがC_PUSH, C_POP, C_FUNCTION, C_CALLの場合のみ本ルーチンを呼ぶようにする
    int arg2;    

public:
    // コンストラクタ
    // @param filename 入力ファイルパス
    Parser(string filename);
    // 次のコマンドを読み、現在のコマンドとする
    void advance();

protected:
    CommandType judgeCommandType(string aCommand) const;
};