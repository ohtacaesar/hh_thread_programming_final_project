#include "stdlib.h"
#include "tag_array.h"

#define DEFAULT_TAG_ARRAY_CAPACITY 512

struct TagArray* TagArray__create() {
    struct TagArray *tag_array = (struct TagArray*) malloc(sizeof(struct TagArray));
    if(tag_array == NULL) {
        return NULL;
    }

    tag_array->size = 0;
    tag_array->capacity = DEFAULT_TAG_ARRAY_CAPACITY;
    tag_array->array = (struct Tag**) malloc(sizeof(struct Tag*) * tag_array->capacity);
    if(tag_array->array == NULL) {
        free(tag_array);
        return NULL;
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


