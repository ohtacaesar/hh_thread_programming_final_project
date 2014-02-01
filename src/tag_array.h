#ifndef TAG_ARRAY_H
#define TAG_ARRAY_H

#include "tag.h"
#include "csv_reader.h"

struct TagArray {
    struct Tag **array;
    int size;
    int capacity;
};

// クラス的メソッド
struct TagArray* TagArray__create();

// インスタンス的メソッド
int TagArray_delete(struct TagArray *tag_array);

int TagArray_add(struct Tag *tag);

#endif
