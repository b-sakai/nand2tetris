#include <cassert>
#include <unordered_set>
#include "CompilationEngine.hpp"
using namespace std;

// コンストラクタ
CompilationEngine::CompilationEngine(string ifilename, string ofilename) {
    file.open(ofilename);
    tokenizer = make_unique<JackTokenizer>(ifilename);    
    symbolTable = make_unique<SymbolTable>();
    vmWriter = make_unique<VMWriter>("./output.vm");
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

// xmlヘッダータグを書き込む
void CompilationEngine::writeHeader(string tagName) {
    file << "<" << tagName << ">" << endl;
    //cout << "<" << tagName << ">" << endl;    
}

// xmlフッタータグを書き込む
void CompilationEngine::writeFooter(string tagName) {
    file << "</" << tagName << ">" << endl;
    //cout << "</" << tagName << ">" << endl;
}

// xml要素を書き込む
void CompilationEngine::writeElement(string tagName, string value) {
    file << "<" << tagName << "> ";
    file << value;
    file << " </" << tagName << ">" << endl;    

    //cout << "<" << tagName << "> ";
    //cout << value;
    //cout << " </" << tagName << ">" << endl;        
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
    if (tokenizer->keyword == "static") {
        kind = S_STATIC;
    } else if (tokenizer->keyword == "field") {
        kind = S_FIELD;
    }
    writeElement("keyword", tokenizer->keyword);
    // type
    advance();
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
    if (tokenizer->tokenType == KEYWORD) { // "int" | "char" | "boolean" | "void"
        typeName = tokenizer->keyword;
        writeElement("keyword", tokenizer->keyword);
    } else if (tokenizer->tokenType == IDENTIFIER) { // className
        typeName = tokenizer->identifier;
        writeElement("identifier", tokenizer->identifier);
    }
}

// メソッド、ファンクション、コンストラクタをコンパイルする
void CompilationEngine::compileSubroutine() {
    writeHeader("subroutineDec");
    // "function" | "constructor" | "method"
    writeElement("keyword", tokenizer->keyword);
    // ("void" | type)
    advance();
    compileType();
    // subroutineName()
    subroutineName = compileSubroutineName();
    // "("    
    advance();
    writeElement("symbol", tokenizer->symbol); // write "("
    // parameterList
    kind = S_ARG;
    int parameterNum = compileParameterList();
    // ")"
    writeElement("symbol", tokenizer->symbol);  // write ")"
    // TODO arg用の実装

    // compile subroutineBody
    compileSubroutineBody();

    writeFooter("subroutineDec");
    symbolTable->clearSubroutineScopeTable();
}

// パラメータのリストをコンパイルする
// パラメータは空の可能性もある
int CompilationEngine::compileParameterList() {
    writeHeader("parameterList");
    int parameterNum = 0;
    advance();
    while (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) { // ")"を見つけるまで
        if (tokenizer->tokenType == SYMBOL && tokenizer->symbol == ",") {
            writeElement("symbol", tokenizer->symbol);
            advance();
        }
        // type
        compileType();
        // varName
        compileVarName();
        parameterNum++;
        advance();
    }
    writeFooter("parameterList");
    return parameterNum;
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
    string funcName = className + "." + subroutineName;
    vmWriter->writeFunction(funcName, symbolTable->varCount(S_VAR));
    subroutineName = "";

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
    kind = S_VAR;
    writeElement("keyword", tokenizer->keyword);
    // type
    advance();
    compileType();
    // varName
    compileVarName();
    // (',' varName)*
    advance();
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
    className = tokenizer->identifier;
}

string CompilationEngine::compileSubroutineName() {
    advance();
    writeElement("identifier", tokenizer->identifier);
    return tokenizer->identifier;
}

string CompilationEngine::compileVarName() {
    advance();
    if (kind == S_STATIC || kind == S_FIELD || kind == S_ARG || kind == S_VAR) {
        symbolTable->define(tokenizer->identifier, typeName, kind);
    }
    string varName = tokenizer->identifier;
    writeElement("identifier", varName);
    return varName;
}

// 一連の文をコンパイルする
// 波括弧"{}"は含まない
void CompilationEngine::compileStatements() {
    writeHeader("statements");
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
    kind = S_LET;
    writeElement("keyword", tokenizer->keyword); // write "let"
    // varName
    string letVal = compileVarName();

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
    SymbolAttribute letKind = symbolTable->kindOf(letVal);
    Segment letSeg = symbolAttributeToSegment(letKind);
    vmWriter->writePop(letSeg, symbolTable->indexOf(letVal));

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
    int curIfIndex = ifIndex++;
    writeElement("symbol", tokenizer->symbol);
    vmWriter->writeIf("IF_TRUE" + to_string(curIfIndex));
    vmWriter->writeGoto("IF_FALSE" + to_string(curIfIndex));
    vmWriter->writeLabel("IF_TRUE" + to_string(curIfIndex));

    // "{"
    advance();
    writeElement("symbol", tokenizer->symbol);    
    // statements
    advance();
    compileStatements();
    // "}"
    writeElement("symbol", tokenizer->symbol);

    vmWriter->writeGoto("IF_END" + to_string(curIfIndex));
    vmWriter->writeLabel("IF_FALSE" + to_string(curIfIndex));
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
    vmWriter->writeLabel("IF_END" + to_string(curIfIndex));
    writeFooter("ifSatement");
}

// while文をコンパイルする
void CompilationEngine::compileWhile() {
    writeHeader("whileStatement");
    // while
    int curWhileIndex = whileIndex++;
    writeElement("keyword", tokenizer->keyword);
    vmWriter->writeLabel("WHILE_EXP" + to_string(curWhileIndex));
    // "("
    advance();
    writeElement("symbol", tokenizer->symbol);
    // expression
    advance();
    compileExpression();
    // ")"
    writeElement("symbol", tokenizer->symbol);
    vmWriter->writeArithmetic(AC_NOT);
    vmWriter->writeIf("WHILE_END" + to_string(curWhileIndex));

    // "{"
    advance();
    writeElement("symbol", tokenizer->symbol);
    // statements
    advance();    
    compileStatements();
    // "}"
    writeElement("symbol", tokenizer->symbol); 
    vmWriter->writeGoto("WHILE_EXP" + to_string(curWhileIndex));    
    vmWriter->writeLabel("WHILE_END" + to_string(curWhileIndex));    

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
    vmWriter->writePop(SEG_TEMP, 0);
}

// return文をコンパイルする
void CompilationEngine::compileReturn() {
    writeHeader("returnSatement");
    // "return"
    writeElement("keyword", tokenizer->keyword);

    advance();
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ";")) {
        compileExpression();
    } else {
        vmWriter->writePush(SEG_CONST, 0);
    }
    // ";""
    writeElement("symbol", tokenizer->symbol);
    writeFooter("returnSatement");
    vmWriter->writeReturn();
}

