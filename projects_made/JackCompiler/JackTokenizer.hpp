#include <string>
#include <fstream>
#include <iostream>
#include "TokenType.hpp"
#include "Keyword.hpp"
using namespace std;

class JackTokenizer {
public:
    // 読み込むファイル
    ifstream file;
    bool hasMoreTokens = true;
    TokenType tokenType;
    // 現トークンのキーワード（tokenType == KEYWORDのときのみ）
    string keyword;
    // 現トークンの文字（tokenType == SYMBOLのときのみ）
    string symbol;
    // 現トークンの識別子（tokenType == IDENTIFIERのときのみ）
    string identifier;
    // 現トークンの整数の値（tokenType == INT_CONSTのときのみ）
    int intVal;
    // 現トークンの文字列（tokenType == STRING_CONSTのときのみ）
    string stringVal;

    // 中間トークン
    string tokens = "";

public:
    // コンストラクタ
    JackTokenizer(string filename);
    // 次のトークンを取得し、それを現在のトークンとする
    void advance();
    // スペース区切りでファイルを読み、tokensに格納する
    // コメントはここで取り除く
    void advanceSpace();    
};