================================================================================
   Unique.rb
================================================================================

・Unique.rb
　　重複しているファイルを検出・削除するrubyスクリプト

・使い方
　　ruby unique.rb path1 [path2...] [options]
　　
　　Options
　　-x
　　--exec
　　　削除を行う。このオプションを付けない限りファイルの削除は行われない。
　　
　　-r
　　--recursive
　　  ディレクトリを再帰的に辿る。
　　
　　-d
　　--recursive-dir-first
　　  -rと併せて使う。深い階層にあるファイルを優先する。
　　  詳細は後述。

・ファイルの削除
　　重複するファイル群を発見した場合、unique.rbが始めに見つけたファイルを保存し、
　　後から見つけたファイルを削除する。
　　
　　unique.rbがファイルを探す順番は以下の規則による。
　　
　　1.別ディレクトリのファイル
　　  先に指定したディレクトリのファイルを保存。
　　   DirA
　　   └ foo.txt
　　   DirB
　　   └ foobarbaz.txt

　　  $ ruby unique.rb DirA DirB
　　    foo.txt を保存
　　    foobarbaz.txt を削除

　　2.サブディレクトリ内のファイル(-r)が指定されたとき
　　  (-dがないとき)浅い階層のファイルを保存。
　　  (-dがあるとき)深い階層のファイルを保存。
　　   DirA
　　   ├ SubDir
　　   │ └ foobarbaz.txt
　　   └ foo.txt

　　  $ ruby unique.rb -r DirA
　　    foo.txt を保存
　　    foobarbaz.txt を削除

　　  $ ruby unique.rb -rd DirA
　　    foobarbaz.txt を保存
　　    foo.txt を削除

　　3. 同じディレクトリ内のファイル
　　   ファイル名の長さが短いものを保存。
　　   DirA
　　   ├ foo.txt
　　   └ foobarbaz.txt

　　   $ ruby unique.rb DirA
　　     foo.txt を保存
　　     foobarbaz.txt を削除

