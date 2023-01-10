#include <cassert>
#include <sstream>
#include <istream>
#include <unordered_set>
#include "CompilationEngine.hpp"
using namespace std;

// コンストラクタ
CompilationEngine::CompilationEngine(string ifilename) {
    tokenizer = make_unique<JackTokenizer>(ifilename);
    symbolTable = make_unique<SymbolTable>();

    // ifilenameからofilenameを取得する（.jack -> .vm)
    string ofilename = ifilename.substr(0, ifilename.size()-4) + "vm";
    cout << ofilename << endl;

    vmWriter = make_unique<VMWriter>(ofilename);
}

// tokenizerを進めて、次のトークンを取得する
void CompilationEngine::advance() {
    tokenizer->advance();
    TokenType type = tokenizer->tokenType;
    while (type == EMPTY && tokenizer->hasMoreTokens) {
        tokenizer->advance();
        type = tokenizer->tokenType;
    }
}

// ファイル全体をコンパイルする
void CompilationEngine::compile() {
    // 全てのファイルはクラス定義から始めるため、まずクラスをコンパイルする
    compileClass();
}

// クラスをコンパイルする
void CompilationEngine::compileClass() {
    // class
    advance();
    // class name
    compileClassName();
    advance(); // {

     // この次はclassVarDecかsubroutineDecのどちらか
    unordered_set<string> subroutineKeyword = 
        {"function", "constructor", "method" };
    unordered_set<string> classVarDecKeyword = 
        {"static", "field" };    
    advance();
    while (tokenizer->tokenType == KEYWORD) {
        string keyword = tokenizer->keyword;
        if (subroutineKeyword.count(keyword)) {
            if (keyword == "method") {
                argThisReserved = 1;
            } else {
                argThisReserved = 0;
            }
            compileSubroutine();
        }
        if (classVarDecKeyword.count(keyword)) {
            compileClassVarDec();
        }
        advance();
        whileIndex = 0;
        ifIndex = 0;
    }
    // }
}

// スタティック宣言 | フィールド宣言をコンパイルする
void CompilationEngine::compileClassVarDec() {
    // ("static" | "field")
    if (tokenizer->keyword == "static") {
        kind = S_STATIC;
    } else if (tokenizer->keyword == "field") {
        kind = S_FIELD;
    }
    // type
    advance();
    compileType();
    // varName
    compileVarName();
    // (',' varName)*
    advance();
    while (tokenizer->symbol == ",") {
        compileVarName();
        advance();
    }
    // ";"
}

void CompilationEngine::compileType() {
    if (tokenizer->tokenType == KEYWORD) { // "int" | "char" | "boolean" | "void"
        typeName = tokenizer->keyword;
    } else if (tokenizer->tokenType == IDENTIFIER) { // className
        typeName = tokenizer->identifier;
    }
}

// メソッド、ファンクション、コンストラクタをコンパイルする
void CompilationEngine::compileSubroutine() {
    // "function" | "constructor" | "method"
    subroutineDeclare = tokenizer->keyword;
    // ("void" | type)
    advance();
    compileType();
    // subroutineName()
    subroutineName = compileSubroutineName();
    // "("    
    advance();
    // parameterList
    kind = S_ARG;
    int parameterNum = compileParameterList();
    // ")"

    // compile subroutineBody
    compileSubroutineBody();
    symbolTable->clearSubroutineScopeTable();
}

// パラメータのリストをコンパイルする
// パラメータは空の可能性もある
int CompilationEngine::compileParameterList() {
    int parameterNum = 0;
    advance();
    while (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) { // ")"を見つけるまで
        if (tokenizer->tokenType == SYMBOL && tokenizer->symbol == ",") {
            advance();
        }
        // type
        compileType();
        // varName
        compileVarName();
        parameterNum++;
        advance();
    }
    return parameterNum;
}

// subroutinBodyをコンパイルする
void CompilationEngine::compileSubroutineBody() {
    // "{"
    advance();
    // varDec*
    advance();
    while (tokenizer->keyword == "var") {
        compileVarDec();
    }
    string funcName = className + "." + subroutineName;
    vmWriter->writeFunction(funcName, symbolTable->varCount(S_VAR));
    subroutineName = "";

    if (subroutineDeclare == "method") {
        vmWriter->writePush(SEG_ARG, 0);
        vmWriter->writePop(SEG_POINTER, 0);
    } else if (subroutineDeclare == "constructor") {
        vmWriter->writePush(SEG_CONST, symbolTable->varCount(S_FIELD));
        vmWriter->writeCall("Memory.alloc", 1);
        vmWriter->writePop(SEG_POINTER, 0);
    }

    // statements
    compileStatements();
    // "}"    
}    

