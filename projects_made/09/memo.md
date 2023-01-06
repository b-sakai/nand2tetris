## 注意点

* 

## Jack言語の仕様

* 仕様の表は本のp200
* 変数の仕様の表は本のp206
* Jack標準ライブラリの仕様の表は本のp210        
* コンストラクタの名前はnew推奨
* gcを持たないので、全てのクラスのdispose()というメソッドを持たせて、メソッド内で適切にメモリを開放する処理をカプセル化する



### 妥協点

コンパイラ実装を簡単にするために色々妥協している

* classはあるが、継承はない
* 型変換が弱い（ユーザー定義の抽象型Complexと配列の型変換が許される）
* 演算子の優先順位と評価順序がない