// 式をコンパイルする
void CompilationEngine::compileExpression() {
    writeHeader("expression");
    // term
    compileTerm();
    unordered_set<string> op = {"+", "-", "*", "/", "&", "|", "<", ">", "="};
    while (op.count(tokenizer->symbol)) {
        // op
        writeElement("symbol", tokenizer->symbol);
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
    writeFooter("expression");    
}

// termをコンパイルする
void CompilationEngine::compileTerm() {
    writeHeader("term");
    unordered_set<string> unaryOp = {"-", "~"};    
    TokenType tokenType = tokenizer->tokenType;
    if (tokenType == INT_CONST) {
        // integerConstant
        writeElement("integerConstant", to_string(tokenizer->intVal));
        vmWriter->writePush(SEG_CONST, tokenizer->intVal);
        advance();
    } else if (tokenType == STRING_CONST) {
        // stringConstant
        writeElement("stringConstant", tokenizer->stringVal);
        advance();
    } else if (tokenType == KEYWORD) {
        // keywordConstant
        writeElement("keyword", tokenizer->keyword);
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
            writeElement("symbol", tokenizer->symbol);                
            // expression
            advance();
            compileExpression();
            // ")"
            writeElement("symbol", tokenizer->symbol);
            advance();
        }
    } else if (tokenType == IDENTIFIER) {
        // "[" -> expression
        // "." -> subroutineCall
        writeElement("identifier", tokenizer->identifier);
        if (symbolTable->kindOf(tokenizer->identifier) != S_NONE) {
            SymbolAttribute termKind = symbolTable->kindOf(tokenizer->identifier);
            Segment termSeg = symbolAttributeToSegment(termKind);
            vmWriter->writePush(termSeg, symbolTable->indexOf(tokenizer->identifier));
        } else {
            subroutineName = tokenizer->identifier;
        }

        advance();
        if (tokenizer->tokenType == SYMBOL) {
            if (tokenizer->symbol == "[") {
                // "["
                writeElement("symbol", tokenizer->symbol);
                // expression
                advance();
                compileExpression();
                // "]"
                writeElement("symbol", tokenizer->symbol);
                advance();
            } else if (tokenizer->symbol == ".") {
                // "."
                writeElement("symbol", tokenizer->symbol);
                subroutineName += ".";
                // compileSubroutineCall
                advance();
                compileSubroutineCall();
                advance();
            }
        }
    }
    writeFooter("term");
}

