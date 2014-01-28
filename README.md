hh_thread_programming_final_project
===================================

スレッドプログラミング最終課題。ニコニコ動画の再生数、コメント数、マイリスト数をタグ毎に集計する。

MapReduce的にやる予定だったが、そうする必要もない気がしてきた。


## テストの動かし方
なんとなくgcc-4.8使ってるので、homebrewでインストールする。

openmpさえ使えればどのバージョンでも良いかもしれないが、変更する場合はmakefileを書き直す必要がある。

Windowsでどう動かすか分からない。

    brew install gcc-4.8
    make
    bin/test
