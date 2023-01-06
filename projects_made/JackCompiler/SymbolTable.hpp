#include <string>
#include <unordered_map>

using namespace std;

enum SymbolAttribute {
    S_STATIC,
    S_FIELD,
    S_ARG,
    S_VAR,
    S_LET,
    S_NONE
};

struct SymbolInfo {
    SymbolAttribute attribute;
    string typeName;
    int index;
};

class SymbolTable {
public:
    // クラス用のスコープのためのシンボルテーブル
    unordered_map<string, SymbolInfo> classScopeTable;
    // サブルーチン用のスコープのためのシンボルテーブル
    unordered_map<string, SymbolInfo> subroutineScopeTable;

public:
    // コンストラクタ
    SymbolTable();
    // 新しい識別子を定義し、それに実行インデックスを割当てる
    // @param name 識別子の名前
    // @param type 識別子の型
    // @param type 識別子の属性
    void define(string name, string type, SymbolAttribute kind);
    // 引数で与えられた属性に対して、それが現在のスコープで定義されている数を返す
    // @param kind 属性
    int varCount(SymbolAttribute kind) const;
    // 引数で与えられた名前の識別子を現在のスコープで探し、その属性を返す
    // その識別子が見つからなかったらNONEを返す
    // @param 名前
    SymbolAttribute kindOf(string name) const;
    // 引数で与えられた名前の識別子を現在のスコープで探し、その型を返す
    // @param 名前
    string typeOf(string name) const;
    // 引数で与えられた名前の識別子を現在のスコープで探し、そのインデックスを返す
    // @param 名前
    int indexOf(string name) const;
    // サブルーチン用のスコープを空にして初期化する
    void clearSubroutineScopeTable();
    // ログ出力を行う
    void log();

protected:
    int varCountCore(const unordered_map<string, SymbolInfo>& table, SymbolAttribute kind) const;
};