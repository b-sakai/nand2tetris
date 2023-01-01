ここでは「構文解析」を行う
（次の章で「コード生成」を行う）

構文解析の内訳
・字句解析 lexical analysis
・文脈自由文法 context-free grammer
・構文木 parse tree
・再帰降下アルゴリズム 

言語変換、構文解析、木、ハッシュテーブルなどのデータ構造の使い方、再帰的なコンパイルアルゴリズム

応用先
・コンピュータ・グラフィクス
・データベース管理
・通信プロトコル
・バイオインフォマティクス
・複雑な構文を持つファイルの構文解析、処理操作

構文解析のモジュール
・トークナイザ（tokenizer）：　トークン（字句）に変換
・パーサ（parser）：一連のトークンを言語の構文ルールに適合させ、その構文構造を明らかにする

構文解析の作業はターゲット言語と完全に独立している！


## 手順

### JackTokenizer

* JackTokenizer.hpp/cppを作成
* xml出力のためのライブラリを検討
* APIのメンバ、メソッドをJackTokenizer.cppに用意する
* JackAnalyzer.cppを実装してJackTokenizerを呼ぶ
* Square.jackを変換してSquareT.xmlと比較する（出力ファイル名はMySquare.xmlなどが良い）
* SquareGame.jackを変換してSquareT.xmlと比較する（出力ファイル名はMySquare.xmlなどが良い）
* ArrayTest/Main.jackを変換してMainT.xmlと比較する

### CompilerEngine

* ComplierEngine.hpp/cppを作成
* APIのメンバ、メソッドをComplierEngine.cppに用意する
* JackAnalyzer.cppからCompilerEngineを呼ぶ

* Square.jackを変換してSquareT.xmlと比較する（出力ファイル名はMySquare.xmlなどが良い）
* SquareGame.jackを変換してSquareT.xmlと比較する（出力ファイル名はMySquare.xmlなどが良い）
* ArrayTest/Main.jackを変換してMainT.xmlと比較する


