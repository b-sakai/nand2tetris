pop attribute index
がSPの値をpopして、"attributeのindexに格納する"まで行うことに注意

pointer 0 = thisのベースアドレス
pointer 1 = thatのベースアドレス

配列はthatを使って、格納する
（
1, SPでa[i]を計算 -> push, add
2, pointer 1にa[i](= a + i)を設定 -> pop pointer 1
3, that(=a[i])に値を格納 -> pop that 0
）


