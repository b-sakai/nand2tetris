#include <cassert>
#include <iostream>
#include "SymbolTable.hpp"

// コンストラクタ
SymbolTable::SymbolTable() {
    classScopeTable.clear();
    subroutineScopeTable.clear();
}

// 新しい識別子を定義し、それに実行インデックスを割当てる
// @param name 識別子の名前
// @param type 識別子の型
// @param type 識別子の属性
void SymbolTable::define(string name, string type, SymbolAttribute kind) {
    int curCount = varCount(kind);
    SymbolInfo info;
    info.attribute = kind;
    info.typeName = type;
    info.index = curCount; // インデックスは0から始まるため (curCount-1) + 1 = curCount
    if (kind == S_STATIC || kind == S_FIELD) {
        // クラススコープのシンボルテーブルに定義する
        classScopeTable[name] = info;
    } else if (kind == S_VAR || kind == S_ARG) {
        // サブルーチンスコープのシンボルテーブルに定義する
        subroutineScopeTable[name] = info;
    } else {
        assert(false);
    }
    log();
}

// 引数で与えられた属性に対して、それが現在のスコープで定義されている数を返す
// @param kind 属性
int SymbolTable::varCount(SymbolAttribute kind) const {
    // 検索対象のテーブル
    if (kind == S_STATIC || kind == S_FIELD) {
        varCountCore(classScopeTable, kind);
    } else if (kind == S_VAR || kind == S_ARG) {
        varCountCore(subroutineScopeTable, kind);
    } else {
        assert(false);
    }
}

int SymbolTable::varCountCore(const unordered_map<string, SymbolInfo>& table, SymbolAttribute kind) const {
    int ret = 0;
    for (auto itr = table.begin(); itr != table.end(); ++itr) {
        SymbolInfo info = itr->second;
        if (info.attribute == kind) {
            ret++;
        }
    }    
    return ret;
}


// 引数で与えられた名前の識別子を現在のスコープで探し、その属性を返す
// その識別子が見つからなかったらNONEを返す
// @param 名前
SymbolAttribute SymbolTable::kindOf(string name) const {
    if (classScopeTable.count(name)) {
        return classScopeTable.at(name).attribute;
    } else if (subroutineScopeTable.count(name)) {
        return subroutineScopeTable.at(name).attribute;
    } else {
        return S_NONE;
    }
}

// 引数で与えられた名前の識別子を現在のスコープで探し、その型を返す
// @param 名前
string SymbolTable::typeOf(string name) const {
    if (classScopeTable.count(name)) {
        return classScopeTable.at(name).typeName;
    } else if (subroutineScopeTable.count(name)) {
        return subroutineScopeTable.at(name).typeName;
    } else {
        return "none";
    }
}

// 引数で与えられた名前の識別子を現在のスコープで探し、そのインデックスを返す
// @param 名前
int SymbolTable::indexOf(string name) const {
    if (classScopeTable.count(name)) {
        return classScopeTable.at(name).index;
    } else if (subroutineScopeTable.count(name)) {
        return subroutineScopeTable.at(name).index;
    } else {
        assert(false);
        return -1;
    }
}

// サブルーチン用のスコープを空にして初期化する
void SymbolTable::clearSubroutineScopeTable() {
    log();
    subroutineScopeTable.clear();
}

void SymbolTable::log() {
    cout << "-------------------" << endl;
    for (auto itr = classScopeTable.begin(); itr != classScopeTable.end(); ++itr) {
        SymbolInfo info = itr->second;
        cout << "class: " << itr->first << " : ";
        cout << info.attribute << "," << info.typeName << "," << info.index << endl;
    }            
    for (auto itr = subroutineScopeTable.begin(); itr != subroutineScopeTable.end(); ++itr) {
        SymbolInfo info = itr->second;
        cout << "subroutine: " << itr->first << " : ";
        cout << info.attribute << "," << info.typeName << "," << info.index << endl;        
    }        
    cout << "-------------------" << endl;
}