// var宣言をコンパイルする
void CompilationEngine::compileVarDec() {
    // "var"
    kind = S_VAR;
    // type
    advance();
    compileType();
    // varName
    compileVarName();
    // (',' varName)*
    advance();
    while (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ";")) {
        // ","
        // varName
        compileVarName();
        advance();
    }
    // ";"
    advance();
}

void CompilationEngine::compileClassName() {
    advance();
    className = tokenizer->identifier;
}

string CompilationEngine::compileSubroutineName() {
    advance();
    return tokenizer->identifier;
}

string CompilationEngine::compileVarName() {
    advance();
    if (kind == S_STATIC || kind == S_FIELD || kind == S_ARG || kind == S_VAR) {
        symbolTable->define(tokenizer->identifier, typeName, kind);
    }
    string varName = tokenizer->identifier;
    return varName;
}

// 一連の文をコンパイルする
// 波括弧"{}"は含まない
void CompilationEngine::compileStatements() {
    // do | let | while | return | ifのどれか
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
        // else節を先読みしているためadvance()しない
    }
    }
}

// let文をコンパイルする
void CompilationEngine::compileLet() {
    // "let"
    kind = S_LET;
    // varName
    string letVal = compileVarName();

    advance();
    if ((tokenizer->tokenType == SYMBOL && tokenizer->symbol == "[")) {
        // "["
        advance();    
        compileExpression();
        // "]"
        advance();
        // push base address
        SymbolAttribute letKind = symbolTable->kindOf(letVal);
        Segment letSeg = symbolAttributeToSegment(letKind);
        int letIndex = symbolTable->indexOf(letVal);
        if (letSeg == SEG_ARG) {
            letIndex += argThisReserved;
        }
        vmWriter->writePush(letSeg, letIndex);
        // SP = letVal+i
        vmWriter->writeArithmetic(AC_ADD);

        // "="
        // expression
        advance();
        compileExpression();
        // = の先のexpressionの評価値をtempに格納
        vmWriter->writePop(SEG_TEMP, 0);
        // thatのベースアドレスをSP(letVal + i)に設定
        vmWriter->writePop(SEG_POINTER, 1);
        // 評価値を(SPに)push
        vmWriter->writePush(SEG_TEMP, 0);
        // that(letVal + i)に評価値を格納
        vmWriter->writePop(SEG_THAT, 0);
    } else {
        // "="
        // expression
        advance();
        compileExpression();
        SymbolAttribute letKind = symbolTable->kindOf(letVal);
        Segment letSeg = symbolAttributeToSegment(letKind);
        int letIndex = symbolTable->indexOf(letVal);
        if (letSeg == SEG_ARG) {
            letIndex += argThisReserved;
        }        
        vmWriter->writePop(letSeg, letIndex);
    }
}

// if文をコンパイルする
// else文を伴う可能性がある
void CompilationEngine::compileIf() {
    // if
    // "("
    advance();
    // expression
    advance();
    compileExpression();
    // ")"
    int curIfIndex = ifIndex++;
    vmWriter->writeIf("IF_TRUE" + to_string(curIfIndex));
    vmWriter->writeGoto("IF_FALSE" + to_string(curIfIndex));
    vmWriter->writeLabel("IF_TRUE" + to_string(curIfIndex));

    // "{"
    advance();
    // statements
    advance();
    compileStatements();
    // "}"

    advance();
    if (tokenizer->tokenType == KEYWORD && tokenizer->keyword == "else") {
        vmWriter->writeGoto("IF_END" + to_string(curIfIndex));
        vmWriter->writeLabel("IF_FALSE" + to_string(curIfIndex));
        // else
        // "{"
        advance();
        // statements
        advance();
        compileStatements();    
        // "}"
        advance();
        vmWriter->writeLabel("IF_END" + to_string(curIfIndex));        
    } else {
        vmWriter->writeLabel("IF_FALSE" + to_string(curIfIndex));
    }
}

// while文をコンパイルする
void CompilationEngine::compileWhile() {
    // while
    int curWhileIndex = whileIndex++;
    vmWriter->writeLabel("WHILE_EXP" + to_string(curWhileIndex));
    // "("
    advance();
    // expression
    advance();
    compileExpression();
    // ")"
    vmWriter->writeArithmetic(AC_NOT);
    vmWriter->writeIf("WHILE_END" + to_string(curWhileIndex));

    // "{"
    advance();
    // statements
    advance();    
    compileStatements();
    // "}"
    vmWriter->writeGoto("WHILE_EXP" + to_string(curWhileIndex));    
    vmWriter->writeLabel("WHILE_END" + to_string(curWhileIndex));    
}