void CompilationEngine::compileSubroutineCall() {
    // p1 : subroutineName(expressionList)
    // p2 : className.subroutineName(expressionList)
    // p3 : varName.subroutineName(expressionList)

    // subroutineName
    writeElement("identifier", tokenizer->identifier);
    subroutineName += tokenizer->identifier;

    advance();
    int parameterNum;
    if (tokenizer->symbol == "(") { // p1 : subroutineName(expressionList)
        // "("
        writeElement("symbol", tokenizer->symbol);
        // expressionList
        advance();
        parameterNum = compileExpressionList();
        // ")"
        writeElement("symbol", tokenizer->symbol);
    } else if (tokenizer->symbol == ".") { // p2,p3 : name.subroutineName(expressonList)
        // "."
        writeElement("symbol", tokenizer->symbol);        
        // "subroutineName"
        string subName = compileSubroutineName();
        subroutineName += "." + subName;
        // "("
        advance();
        writeElement("symbol", tokenizer->symbol);
        // expressionList
        advance();
        parameterNum = compileExpressionList();        
        // ")"
        writeElement("symbol", tokenizer->symbol);
    }
    vmWriter->writeCall(subroutineName, parameterNum);
    subroutineName = "";
}

// コンマで分離された式のリストをコンパイルする
int CompilationEngine::compileExpressionList() {
    writeHeader("expressionList");
    int expressionNum = 0;
    // (expression (, expression)* )?
    if (!(tokenizer->tokenType == SYMBOL && tokenizer->symbol == ")")) {
        // expression
        compileExpression();
        expressionNum++;
        // (",",  expression)*
        while (tokenizer->tokenType == SYMBOL && tokenizer->symbol == ",") {
            // ","
            writeElement("symbol", tokenizer->symbol);
            // expression
            advance();
            compileExpression();
            expressionNum++;
        }
    }
    writeFooter("expressionList");
    return expressionNum;
}

void CompilationEngine::compileUnaryOp() {
    // unaryOp
    writeElement("symbol", tokenizer->symbol);
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

Segment CompilationEngine::symbolAttributeToSegment(SymbolAttribute attr) {
    switch (attr) {
        case S_STATIC:
            return SEG_STATIC;
        case S_ARG:
            return SEG_ARG;
        case S_VAR:
            return SEG_LOCAL;
        default:
            return SEG_TEMP;
    }
}

// forDebug
void CompilationEngine::logF() {
    file << tokenizer->toString() << endl;
}