#include "stdlib.h"
#include "tag_list.h"

struct TagList* TagList__create(struct Tag *tag) {
    struct TagList *tag_list = (struct TagList*) malloc(sizeof(struct TagList));

    tag_list->tag  = tag;
    tag_list->next = NULL;

    return tag_list;
}

struct TagList* TagList__create_from_csv(char *file_path) {
    struct CsvReader *csv_reader = CsvReader__create();
    if(CsvReader_open(csv_reader, file_path) < 0) {
        CsvReader_delete(csv_reader);
        return NULL;
    }

    struct TagList   *tag_list = NULL, *current_tag_list = NULL;
    struct CsvColumn *csv_column = NULL;
    while((csv_column = CsvReader_gets(csv_reader)) != NULL) {
        struct Tag *tag = Tag__create_from_CsvColumn(csv_column);
        if(tag_list == NULL) {
            // 1週目
            tag_list = TagList__create(tag);
            if(tag_list == NULL) {
                TagList_delete(tag_list);
                CsvReader_delete(csv_reader);

                return NULL;
            }
            current_tag_list = tag_list;
        } else {
            // 2週目以降
            current_tag_list = TagList_create_next(current_tag_list, tag);
            if(current_tag_list == NULL) {
                TagList_delete(tag_list);
                CsvReader_delete(csv_reader);

                return NULL;
            }
        }
    }

    CsvReader_delete(csv_reader);

    return tag_list;
}

int TagList_delete(struct TagList *tag_list) {
    struct TagList *prev = NULL;

    while(tag_list != NULL) {
        prev = tag_list;
        tag_list = prev->next;

        if(Tag_delete(prev->tag) == 0) {
            prev->tag = NULL;
        } else {
        }
    }

    if(prev != NULL) {
        free(prev);
    }

    return 0;
}

struct TagList* TagList_create_next(struct TagList *prev_tag_list, struct Tag *tag) {
    struct TagList *tag_list = TagList__create(tag);

    if(tag_list == NULL) {
        return NULL;
    }
    prev_tag_list->next = tag_list;

    return tag_list;
}
