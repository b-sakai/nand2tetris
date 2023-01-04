#include <cassert>
#include <unordered_set>
#include "CompilationEngine.hpp"
using namespace std;

// コンストラクタ
CompilationEngine::CompilationEngine(string ifilename, string ofilename) {
    file.open(ofilename);
    tokenizer = make_unique<JackTokenizer>(ifilename);    
}

// tokenizerを進めて、次のトークンを取得する
void CompilationEngine::advance() {
    tokenizer->advance();
    TokenType type = tokenizer->tokenType;
    while (type == EMPTY && tokenizer->hasMoreTokens) {
        tokenizer->advance();
        type == tokenizer->tokenType;
    }
}

// xmlヘッダータグを書き込む
void CompilationEngine::writeHeader(string tagName) {
    file << "<" << tagName << ">" << endl;
    cout << "<" << tagName << ">" << endl;    
}

// xmlフッタータグを書き込む
void CompilationEngine::writeFooter(string tagName) {
    file << "</" << tagName << ">" << endl;
    cout << "</" << tagName << ">" << endl;
}

// xml要素を書き込む
void CompilationEngine::writeElement(string tagName, string value) {
    file << "<" << tagName << "> ";
    file << value;
    file << " </" << tagName << ">" << endl;    

    cout << "<" << tagName << "> ";
    cout << value;
    cout << " </" << tagName << ">" << endl;        
}

// ファイル全体をコンパイルする
void CompilationEngine::compile() {
    // 全てのファイルはクラス定義から始めるため、まずクラスをコンパイルする
    compileClass();
}

// クラスをコンパイルする
void CompilationEngine::compileClass() {
    writeHeader("class");

    // class
    advance();
    writeElement("keyword", tokenizer->keyword);
    // class name
    compileClassName();
    advance(); // {
    writeElement("symbol", tokenizer->symbol); // write "{"

     // この次はclassVarDecかsubroutineDecのどちらか
    unordered_set<string> subroutineKeyword = 
        {"function", "constructor", "method" };
    unordered_set<string> classVarDecKeyword = 
        {"static", "field" };    
    advance();
    while (tokenizer->tokenType == KEYWORD) {
        string keyword = tokenizer->keyword;
        if (subroutineKeyword.count(keyword)) {
            compileSubroutine();
        }
        if (classVarDecKeyword.count(keyword)) {
            compileClassVarDec();
        }
        advance();
    }
    // "}"
    writeElement("symbol", tokenizer->symbol);
    writeFooter("class");
}

// スタティック宣言 | フィールド宣言をコンパイルする
void CompilationEngine::compileClassVarDec() {
    writeHeader("classVarDec");

    // ("static" | "field")
    writeElement("keyword", tokenizer->keyword);
    // type
    compileType();
    // varName
    compileVarName();
    // (',' varName)*
    advance();
    while (tokenizer->symbol == ",") {
        writeElement("symbol", tokenizer->symbol);
        compileVarName();
        advance();
    }
    // ";"
    writeElement("symbol", tokenizer->symbol); // write ";""
    writeFooter("classVarDec");
}

void CompilationEngine::compileType() {
    advance();
    if (tokenizer->tokenType == KEYWORD) { // "int" | "char" | "boolean" | "void"
        writeElement("keyword", tokenizer->keyword);
    } else if (tokenizer->tokenType == IDENTIFIER) { // className
        writeElement("identifier", tokenizer->identifier);
    }
}

// メソッド、ファンクション、コンストラクタをコンパイルする
void CompilationEngine::compileSubroutine() {
    writeHeader("subroutineDec");
    // "function" | "constructor" | "method"
    writeElement("keyword", tokenizer->keyword);
    // ("void" | type)
    compileType();
    // subroutineName()
    compileSubroutineName();
    // "("    
    advance();
    writeElement("symbol", tokenizer->symbol); // write "("
    // parameterList
    compileParameterList();
    // ")"
    writeElement("symbol", tokenizer->symbol);  // write ")"

    // compile subroutineBody
    compileSubroutineBody();

    writeFooter("subroutineDec");
}

