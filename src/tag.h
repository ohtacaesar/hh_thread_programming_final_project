#ifndef TAG_H
#define TAG_H

#include "csv_column.h"

struct Tag {
    char *name;
    int view_count;
    int comment_count;
    int mylist_count;
};

// クラス的メソッド
struct Tag* Tag__create(char *name, int view_count, int comment_count, int mylist_count);

struct Tag* Tag__create_from_CsvColumn(struct CsvColumn *csv_column);

// インスタンス的メソッド
int Tag_delete(struct Tag *tag);

#endif
