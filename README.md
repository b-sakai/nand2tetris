# nand2tetris

演習の実装まとめ
リファクタリングなどしていないため雑な実装が多い

## chapter 1-5

* projects_made/1などの各チャプターのディレクトリに各モジュールの*.hdlファイルに実装がある

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

## chapter 9

* 特に実装はせず

## chapter 10, 11 JackCompiler

* .jack -> .vmのコンパイルを行う
* projects_made/JackCompiler10/以下にxmlファイルを出力するまで行うプログラム軍がある
* projects_made/JackCompiler/以下に完全にコンパイルを行うプログラム群がある
* 詳しい説明は各ディレクトリを参照