// パラメータのリストをコンパイルする
// パラメータは空の可能性もある
void CompilationEngine::compileParameterList() {
    writeHeader("parameterList");
    advance();
    while (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) { // ")"を見つけるまで
        if (tokenizer->tokenType == KEYWORD) {
            writeElement("keyword", tokenizer->keyword);
        } else if (tokenizer->tokenType == IDENTIFIER) {
            writeElement("identifier", tokenizer->identifier);
        } else if (tokenizer->tokenType == SYMBOL) {
            writeElement("symbol", tokenizer->symbol);
        }
        advance();
    }
    writeFooter("parameterList");
}

// subroutinBodyをコンパイルする
void CompilationEngine::compileSubroutineBody() {
    writeHeader("subroutineBody");
    // "{"
    advance();
    writeElement("symbol", tokenizer->symbol); // write "{"
    // varDec*
    advance();
    while (tokenizer->keyword == "var") {
        compileVarDec();
    }
    // statements
    compileStatements();
    // "}"    
    writeElement("symbol", tokenizer->symbol); // write "}"
    writeFooter("subroutineBody");
}    

// var宣言をコンパイルする
void CompilationEngine::compileVarDec() {
    writeHeader("varDec");
    // "var"
    writeElement("keyword", tokenizer->keyword);
    // type
    compileType();
    // varName
    compileVarName();
    // (',' varName)*
    tokenizer->log();
    advance();
    tokenizer->log();    
    while (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ";")) {
        // ","
        writeElement("symbol", ",");
        // varName
        compileVarName();
        advance();
    }
    // ";"
    writeElement("symbol", tokenizer->symbol);
    advance();
    writeFooter("varDec");   
}

void CompilationEngine::compileClassName() {
    advance();
    writeElement("identifier", tokenizer->identifier);
}

void CompilationEngine::compileSubroutineName() {
    advance();
    writeElement("identifier", tokenizer->identifier);    
}

void CompilationEngine::compileVarName() {
    advance();
    writeElement("identifier", tokenizer->identifier);    
}

// 一連の文をコンパイルする
// 波括弧"{}"は含まない
void CompilationEngine::compileStatements() {
    writeHeader("statements");
    // do | let | while | return | ifのどれか
    tokenizer->log();
    assert(tokenizer->tokenType == KEYWORD);
    while(tokenizer->tokenType == KEYWORD) {
    if (tokenizer->keyword == "do") {
        compileDo();
        advance();        
    } else if (tokenizer->keyword == "let") {
        compileLet();
        advance();
    } else if (tokenizer->keyword == "while") {
        compileWhile();
        advance();
    } else if (tokenizer->keyword == "return") {                
        compileReturn();
        advance();
    } else if (tokenizer->keyword == "if") {
        compileIf();
        // else節を先読み失敗して失敗している場合はadvance()しない
        if (tokenizer->tokenType == SYMBOL) {
            advance();
        }
    }
    }
    writeFooter("statements");
}

// let文をコンパイルする
void CompilationEngine::compileLet() {
    writeHeader("letSatement");
    // "let"
    writeElement("keyword", tokenizer->keyword); // write "let"
    // varName
    compileVarName();

    advance();
    if ((tokenizer->tokenType == SYMBOL && tokenizer->symbol == "[")) {
        // "["
        writeElement("symbol", tokenizer->symbol);
        advance();    
        compileExpression();
        // "]"
        writeElement("symbol", tokenizer->symbol);
        advance();
    }
    // "="
    writeElement("symbol", tokenizer->symbol); // write "="
    // expression
    advance();
    compileExpression();
    // tokenizer->log();
    writeElement("symbol", tokenizer->symbol); // write ";"

    writeFooter("letSatement");
}

// if文をコンパイルする
// else文を伴う可能性がある
void CompilationEngine::compileIf() {
    writeHeader("ifSatement");
    // if
    writeElement("keyword", tokenizer->keyword);    
    // "("
    advance();
    writeElement("symbol", tokenizer->symbol);
    // expression
    advance();
    compileExpression();
    // ")"
    writeElement("symbol", tokenizer->symbol);

    // "{"
    advance();
    writeElement("symbol", tokenizer->symbol);    
    // statements
    advance();
    compileStatements();
    // "}"
    writeElement("symbol", tokenizer->symbol);

    advance();
    if (tokenizer->tokenType == KEYWORD && tokenizer->keyword == "else") {
        // else
        writeElement("keyword", tokenizer->keyword);
        // "{"
        advance();
        writeElement("symbol", tokenizer->symbol);
        // statements
        advance();
        compileStatements();    
        // "}"
        writeElement("symbol", tokenizer->symbol);        
    }

    writeFooter("ifSatement");
}

