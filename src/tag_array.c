#include "stdlib.h"
#include "tag_array.h"

#define DEFAULT_TAG_ARRAY_CAPACITY 512

struct TagArray* TagArray__create() {
    return TagArray__create_with_capacity(DEFAULT_TAG_ARRAY_CAPACITY);
}

struct TagArray* TagArray__create_with_capacity(int capacity) {
    struct TagArray *tag_array = 
        (struct TagArray*) malloc(sizeof(struct TagArray));
    if(tag_array == NULL) {
        return NULL;
    }

    tag_array->size = 0;
    tag_array->capacity = capacity;
    tag_array->array = 
        (struct Tag**) malloc(sizeof(struct Tag*) * tag_array->capacity);
    if(tag_array->array == NULL) {
        free(tag_array);
        return NULL;
    }

    return tag_array;
}

struct TagArray* TagArray__create_from_csv(char *file_path) {
    struct CsvReader *csv_reader = CsvReader__create();
    if(CsvReader_open(csv_reader, file_path) < 0) {
        CsvReader_delete(csv_reader);
        return NULL;
    }

    struct TagArray *tag_array = TagArray__create();
    struct CsvColumn *csv_column = NULL;
    while((csv_column = CsvReader_gets(csv_reader)) != NULL) {
        struct Tag *tag = Tag__create_from_CsvColumn(csv_column);
        // 失敗を表す数値が1だったり-1だったりばらばら…
        if(TagArray_add(tag_array, tag) == 1) {
            CsvReader_delete(csv_reader);
            TagArray_delete(tag_array);

            return NULL;
        }
    }

    return tag_array;
}


int TagArray_delete(struct TagArray *tag_array) {
    if(tag_array == NULL) {
        return 0;
    }

    free(tag_array->array);
    free(tag_array);

    return 0;
}

int TagArray_add(struct TagArray *tag_array, struct Tag *tag) {
    if(tag_array == NULL || tag == NULL) {
        return 1;
    }

    // capacityいっぱいになったら2倍に拡張
    if(tag_array->size >= tag_array->capacity) {
        if(TagArray_update_capacity(tag_array, tag_array->capacity * 2) == 1) {
            return 1;
        }
    }
    tag_array->array[tag_array->size++] = tag;

    return 0;
}

int TagArray_update_capacity(struct TagArray *tag_array, int new_capacity) {
    if(tag_array == NULL) {
        return 1;
    }

    if(new_capacity < tag_array->size) {
        return 1;
    }

    struct Tag **temp = (struct Tag **) realloc(
            tag_array->array, sizeof(struct Tag *) * new_capacity); 
    if(temp == NULL) {
        perror("realloc");
        return 1;
    }

    tag_array->array    = temp;
    tag_array->capacity = new_capacity;

    return 0;
}