// do文をコンパイルする
void CompilationEngine::compileDo() {
    // "do"
    // subroutineCall
    advance();
    parameterNum = 0;
    compileSubroutineCall();
    // ";"
    advance();
    vmWriter->writePop(SEG_TEMP, 0);
}

// return文をコンパイルする
void CompilationEngine::compileReturn() {
    // "return"

    advance();
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ";")) {
        compileExpression();
    } else {
        vmWriter->writePush(SEG_CONST, 0);
    }
    // ";""
    vmWriter->writeReturn();
}

// 式をコンパイルする
void CompilationEngine::compileExpression() {
    // term
    compileTerm();
    unordered_set<string> op = {"+", "-", "*", "/", "&", "|", "<", ">", "="};
    while (op.count(tokenizer->symbol)) {
        // op
        string op_token = tokenizer->symbol;
        // term
        advance();
        compileTerm();
        if (op_token == "+") {
            vmWriter->writeArithmetic(AC_ADD);
        } else if (op_token == "-") {
            vmWriter->writeArithmetic(AC_SUB);
        } else if (op_token == "*") {
            vmWriter->writeCall("Math.multiply", 2);
        } else if (op_token == "/") {
            vmWriter->writeCall("Math.divide", 2);
        } else if (op_token == "&") {
            vmWriter->writeArithmetic(AC_AND);
        } else if (op_token == "|") {
            vmWriter->writeArithmetic(AC_OR);
        } else if (op_token == "<") {
            vmWriter->writeArithmetic(AC_LT);
        } else if (op_token == ">") {
            vmWriter->writeArithmetic(AC_GT);            
        } else if (op_token == "=") {            
            vmWriter->writeArithmetic(AC_EQ);                        
        } else {
            assert(false);
        }
    }
}

// termをコンパイルする
void CompilationEngine::compileTerm() {
    unordered_set<string> unaryOp = {"-", "~"};    
    TokenType tokenType = tokenizer->tokenType;
    if (tokenType == INT_CONST) {
        // integerConstant
        vmWriter->writePush(SEG_CONST, tokenizer->intVal);
        advance();
    } else if (tokenType == STRING_CONST) {
        // stringConstant
        string stringConstant = tokenizer->stringVal;
        compileStringConstant(stringConstant);
        advance();
    } else if (tokenType == KEYWORD) {
        // keywordConstant
        if (tokenizer->keyword == "true") {
            vmWriter->writePush(SEG_CONST, 0);
            vmWriter->writeArithmetic(AC_NOT);
        } else if (tokenizer->keyword == "false") {
            vmWriter->writePush(SEG_CONST, 0);            
        } else if (tokenizer->keyword == "null") {
            vmWriter->writePush(SEG_CONST, 0);            
        } else if (tokenizer->keyword == "this") {
            vmWriter->writePush(SEG_POINTER, 0);            
        } else {
            assert(false);
        }
        advance();
    } else if (tokenType == SYMBOL) {
        if (unaryOp.count(tokenizer->symbol)) {
            // unaryOp
             compileUnaryOp();
        } else if (tokenizer->symbol == "(") {
            // "(" expression ")"
            // "("
            // expression
            advance();
            compileExpression();
            // ")"
            advance();
        }
    } else if (tokenType == IDENTIFIER) {
        // "[" -> expression
        // "." -> subroutineCall
        string instanceName = tokenizer->identifier;
        advance();

        if (tokenizer->symbol == "[") { // "[" -> expression
            // 下のif文で処理を行う
        } else if (symbolTable->kindOf(instanceName) != S_NONE) {
            SymbolAttribute termKind = symbolTable->kindOf(instanceName);
            Segment termSeg = symbolAttributeToSegment(termKind);
            int termIndex = symbolTable->indexOf(instanceName);
            if (termSeg == SEG_ARG) {
                termIndex += argThisReserved;
            }
            vmWriter->writePush(termSeg, termIndex);
        } else {
            subroutineName = tokenizer->identifier;
        }


        if (tokenizer->tokenType == SYMBOL) {
            if (tokenizer->symbol == "[") {
                // "["
                // expression
                advance();
                compileExpression();
                SymbolAttribute termKind = symbolTable->kindOf(instanceName);
                Segment termSeg = symbolAttributeToSegment(termKind);
                int termIndex = symbolTable->indexOf(instanceName);
                if (termSeg == SEG_ARG) {
                    termIndex += argThisReserved;
                }
                vmWriter->writePush(termSeg, termIndex);                
                // "]"
                vmWriter->writeArithmetic(AC_ADD);
                vmWriter->writePop(SEG_POINTER, 1);
                vmWriter->writePush(SEG_THAT, 0);
                advance();
            } else if (tokenizer->symbol == ".") {
                parameterNum = 0;
                if (symbolTable->kindOf(instanceName) != S_NONE) {                
                    subroutineName = symbolTable->typeOf(instanceName);
                    parameterNum++;
                }
                // "."
                subroutineName += ".";
                // compileSubroutineCall
                advance();
                compileSubroutineCall();
                advance();
            }
        }
    }
}

