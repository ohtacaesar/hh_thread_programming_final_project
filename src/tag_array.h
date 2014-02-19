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

struct TagArray* TagArray__create_with_capacity(int capacity);

struct TagArray* TagArray__create_from_csv(char *file_path);

// インスタンス的メソッド
int TagArray_delete(struct TagArray *tag_array);

int TagArray_add(struct TagArray *tag_array, struct Tag *tag);

int TagArray_sort(struct TagArray *tag_array);

#endif