// while文をコンパイルする
void CompilationEngine::compileWhile() {
    writeHeader("whileStatement");
    // while
    writeElement("keyword", tokenizer->keyword);
    // "("
    advance();
    writeElement("symbol", tokenizer->symbol);
    // expression
    advance();
    compileExpression();
    // ")"
    writeElement("symbol", tokenizer->symbol);

    // "{"
    advance();
    writeElement("symbol", tokenizer->symbol);
    // statements
    advance();    
    compileStatements();
    // "}"
     writeElement("symbol", tokenizer->symbol);    

    writeFooter("whileStatement");
}

// do文をコンパイルする
void CompilationEngine::compileDo() {
    writeHeader("doSatement");
    // "do"
    writeElement("keyword", tokenizer->keyword);
    // subroutineCall
    advance();
    compileSubroutineCall();
    // ";"
    advance();
    writeElement("symbol", tokenizer->symbol);
    writeFooter("doSatement");
}

// return文をコンパイルする
void CompilationEngine::compileReturn() {
    writeHeader("returnSatement");
    // "return"
    writeElement("keyword", tokenizer->keyword);

    advance();
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ";")) {
        compileExpression();
    }
    // ";""
    writeElement("symbol", tokenizer->symbol);
    writeFooter("returnSatement");
}

// 式をコンパイルする
void CompilationEngine::compileExpression() {
    writeHeader("expression");
    // term
    compileTerm();
    unordered_set<string> op = {"+", "-", "*", "/", "&", "|", "<", ">", "="};
    advance();
    while (op.count(tokenizer->symbol)) {
        // op
        writeElement("symbol", tokenizer->symbol);
        // term
        advance();
        compileTerm();
        advance();
    }
    writeFooter("expression");    
}

// termをコンパイルする
void CompilationEngine::compileTerm() {
    writeHeader("term");
    TokenType tokenType = tokenizer->tokenType;
    if (tokenType == INT_CONST) {
        // integerConstant
        writeElement("integerConstant", to_string(tokenizer->intVal));
    } else if (tokenType == STRING_CONST) {
        // stringConstant
        writeElement("stringConstant", tokenizer->stringVal);
    } else if (tokenType == KEYWORD) {
        // keywordConstant
        writeElement("keyword", tokenizer->keyword);
    } else if (tokenType == SYMBOL) {
        // unaryOp
        compileUnaryOp();
    } else if (tokenType == IDENTIFIER) {
        // "[" -> expression
        // "(" -> expression
        // "." -> subroutineCall
        writeElement("identifier", tokenizer->identifier);
    }
    writeFooter("term");
}

void CompilationEngine::compileSubroutineCall() {
    // p1 : subroutineName(expressionList)
    // p2 : className.subroutineName(expressionList)
    // p3 : varName.subroutineName(expressionList)

    // subroutineName
    writeElement("identifier", tokenizer->identifier);

    advance();
    if (tokenizer->symbol == "(") { // p1 : subroutineName(expressionList)
        // "("
        writeElement("symbol", tokenizer->symbol);
        // expressionList
        advance();
        compileExpressionList();
        // ")"
        writeElement("symbol", tokenizer->symbol);        
    } else if (tokenizer->symbol == ".") { // p2,p3 : name.subroutineName(expressonList)
        // "."
        writeElement("symbol", tokenizer->symbol);        
        // "subroutineName"
        compileSubroutineName();
        // "("
        advance();
        writeElement("symbol", tokenizer->symbol);
        // expressionList
        advance();
        compileExpressionList();        
        // ")"
        writeElement("symbol", tokenizer->symbol);                   
    }
}

// コンマで分離された式のリストをコンパイルする
void CompilationEngine::compileExpressionList() {
    writeHeader("expressionList");
    // (expression (, expression)* )?
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) {
        // expression
        compileExpression();
        // (",",  expression)*
        while (tokenizer->tokenType == SYMBOL && tokenizer->symbol == ",") {
            // ","
            writeElement("symbol", tokenizer->symbol);
            // expression
            advance();
            compileExpression();
        }
    }
    writeFooter("expressionList");
}

void CompilationEngine::compileUnaryOp() {
    // unaryOp
    writeElement("symbol", tokenizer->symbol);
    // term
    advance();
    compileTerm();
}