void CompilationEngine::compileSubroutineCall() {
    // p1 : subroutineName(expressionList)
    // p2 : className.subroutineName(expressionList)
    // p3 : varName.subroutineName(expressionList)

    // subroutineName
    subroutineName += tokenizer->identifier;

    advance();
    if (tokenizer->symbol == "(") { // p1 : subroutineName(expressionList)
        if (subroutineName.find('.') == string::npos) {
            // ここに来るのは同じクラスのメソッドを呼んだとき
            subroutineName = className + "." + subroutineName; // クラス名で修飾
            vmWriter->writePush(SEG_POINTER, 0); // 自身のアドレスをプッシュする
            parameterNum++; // 引数の数をインクリメントする
        }
        // "("
        // expressionList
        advance();
        parameterNum += compileExpressionList();
        // ")"
    } else if (tokenizer->symbol == ".") { // p2,p3 : name.subroutineName(expressonList)
        if (symbolTable->typeOf(subroutineName) != "none") {
            // インスタンス変数のときは
            // 1. subroutineNameをクラス名に変更する
            // 2. 引数として渡すので、アドレスをシンボルテーブルから検索してプッシュする
            // 3. 引数が増えるのでparameterNumをインクリメントする
            subroutineName = symbolTable->typeOf(subroutineName);
            SymbolAttribute termKind = symbolTable->kindOf(tokenizer->identifier);
            Segment termSeg = symbolAttributeToSegment(termKind);
            int termIndex = symbolTable->indexOf(tokenizer->identifier);
            if (termSeg == SEG_ARG) {
                termIndex += argThisReserved;
            }
            vmWriter->writePush(termSeg, termIndex);
            parameterNum++;
        }
        // "."
        // "subroutineName"
        string subName = compileSubroutineName();
        subroutineName += "." + subName;
        // "("
        advance();
        // expressionList
        advance();
        parameterNum += compileExpressionList();        
        // ")"
    }
    vmWriter->writeCall(subroutineName, parameterNum);
    subroutineName = "";
}

// コンマで分離された式のリストをコンパイルする
int CompilationEngine::compileExpressionList() {
    int expressionNum = 0;
    // (expression (, expression)* )?
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) {
        // expression
        compileExpression();
        expressionNum++;
        // (",",  expression)*
        while (tokenizer->tokenType == SYMBOL && tokenizer->symbol == ",") {
            // ","
            // expression
            advance();
            compileExpression();
            expressionNum++;
        }
    }
    return expressionNum;
}

void CompilationEngine::compileUnaryOp() {
    // unaryOp
    string unaryOpToken = tokenizer->symbol;
    // term
    advance();
    compileTerm();
    if (unaryOpToken == "-") {
        vmWriter->writeArithmetic(AC_NEG);
    } else if (unaryOpToken == "~") {
        vmWriter->writeArithmetic(AC_NOT);
    } else {
        assert(false);
    }
}

void CompilationEngine::compileStringConstant(string sc) {
    // サイズをプッシュする
    vmWriter->writePush(SEG_CONST, sc.size());
    // String.newを呼び出す
    vmWriter->writeCall("String.new", 1);
    // 文字を１つずつプッシュして、appendCharを呼び出す
    for (auto c : sc) {
        vmWriter->writePush(SEG_CONST, c);
        vmWriter->writeCall("String.appendChar", 2);
    }
}

Segment CompilationEngine::symbolAttributeToSegment(SymbolAttribute attr) {
    switch (attr) {
        case S_STATIC:
            return SEG_STATIC;
        case S_ARG:
            return SEG_ARG;
        case S_VAR:
            return SEG_LOCAL;
        case S_FIELD:
            return SEG_THIS;             
        default:
            return SEG_TEMP;
    }
}
