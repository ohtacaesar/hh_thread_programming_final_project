hh_thread_programming_final_project
===================================

スレッドプログラミング最終課題。ニコニコ動画の再生数、コメント数、マイリスト数をタグ毎に集計する。

MapReduce的にやる予定だったが、そうする必要もない気がしてきた。


## テストの動かし方
なんとなくgcc-4.8使ってるので、homebrewでインストールする。

OpenMPさえ使えればどのバージョンでも良いかもしれないが、変更する場合はmakefileを書き直す必要がある。と思ったが、うたぽよのSnow leopardだと動かなかったのでMarvericksにアップデートした。

Windowsでどう動かすか分からない。

    brew install gcc-4.8
    git clone git@github.com:ohtacaesar/hh_thread_programming_final_project.git
    cd hh_thread_programming_final_project
    make
    bin/test

## 処理の流れ

CsvReaderで1行ごとにタグ名、再生数、コメント数、マイリスト数をもつcsvのファイルを開いてレコード（行）のリストを取得し、タグの構造体の配列かリスト作成する。その後、配列かリストからタグ名毎に各値を集計して、表示する。

