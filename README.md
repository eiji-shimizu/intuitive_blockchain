# intuitive_blockchain

## 概略
「長沼伸一郎. 現代経済学の直観的方法. 講談社, 2020.」のブロックチェーンについての章にある<br>
いくつかのお店が売り上げの台帳を分散管理しているイメージによるたとえの説明箇所を<br>
プログラム化したもの<br>
<br>

## ビルド
CMakeを利用する<br>
Windows上でのビルドにのみ対応<br>
<br>

## 機能
実行すると次の処理が繰り返される
1. 各店はそれぞれの取引を 10 明細分、それぞれで記録する
2. 10 明細分たまったら当番（今回ブロックチェーンに加える新たなブロックを作成する店）に送る
3. 当番は送られてきた明細をもとに新たなブロックを作成する
4. 当番は他の各店に作成したブロックを送る
5. 他の店が持つ台帳にも正式にブロックが追加される
6. 当番を変更する
  
店の数はmainメソッド内にリテラル表記している<br>
コマンドプロンプトにて実行中に、qを入力してEnter押下で終了する<br>
その終了時にカレントディレクトリに各店の明細記録がテキストファイルで出力される<br>
もし店の数が10であればdata0.txt ... data9.txt(10ファイル)が出力される<br>
この各ファイル内に実行時に各店のスレッドが乱数で作成した取引明細が<br>
10明細が1ブロックとなって、作成された分全てが記録される<br>
基本的には上記文献によるブロックチェーンの簡単な説明と同様の手順で<br>
前後のブロックがハッシュ値で連結されている<br>
  
実行中は、当番になった店が、ブロック作成前と送信後にコマンドプロンプトにログ出力する<br>
このためログ出力でどのくらいの明細が既に記録されたか(現在何周したのか)は推測できるが<br>
店の数を多くするとすぐに明細数が増加して、最後に出力されるファイルが大きくなるので注意<br>
