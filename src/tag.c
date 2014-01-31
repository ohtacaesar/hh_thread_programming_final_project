#include <stdlib.h>
#include <string.h>
#include "tag.h"

struct Tag* Tag__create(char *name, int view_count, int comment_count, int mylist_count) {
    struct Tag *tag = (struct Tag*) malloc(sizeof(struct Tag));

    tag->name = (char*) malloc(sizeof(char) * strlen(name));
    strncpy(tag->name, name, strlen(name));
    tag->view_count    = view_count;
    tag->comment_count = comment_count;
    tag->mylist_count  = mylist_count;

    return tag;
}

struct Tag* Tag__create_from_CsvColumn(struct CsvColumn *csv_column) {
    char *name = csv_column->value;
    if(name == NULL) {
        return NULL;
    }

    int counts[3], i;
    for(i = 0; i < 3; i++) {
        csv_column = csv_column->next;
        if(csv_column == NULL) {
            return NULL;
        }

        char *temp = csv_column->value;
        if(temp == NULL) {
            return NULL;
        }
        counts[i] = atoi(temp);
    }

    return Tag__create(name, counts[0], counts[1], counts[2]);
}

int Tag_delete(struct Tag *tag) {
    // TagがNULLだった場合、どうやってそれを呼び出し元に伝えるか
    if(tag == NULL) {
        return 0;
    }

    free(tag->name);
    tag->name = NULL;
    free(tag);
    tag = NULL;

    return 0;
}

