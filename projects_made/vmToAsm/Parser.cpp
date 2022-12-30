#include "Parser.hpp"
#include <cstring>
#include <cassert>

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

// コンストラクタ
Parser::Parser(string filename) {
    file.open(filename);
}

// 次のコマンドを読み、現在のコマンドとする
void Parser::advance() {
    string line = "";
    if (getline(file, line)) { // 一行呼んで最終行のとき
    } else {
        hasMoreCommands = false;
        cout << "last" << endl;
        return;
    }
    if (line.size() == 1) {
        advance();
        return;
    }
    auto separator = std::string(" ");
    auto separator_length = separator.length();
    auto list = vector<string>();


    auto offset = string::size_type(0);
    while (true) {
        auto pos = line.find(separator, offset);
        if (pos == string::npos) {
            list.push_back(line.substr(offset));
            break;
        }
        list.push_back(line.substr(offset, pos - offset));
        offset = pos + separator_length;
    }
    if (list[0] == "//") {
        advance();
        return;
    }

    command = trim(list[0]);
    commandType = judgeCommandType(list[0]);
    arg1 = "";
    arg2 = -1;
    if (list.size() >= 2) {
        arg1 = list[1];
        if (list.size() >= 3) {
            arg2 = stoi(list[2]);
        }
    }
}

CommandType Parser::judgeCommandType(string aCommand) const {
    cout << "----------" << endl;
    cout << aCommand << endl;
    cout << aCommand.size() << endl;
    cout << "----------" << endl;
    CommandType ret;
    if (aCommand == "add" || aCommand == "sub" || aCommand == "neg" ||
        aCommand == "eq" || aCommand == "gt" || aCommand == "lt" ||
        aCommand == "and" || aCommand == "or" || aCommand == "not") {
        cout << "judged : " << aCommand << endl;
        ret = C_ARITHMETIC;
    } else if (aCommand == "push") {
        ret = C_PUSH;
    } else {
        cout << "should not called " << aCommand << endl;
    }
    return ret;
}

