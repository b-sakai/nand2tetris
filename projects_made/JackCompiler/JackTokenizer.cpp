#include <cstring>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include "JackTokenizer.hpp"

const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

unordered_set<string> keywords {
    "class",
    "constructor",
    "function",
    "method",
    "field",
    "static",    
    "var",
    "int",
    "char",
    "boolean",
    "void",
    "true",
    "false",
    "null",
    "this",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return"
};

unordered_set<char> symbols{
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '=',
    '~'
};

// コンストラクタ
JackTokenizer::JackTokenizer(string filename) {
    file.open(filename);
}

// 次のトークンを取得し、それを現在のトークンとする
void JackTokenizer::advance() {
    if (tokens.empty()) {
        advanceSpace();
    }
    // １字ずつ読み込む
    string token = "";
    int i = 0;
    int tokenSize = tokens.size();
    while (i < tokens.size()) {
        if (symbols.count(tokens[i])) {
            if (i == 0) {
                token = token + tokens[i];
                i++;
                tokens = tokens.substr(i, tokens.size());
            } else {
                tokens = tokens.substr(i, tokens.size());
                i++;
            }
            break;
        }
        token = token + tokens[i];
        i++;
    }
    if (!symbols.count(tokens[0]) && i == tokenSize) {
        tokens = "";
    }
    if (keywords.count(trim(token))) {
        tokenType = KEYWORD;
        keyword = trim(token);
    } else if (symbols.count(token[0])) {
        tokenType = SYMBOL;
        symbol = token;
    } else if (isdigit(token[0])) {
        tokenType = INT_CONST;
        intVal = stoi(token);
    } else if (token[0] == '"') {
        if (token.at(token.size()-1) == '"') {
            tokenType = STRING_CONST;
            token.erase(std::remove(token.begin(), token.end(), '"'), token.end());
            stringVal = token;
        } else {
            tokenType = EMPTY;
            tokens = token + " ";
            advanceSpace();
            return;
        }
    } else if (!trim(token).empty()) {
        tokenType = IDENTIFIER;
        identifier = trim(token);
    } else {
        tokenType = EMPTY;
    }

    // cout << tokenType << " :  「" << token << "」" << endl;
}

// スペース区切りでファイルを読み、tokensに格納する
// コメントはここで取り除く
void JackTokenizer::advanceSpace() {
    string line = "";
    if (getline(file, line, ' ')) { // 一行呼んで最終行のとき
    } else {
        hasMoreTokens = false;
        file.close();
        cout << "last" << endl;
        return;
    }
    tokens += trim(line);
    if (tokens.size() >= 2 && tokens.substr(0, 2) == "//") { // 1行コメントのとき
        getline(file, line);
        tokens = "";
        advanceSpace();
        return;
    }
    if (tokens.size() >= 2 && tokens.substr(0, 2) == "/*") { // 複数行コメントのとき
        getline(file, line);
        line = trim(line);
        while (line.substr(line.size()-2, 2) != "*/") {
            getline(file, line);
            line = trim(line);
        }
        tokens = "";
        advanceSpace();
        return;
    }
    if (tokens.empty() || tokens == "\n") { // 空白、改行コードのとき
        tokens = "";
        advanceSpace();
        return;
    }
 }

// for debug
void JackTokenizer::log() {
    if (tokenType == KEYWORD) {
        cout << "LLLLL keyword : " << keyword << endl;
    } else if (tokenType == SYMBOL) {
        cout << "LLLLL symbol : " << symbol << endl;
    } else if (tokenType == IDENTIFIER) {
        cout << "LLLLL identifier : " << identifier << endl;
    } else if (tokenType == INT_CONST) {
        cout << "LLLLL int_const : " << intVal << endl;
    } else if (tokenType == STRING_CONST) {
        cout << "LLLLL string_const : " << stringVal << endl;
    } else {
        cout << "LLLLL empty : " << endl;
    }
}