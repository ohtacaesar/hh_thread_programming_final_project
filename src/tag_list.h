#ifndef TAG_LIST_H
#define TAG_LIST_H

#include "tag.h"
#include "csv_reader.h"

struct TagList {
    struct Tag *tag;
    struct TagList *next;
};

// クラス的メソッド
struct TagList* TagList__create(struct Tag *tag);

struct TagList* TagList__create_from_csv(char *file_path);

// インスタンス的メソッド
int TagList_delete(struct TagList *tag_list);

struct TagList* TagList_create_next(struct TagList *prev_tag_list, struct Tag *tag);

#endif
