### 個人的要点

#### pop

pop attribute index
がSPの値をpopして、"attributeのindexに格納する"まで行うことに注意

#### pointer, this, that

pointer 0 = thisのベースアドレス
pointer 1 = thatのベースアドレス

配列はthatを使って、格納する
（
1, SPでa[i]を計算 -> push, add
2, pointer 1にa[i](= a + i)を設定 -> pop pointer 1
3, that(=a[i])に値を格納 -> pop that 0
）

#### arg index

methodのargはargumet 0はthisなので、１つずらす必要がある


### やり残し

関数化できるところ（変数のwritePush,writePopなど）はあるが、そこはやっていない

