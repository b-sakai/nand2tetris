

StaticTestだけ落ちる問題が合ったが、おそらく以下が原因
writeReturnの実装が怪しい？

-> CodeWriter::setFileNameを上手く呼べていなかったせいでシンボルが
　被ってしまっていた。。

http://nand2tetris-questions-and-answers-forum.52.s1.nabble.com/Wrong-test-script-for-StaticsTest-And-a-question-td4033684.html#a4033686