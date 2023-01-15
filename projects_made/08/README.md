## メモ


StaticTestだけ落ちる問題が合ったが、おそらく以下が原因
writeReturnの実装が怪しい？

-> CodeWriter::setFileNameを上手く呼べていなかったせいでシンボルが
　被ってしまっていた。。

http://nand2tetris-questions-and-answers-forum.52.s1.nabble.com/Wrong-test-script-for-StaticsTest-And-a-question-td4033684.html#a4033686

### call

* 関数を呼び出すとき、引数として以下のものを渡してしまうがが重要
（Argの指し示すアドレスごと変えてしまうことに注意）
** リターンアドレス
** 呼び出し元のLocalアドレス領域のアドレス
** 呼び出し元のArgアドレス領域のアドレス
** 呼び出し元のThisアドレス
** 呼び出し元のThatアドレス

### return
* 現在のARGの中身を使ってリターンする

