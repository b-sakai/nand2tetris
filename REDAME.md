## chapter 6 assembler

* .asm -> .hackの変換を行う
* projects_made/a.pyがそれを行う
* （※実装できていない箇所有り）

### 実行方法

* a.pyの内部でパスを記述
* python3 a.py

## chapter 7, 8 vmToAsm
* .vm -> .asmの変換を行う
* projects_made/vmToAsm/以下にプログラム群がある

### 実行方法

* vmToAsm.cpp内のifilename, ofilenameにパスを記述
* sh run.shでコンパイルして実行

### 各ファイルの説明

* CommandType.hpp : CommandTypeのenumを定義
* Parser.hpp/cpp : Parserモジュールを実装
* CodeWriter.hpp/cpp : CodeWriterモジュールを実装
* vmToAsm.cpp : メインプログラム。
* vmToAsmStack.cpp : 7章までの内容に対応した変換プログラム

