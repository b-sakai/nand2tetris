#include <string>
#include <fstream>
#include <memory>
#include "JackTokenizer.hpp"
#include "SymbolTable.hpp"
#include "VMWriter.hpp"
using namespace std;

class CompilationEngine {
public:
    ofstream file;
    unique_ptr<JackTokenizer> tokenizer;

    unique_ptr<SymbolTable> symbolTable;
    SymbolAttribute kind;
    string typeName;

    unique_ptr<VMWriter> vmWriter;
    string className;
    string subroutineName;
    string subroutineDeclare;
    int parameterNum = 0;

    int whileIndex = 0;
    int ifIndex = 0;
    // method && argのとき,thisがargument 0を使うので、argを１つずつずらすためのメンバ
    int argThisReserved = 0;


    
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
    // パラメータの数を返す    
    int compileParameterList();
    void compileSubroutineBody();    
    void compileVarDec();
    void compileClassName();
    string compileSubroutineName();    
    string compileVarName();

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
    // expressionの数を返す
    int compileExpressionList();
    void compileOp();
    void compileUnaryOp();    
    void compileKeywordConstant();

    void compileStringConstant(string sc);
    Segment symbolAttributeToSegment(SymbolAttribute attr);

    // forDebug
    void logF();
};
