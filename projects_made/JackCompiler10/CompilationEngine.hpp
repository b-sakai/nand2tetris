#include <string>
#include <fstream>
#include <memory>
#include "JackTokenizer.hpp"
using namespace std;

class CompilationEngine {
public:
    ofstream file;
    unique_ptr<JackTokenizer> tokenizer;

public:
    // コンストラクタ
    CompilationEngine(string ifilename, string ofilename);
    // ファイル全体をコンパイルする
    void compile();

protected:
    // tokenizerを進めて、次のトークンを取得する
    void advance();
    // xmlヘッダータグを書き込む
    void writeHeader(string tagName);
    // xmlフッタータグを書き込む
    void writeFooter(string tagName);
    // xml要素を書き込む
    void writeElement(string tagName, string value);

    // クラス
    void compileClass();
    void compileClassVarDec();
    void compileType();
    void compileSubroutine();
    void compileParameterList();
    void compileSubroutineBody();    
    void compileVarDec();
    void compileClassName();
    void compileSubroutineName();    
    void compileVarName();

    // 文
    void compileStatements();       
    void compileLet();
    void compileIf();    
    void compileWhile();
    void compileDo();
    void compileReturn();

    // 式
    void compileExpression();
    void compileTerm();
    void compileSubroutineCall();
    void compileExpressionList();
    void compileOp();
    void compileUnaryOp();    
    void compileKeywordConstant();

    // forDebug
    void logF();
};
